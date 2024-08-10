/*
    Scan Tailor - Interactive post-processing tool for scanned pages.
    Copyright (C) 2007-2015 Joseph Artsimovich <joseph.artsimovich@gmail.com>

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

#ifndef RELATIVE_MARGINS_H_
#define RELATIVE_MARGINS_H_

#include "foundation_config.h"
#include <QRectF>
#include <QSizeF>
#include <cmath>

/**
 * RelativeMargins stores 4 margins from each side of a content box
 * as fractions of content box width. We are not using content box
 * height, as it's quite common to have partially filled pages.
 */
class FOUNDATION_EXPORT RelativeMargins
{
public:
    RelativeMargins() : m_top(), m_bottom(), m_left(), m_right(), m_base() {}

    RelativeMargins(double left, double top, double right, double bottom, double base = 0.0)
        : m_top(top), m_bottom(bottom), m_left(left), m_right(right), m_base(base) {}

    double getBaseRect(QRectF const& rect) const
    {
        double scale;
        if (m_base > 0.0)
        {
            scale = m_base;
        }
        else
        {
            double const scalex = rect.width();
            double const scaley = rect.height() * 0.7071067811865475244;
            scale = (scalex < scaley) ? scaley : scalex;
        }
        return scale;
    }

    QRectF extendContentRect(QRectF const& rect) const
    {
        double const scale = getBaseRect(rect);
        return rect.adjusted(-m_left * scale, -m_top * scale, m_right * scale, m_bottom * scale);
    }

    double getBaseSize(QSizeF const& size) const
    {
        double scale;
        if (m_base > 0.0)
        {
            scale = m_base;
        }
        else
        {
            double const scalex = size.width();
            double const scaley = size.height() * 0.7071067811865475244;
            scale = (scalex < scaley) ? scaley : scalex;
        }
        return scale;
    }

    QSizeF extendContentSize(QSizeF const& size) const
    {
        double const scale = getBaseSize(size);
        return QSizeF(
                   size.width() + (m_left + m_right) * scale,
                   size.height() + (m_top + m_bottom) * scale
               );
    }

    double top() const
    {
        return m_top;
    }

    void setTop(double val)
    {
        m_top = val;
    }

    double bottom() const
    {
        return m_bottom;
    }

    void setBottom(double val)
    {
        m_bottom = val;
    }

    double left() const
    {
        return m_left;
    }

    void setLeft(double val)
    {
        m_left = val;
    }

    double right() const
    {
        return m_right;
    }

    void setRight(double val)
    {
        m_right = val;
    }

    double getBase() const
    {
        return m_base;
    }

    void setBase(double val)
    {
        m_base = val;
    }

    bool horizontalMarginsApproxEqual() const
    {
        return std::lround(std::abs(m_right - m_left) * 100.0) == 0;
    }

    bool verticalMarginsApproxEqual() const
    {
        return std::lround(std::abs(m_top - m_bottom) * 100.0) == 0;
    }
private:
    double m_top;
    double m_bottom;
    double m_left;
    double m_right;
    double m_base;
};

#endif
