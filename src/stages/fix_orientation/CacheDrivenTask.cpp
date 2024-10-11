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

#include "CacheDrivenTask.h"
#include "Settings.h"
#include "PageInfo.h"
#include "PageId.h"
#include "ImageId.h"
#include "ThumbnailBase.h"
#include "filter_dc/AbstractFilterDataCollector.h"
#include "filter_dc/ThumbnailCollector.h"
#include "filter_dc/PageOrientationCollector.h"
#include "stages/page_split/CacheDrivenTask.h"

using namespace imageproc;

namespace fix_orientation
{

CacheDrivenTask::CacheDrivenTask(
    IntrusivePtr<Settings> const& settings,
    IntrusivePtr<page_split::CacheDrivenTask> const& next_task)
    : m_ptrNextTask(next_task),
      m_ptrSettings(settings)
{
}

CacheDrivenTask::~CacheDrivenTask()
{
}

void
CacheDrivenTask::process(
    PageInfo const& page_info, AffineImageTransform const& image_transform,
    AbstractFilterDataCollector* collector)
{
    OrthogonalRotation const rotation(m_ptrSettings->getRotationFor(page_info.imageId()));

    if (PageOrientationCollector* col = dynamic_cast<PageOrientationCollector*>(collector))
    {
        col->process(rotation);
    }

    AffineImageTransform rotated_transform(image_transform);
    rotated_transform.rotate(rotation.toDegrees());

    if (m_ptrNextTask)
    {
        m_ptrNextTask->process(
            page_info, rotation, std::move(rotated_transform), collector
        );
        return;
    }

    if (ThumbnailCollector* thumb_col = dynamic_cast<ThumbnailCollector*>(collector))
    {
        thumb_col->processThumbnail(
            std::unique_ptr<QGraphicsItem>(
                new ThumbnailBase(
                    thumb_col->thumbnailCache(),
                    thumb_col->maxLogicalThumbSize(),
                    page_info.id(),
                    std::move(rotated_transform)
                )
            )
        );
    }
}

} // namespace fix_orientation
