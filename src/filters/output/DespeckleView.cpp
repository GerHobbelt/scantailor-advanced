/*
    Scan Tailor - Interactive post-processing tool for scanned pages.
    Copyright (C) 2015  Joseph Artsimovich <joseph.artsimovich@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <memory>
#include <exception>
#include <QPointer>
#include <QDebug>
#include "DespeckleView.h"
#include "DespeckleVisualization.h"
#include "Despeckle.h"
#include "AbstractCommand.h"
#include "BackgroundExecutor.h"
#include "BackgroundTask.h"
#include "ImageViewBase.h"
#include "BasicImageView.h"
#include "OutputMargins.h"
#include "ProcessingIndicationWidget.h"
#include "DebugImagesImpl.h"
#include "TabbedDebugImages.h"
#include "AutoRemovingFile.h"
#include "TaskStatus.h"
#include "imageproc/BinaryImage.h"
#include "imageproc/RasterOp.h"

using namespace imageproc;

namespace output
{

class DespeckleView::TaskCancelException : public std::exception
{
public:
    virtual char const* what() const throw()
    {
        return "Task cancelled";
    }
};


class DespeckleView::TaskCancelHandle : public TaskStatus, public RefCountable
{
public:
    virtual void cancel();

    virtual bool isCancelled() const;

    virtual void throwIfCancelled() const;
private:
    QAtomicInt m_cancelFlag;
};


class DespeckleView::DespeckleTask :
    public AbstractCommand0<BackgroundExecutor::TaskResultPtr>
{
public:
    DespeckleTask(
        DespeckleView* owner,
        std::shared_ptr<AcceleratableOperations> const& accel_ops,
        DespeckleState const& despeckle_state,
        IntrusivePtr<TaskCancelHandle> const& cancel_handle,
        double new_factor, bool debug);

    virtual BackgroundExecutor::TaskResultPtr operator()();
private:
    QPointer<DespeckleView> m_ptrOwner;
    std::shared_ptr<AcceleratableOperations> m_ptrAccelOps;
    DespeckleState m_despeckleState;
    IntrusivePtr<TaskCancelHandle> m_ptrCancelHandle;
    std::unique_ptr<DebugImagesImpl> m_ptrDbg;
    DespeckleLevel m_despeckleLevel;
    double m_despeckleFactor;
};


class DespeckleView::DespeckleResult : public AbstractCommand0<void>
{
public:
    DespeckleResult(
        QPointer<DespeckleView> const& owner,
        IntrusivePtr<TaskCancelHandle> const& cancel_handle,
        DespeckleState const& despeckle_state,
        DespeckleVisualization const& visualization,
        std::unique_ptr<DebugImagesImpl>& debug_images);

    // This method is called from the main thread.
    virtual void operator()();
private:
    QPointer<DespeckleView> m_ptrOwner;
    IntrusivePtr<TaskCancelHandle> m_ptrCancelHandle;
    std::unique_ptr<DebugImagesImpl> m_ptrDbg;
    DespeckleState m_despeckleState;
    DespeckleVisualization m_visualization;
};


/*============================ DespeckleView ==============================*/

DespeckleView::DespeckleView(
    std::shared_ptr<AcceleratableOperations> const& accel_ops,
    DespeckleState const& despeckle_state,
    DespeckleVisualization const& visualization, bool debug)
    :	m_ptrAccelOps(accel_ops),
      m_despeckleState(despeckle_state),
      m_pProcessingIndicator(new ProcessingIndicationWidget(this)),
      m_despeckleFactor(despeckle_state.factor()),
      m_debug(debug)
{
    addWidget(m_pProcessingIndicator);

    if (!visualization.isNull())
    {
        // Create the image view.
        std::auto_ptr<QWidget> widget(
            new BasicImageView(accel_ops, visualization.image(), visualization.downscaledImage())
        );
        setCurrentIndex(addWidget(widget.release()));
    }
}

DespeckleView::~DespeckleView()
{
    cancelBackgroundTask();
}

void
DespeckleView::despeckleLevelChanged(double const new_factor, bool* handled)
{
    if (new_factor == m_despeckleFactor)
    {
        return;
    }

    m_despeckleFactor = new_factor;

    if (isVisible())
    {
        *handled = true;
        if (currentWidget() == m_pProcessingIndicator)
        {
            initiateDespeckling(RESUME_ANIMATION);
        }
        else
        {
            initiateDespeckling(RESET_ANIMATION);
        }
    }
}

void
DespeckleView::hideEvent(QHideEvent* evt)
{
    QStackedWidget::hideEvent(evt);

    // We don't want background despeckling to continue when user
    // switches to another tab.
    cancelBackgroundTask();
}

void
DespeckleView::showEvent(QShowEvent* evt)
{
    QStackedWidget::showEvent(evt);

    if (currentWidget() == m_pProcessingIndicator)
    {
        initiateDespeckling(RESET_ANIMATION);
    }
}

