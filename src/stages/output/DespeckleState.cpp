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

#include <stdint.h>
#include <new>
#include "DespeckleState.h"
#include "DespeckleVisualization.h"
#include "Despeckle.h"
#include "TaskStatus.h"
#include "DebugImages.h"
#include "imageproc/RasterOp.h"

using namespace imageproc;

namespace output
{

DespeckleState::DespeckleState(
    QImage const& output, BinaryImage const& speckles, double factor)
    : m_speckles(speckles),
      m_despeckleFactor(factor)
{
    m_everythingMixed = overlaySpeckles(output, speckles);
    m_everythingBW = extractBW(m_everythingMixed);
}

DespeckleVisualization
DespeckleState::visualize(std::shared_ptr<AcceleratableOperations> const& accel_ops) const
{
    return DespeckleVisualization(accel_ops, m_everythingMixed, m_speckles);
}

DespeckleState
DespeckleState::redespeckle(
    double const factor,
    TaskStatus const& status, DebugImages* dbg) const
{
    DespeckleState new_state(*this);

    if (factor == m_despeckleFactor)
    {
        return new_state;
    }

    new_state.m_despeckleFactor = factor;

    if (factor > 0.0)
    {
        // Null speckles image is equivalent to a white one.
        new_state.m_speckles.release();
        return new_state;
    }
    new_state.m_speckles = Despeckle::despeckle(
                               m_everythingBW, factor, status, dbg
                           );

    status.throwIfCancelled();

    rasterOp<RopSubtract<RopSrc, RopDst> >(new_state.m_speckles, m_everythingBW);

    return new_state;
}

QImage
DespeckleState::overlaySpeckles(
    QImage const& mixed, imageproc::BinaryImage const& speckles)
{
    QImage result(mixed.convertToFormat(QImage::Format_RGB32));
    if (result.isNull() && !mixed.isNull())
    {
        throw std::bad_alloc();
    }

    if (speckles.isNull())
    {
        return result;
    }

    uint32_t* result_line = (uint32_t*)result.bits();
    int const result_stride = result.bytesPerLine() / 4;

    uint32_t const* speckles_line = speckles.data();
    int const speckles_stride = speckles.wordsPerLine();
    uint32_t const msb = uint32_t(1) << 31;

    int const width = result.width();
    int const height = result.height();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (speckles_line[x >> 5] & (msb >> (x & 31)))
            {
                result_line[x] = 0xff000000; // opaque black
            }
        }
        result_line += result_stride;
        speckles_line += speckles_stride;
    }

    return result;
}

/**
 * Here we assume that B/W content have all their color components
 * set to either 0x00 or 0xff.  We enforce this convention when
 * generating output files.
 */
BinaryImage
DespeckleState::extractBW(QImage const& mixed)
{
    BinaryImage result(mixed.size(), WHITE);

    uint32_t const* mixed_line = (uint32_t const*)mixed.bits();
    int const mixed_stride = mixed.bytesPerLine() / 4;

    uint32_t* result_line = result.data();
    int const result_stride = result.wordsPerLine();
    uint32_t const msb = uint32_t(1) << 31;

    int const width = result.width();
    int const height = result.height();

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (mixed_line[x] == 0xff000000)
            {
                result_line[x >> 5] |= msb >> (x & 31);
            }
        }
        mixed_line += mixed_stride;
        result_line += result_stride;
    }

    return result;
}

} // namespace output
