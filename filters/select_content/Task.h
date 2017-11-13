/*
    Scan Tailor - Interactive post-processing tool for scanned pages.
    Copyright (C) 2007-2015  Joseph Artsimovich <joseph.artsimovich@gmail.com>

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

#ifndef SELECT_CONTENT_TASK_H_
#define SELECT_CONTENT_TASK_H_

#include "NonCopyable.h"
#include "RefCountable.h"
#include "FilterResult.h"
#include "PageId.h"
#include "CachingFactory.h"
#include "acceleration/AcceleratableOperations.h"
#include <QSizeF>
#include <QRectF>
#include <memory>

class TaskStatus;
class DebugImagesImpl;
class QImage;

namespace imageproc
{
	class BinaryImage;
	class GrayImage;
	class AffineImageTransform;
	class AffineTransformedImage;
}

namespace page_layout
{
	class Task;
}

namespace select_content
{

class Filter;
class Settings;
class Params;

class Task : public RefCountable
{
	DECLARE_NON_COPYABLE(Task)
public:
	Task(IntrusivePtr<Filter> const& filter,
		IntrusivePtr<Settings> const& settings,
		IntrusivePtr<page_layout::Task> const& next_task,
		PageId const& page_id, bool batch, bool debug);
	
	virtual ~Task();
	
	FilterResultPtr process(
		TaskStatus const& status,
		std::shared_ptr<AcceleratableOperations> const& accel_ops,
		QImage const& orig_image,
		CachingFactory<imageproc::GrayImage> const& gray_orig_image_factory,
		std::shared_ptr<imageproc::AbstractImageTransform const> const& orig_image_transform);
private:
	class ManualModeUiUpdater;
	class ContentModeUiUpdater;
	class PageModeUiUpdater;
	class ImageModeUiUpdater;
	
	FilterResultPtr processManualMode(
		TaskStatus const& status,
		std::shared_ptr<AcceleratableOperations> const& accel_ops,
		QImage const& orig_image,
		CachingFactory<imageproc::GrayImage> const& gray_orig_image_factory,
		imageproc::AffineImageTransform const& orig_image_transform, Params& params);

	FilterResultPtr processContentMode(
		TaskStatus const& status,
		std::shared_ptr<AcceleratableOperations> const& accel_ops,
		QImage const& orig_image,
		CachingFactory<imageproc::GrayImage> const& gray_orig_image_factory,
		imageproc::AffineImageTransform const& orig_image_transform, Params& params);

	FilterResultPtr processPageMode(
		TaskStatus const& status,
		std::shared_ptr<AcceleratableOperations> const& accel_ops,
		QImage const& orig_image,
		CachingFactory<imageproc::GrayImage> const& gray_orig_image_factory,
		imageproc::AffineImageTransform const& orig_image_transform, Params& params);

	FilterResultPtr processImageMode(
		TaskStatus const& status,
		std::shared_ptr<AcceleratableOperations> const& accel_ops,
		QImage const& orig_image,
		CachingFactory<imageproc::GrayImage> const& gray_orig_image_factory,
		imageproc::AffineImageTransform const& orig_image_transform, Params& params);

	static void cleanup(TaskStatus const& status, imageproc::BinaryImage& img);

	IntrusivePtr<Filter> m_ptrFilter;
	IntrusivePtr<Settings> m_ptrSettings;
	IntrusivePtr<page_layout::Task> m_ptrNextTask;
	std::auto_ptr<DebugImagesImpl> m_ptrDbg;
	PageId m_pageId;
	bool m_batchProcessing;
};

} // namespace select_content

#endif
