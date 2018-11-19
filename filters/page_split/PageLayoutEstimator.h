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

#ifndef PAGE_SPLIT_PAGELAYOUTESTIMATOR_H_
#define PAGE_SPLIT_PAGELAYOUTESTIMATOR_H_

#include "LayoutType.h"
#include "foundation/VirtualFunction.h"
#include "acceleration/AcceleratableOperations.h"
#include <QLineF>
#include <deque>
#include <memory>

class QRect;
class QPoint;
class QImage;
class DebugImages;
class Span;

namespace imageproc
{
	class AffineTransformedImage;
}

namespace imageproc
{
	class BinaryImage;
	class BinaryThreshold;
}

namespace page_split
{

class PageLayout;

class PageLayoutEstimator
{
public:
	/**
	 * \brief Estimates the page layout according to the provided layout type.
	 *
	 * \param layout_type The type of a layout to detect.  If set to
	 *        something other than Rule::AUTO_DETECT, the returned
	 *        layout will have the same type.
	 * \param image The input image plus a linear transformation.
	 * \param bw_threshold The global binarization threshold for the
	 *        input image.
	 * \param dbg An optional sink for debugging images.
	 * \return The estimated PageLayout of type consistent with the
	 *         requested layout type.
	 */
	static PageLayout estimatePageLayout(
		LayoutType layout_type, imageproc::AffineTransformedImage const& image,
		std::shared_ptr<AcceleratableOperations> const& accel_ops,
		DebugImages* dbg = nullptr);
private:
	static std::unique_ptr<PageLayout> tryCutAtFoldingLine(
		LayoutType layout_type, imageproc::AffineTransformedImage const& image,
		std::shared_ptr<AcceleratableOperations> const& accel_ops,
		DebugImages* dbg);
		
	static PageLayout cutAtWhitespace(
		LayoutType layout_type, imageproc::AffineTransformedImage const& image,
		std::shared_ptr<AcceleratableOperations> const& accel_ops,
		DebugImages* dbg);
	
	static PageLayout cutAtWhitespaceDeskewed150(
		LayoutType layout_type, int num_pages,
		imageproc::BinaryImage const& image,
		bool left_offcut, bool right_offcut, DebugImages* dbg);
	
	static imageproc::BinaryImage binarize(
		imageproc::AffineTransformedImage const& image,
		std::shared_ptr<AcceleratableOperations> const& accel_ops);
	
	static imageproc::BinaryImage removeGarbageAnd2xDownscale(
		imageproc::BinaryImage const& image, DebugImages* dbg);
	
	static bool checkForLeftOffcut(imageproc::BinaryImage const& image);
	
	static bool checkForRightOffcut(imageproc::BinaryImage const& image);
	
	static void visualizeSpans(
		DebugImages& dbg, std::deque<Span> const& spans,
		imageproc::BinaryImage const& image, char const* label);
	
	static void removeInsignificantEdgeSpans(std::deque<Span>& spans);
	
	static PageLayout processContentSpansSinglePage(
		LayoutType layout_type,
		std::deque<Span> const& spans,
		int width, int height,
		bool left_offcut, bool right_offcut);
		
	static PageLayout processContentSpansTwoPages(
		LayoutType layout_type,
		std::deque<Span> const& spans,
		int width, int height);
	
	static PageLayout processTwoPagesWithSingleSpan(
		Span const& span, int width, int height);
	
	static QLineF vertLine(double x);
};

} // namespace page_split

#endif
