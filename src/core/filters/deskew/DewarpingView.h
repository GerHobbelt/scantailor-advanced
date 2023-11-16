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

#ifndef DESKEW_DEWARPING_VIEW_H_
#define DESKEW_DEWARPING_VIEW_H_

#include "ImageViewBase.h"
#include "ImageTransformation.h"
#include "DragHandler.h"
#include "ZoomHandler.h"

namespace deskew
{

class DewarpingView : public ImageViewBase
{
    Q_OBJECT
public:
    DewarpingView(
        QImage const& image, QImage const& downscaled_image,
        ImageTransformation const& xform);

    virtual ~DewarpingView();
private:
    DragHandler m_dragHandler;
    ZoomHandler m_zoomHandler;
};

} // namespace deskew

#endif