void
DespeckleView::initiateDespeckling(AnimationAction const anim_action)
{
    removeImageViewWidget();
    if (anim_action == RESET_ANIMATION)
    {
        m_pProcessingIndicator->resetAnimation();
    }
    else
    {
        m_pProcessingIndicator->processingRestartedEffect();
    }

    cancelBackgroundTask();
    m_ptrCancelHandle.reset(new TaskCancelHandle);

    // Note that we are getting rid of m_initialSpeckles,
    // as we wouldn't need it any more.

    BackgroundExecutor::TaskPtr const task(
        new DespeckleTask(
            this, m_ptrAccelOps, m_despeckleState,
            m_ptrCancelHandle, m_despeckleFactor, m_debug
        )
    );
    ImageViewBase::backgroundExecutor().enqueueTask(task);
}

void
DespeckleView::despeckleDone(
    DespeckleState const& despeckle_state,
    DespeckleVisualization const& visualization, DebugImagesImpl* dbg)
{
    assert(!visualization.isNull());

    m_despeckleState = despeckle_state;

    removeImageViewWidget();

    std::auto_ptr<QWidget> widget(
        new BasicImageView(
            m_ptrAccelOps, visualization.image(),
            visualization.downscaledImage(), OutputMargins()
        )
    );

    if (dbg && !dbg->empty())
    {
        std::auto_ptr<TabbedDebugImages> tab_widget(new TabbedDebugImages);
        tab_widget->addTab(widget.release(), "Main");
        IntrusivePtr<DebugViewFactory> factory;
        QString label;
        while ((factory = dbg->retrieveNext(&label)))
        {
            tab_widget->addTab(new DebugImageView(factory), label);
        }
        widget = tab_widget;
    }

    setCurrentIndex(addWidget(widget.release()));
}

void
DespeckleView::cancelBackgroundTask()
{
    if (m_ptrCancelHandle)
    {
        m_ptrCancelHandle->cancel();
        m_ptrCancelHandle.reset();
    }
}

void
DespeckleView::removeImageViewWidget()
{
    // Widget 0 is always m_pProcessingIndicator, so we start with 1.
    // Also, normally there can't be more than 2 widgets, but just in case ...
    while (count() > 1)
    {
        QWidget* wgt = widget(1);
        removeWidget(wgt);
        delete wgt;
    }
}


/*============================= DespeckleTask ==========================*/

DespeckleView::DespeckleTask::DespeckleTask(
    DespeckleView* owner,
    std::shared_ptr<AcceleratableOperations> const& accel_ops,
    DespeckleState const& despeckle_state,
    IntrusivePtr<TaskCancelHandle> const& cancel_handle,
    double const factor, bool const debug)
    :	m_ptrOwner(owner),
      m_ptrAccelOps(accel_ops),
      m_despeckleState(despeckle_state),
      m_ptrCancelHandle(cancel_handle),
      m_despeckleFactor(factor)
{
    if (debug)
    {
        m_ptrDbg.reset(new DebugImagesImpl);
    }
}

BackgroundExecutor::TaskResultPtr
DespeckleView::DespeckleTask::operator()()
{
    try
    {
        m_ptrCancelHandle->throwIfCancelled();

        m_despeckleState = m_despeckleState.redespeckle(
                               m_despeckleFactor, *m_ptrCancelHandle, m_ptrDbg.get()
                           );

        m_ptrCancelHandle->throwIfCancelled();

        DespeckleVisualization visualization(m_despeckleState.visualize(m_ptrAccelOps));

        m_ptrCancelHandle->throwIfCancelled();

        return BackgroundExecutor::TaskResultPtr(
                   new DespeckleResult(
                       m_ptrOwner, m_ptrCancelHandle,
                       m_despeckleState, visualization, m_ptrDbg
                   )
               );
    }
    catch (TaskCancelException const&)
    {
        return BackgroundExecutor::TaskResultPtr();
    }
}


/*======================== DespeckleResult ===========================*/

DespeckleView::DespeckleResult::DespeckleResult(
    QPointer<DespeckleView> const& owner,
    IntrusivePtr<TaskCancelHandle> const& cancel_handle,
    DespeckleState const& despeckle_state,
    DespeckleVisualization const& visualization,
    std::unique_ptr<DebugImagesImpl>& debug_images)
    :	m_ptrOwner(owner),
      m_ptrCancelHandle(cancel_handle),
      m_despeckleState(despeckle_state),
      m_visualization(visualization)
{
    m_ptrDbg = std::move(debug_images);
}

void
DespeckleView::DespeckleResult::operator()()
{
    if (m_ptrCancelHandle->isCancelled())
    {
        return;
    }

    if (DespeckleView* owner = m_ptrOwner)
    {
        owner->despeckleDone(m_despeckleState, m_visualization, m_ptrDbg.get());
    }
}


/*========================= TaskCancelHandle ============================*/

void
DespeckleView::TaskCancelHandle::cancel()
{
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    m_cancelFlag.store(1);
#else
    m_cancelFlag.storeRelaxed(1);
#endif
}

bool
DespeckleView::TaskCancelHandle::isCancelled() const
{
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    return m_cancelFlag.load() != 0;
#else
    return m_cancelFlag.loadRelaxed() != 0;
#endif
}

void
DespeckleView::TaskCancelHandle::throwIfCancelled() const
{
    if (isCancelled())
    {
        throw TaskCancelException();
    }
}

} // namespace output
