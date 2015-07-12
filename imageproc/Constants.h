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

#ifndef IMAGEPROC_CONSTANTS_H_
#define IMAGEPROC_CONSTANTS_H_

#include "imageproc_config.h"

namespace imageproc
{

namespace constants
{

IMAGEPROC_EXPORT extern double const PI;

IMAGEPROC_EXPORT extern double const SQRT_2;

/**
 * angle_rad = angle_deg * RED2RAD
 */
IMAGEPROC_EXPORT extern double const DEG2RAD;

/**
 * angle_deg = angle_rad * RAD2DEG
 */
IMAGEPROC_EXPORT extern double const RAD2DEG;

/**
 * mm = inch * INCH2MM
 */
IMAGEPROC_EXPORT extern double const INCH2MM;

/**
 * inch = mm * MM2INCH
 */
IMAGEPROC_EXPORT extern double const MM2INCH;

/**
 * dots_per_meter = dots_per_inch * DPI2DPM
 */
IMAGEPROC_EXPORT extern double const DPI2DPM;

/**
 * dots_per_inch = dots_per_meter * DPM2DPI
 */
IMAGEPROC_EXPORT extern double const DPM2DPI;

} // namespace constants

} // namespace imageproc

#endif
