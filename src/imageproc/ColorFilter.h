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

#ifndef IMAGEPROC_COLOR_FILTER_H_
#define IMAGEPROC_COLOR_FILTER_H_

#include "imageproc_config.h"
#include <QSize>

class QImage;

namespace imageproc
{

class GrayImage;

/**
 * @brief Applies the Wiener filter to a grayscale image.
 *
 * @param image The image to apply the filter to. A null image is allowed.
 * @param window_size The local neighbourhood around a pixel to use.
 * @param noise_sigma The standard deviation of noise in the image.
 * @return The filtered image.
 */
IMAGEPROC_EXPORT GrayImage wienerFilter(
    GrayImage const& image, QSize const& window_size, float noise_sigma);

/**
 * @brief An in-place version of wienerFilter().
 * @see wienerFilter()
 */
IMAGEPROC_EXPORT void wienerFilterInPlace(
    GrayImage& image, QSize const& window_size, float noise_sigma);

IMAGEPROC_EXPORT QImage wienerColorFilter(
    QImage const& image, QSize const& window_size, float coef = 0.0f);

IMAGEPROC_EXPORT void wienerColorFilterInPlace(
    QImage& image, QSize const& window_size, float coef = 0.0f);

IMAGEPROC_EXPORT QImage knnDenoiserFilter(
    QImage const& image, int radius = 1, float coef = 0.0f);

IMAGEPROC_EXPORT void knnDenoiserFilterInPlace(
    QImage& image, int radius = 1, float coef = 0.0f);

IMAGEPROC_EXPORT QImage colorDespeckleFilter(
    QImage const& image, int radius = 1, float coef = 0.0f);

IMAGEPROC_EXPORT void colorDespeckleFilterInPlace(
    QImage& image, int radius = 1, float coef = 0.0f);

IMAGEPROC_EXPORT QImage blurFilter(
    QImage const& image, QSize const& window_size, float coef = 0.0f);

IMAGEPROC_EXPORT void blurFilterInPlace(
    QImage& image, QSize const& window_size, float coef = 0.0f);

/**
 * @brief Applies the Screen filter to a image.
 *
 * @param image The image to apply the filter to. A null image is allowed.
 * @param window_size The local neighbourhood around a pixel to use.
 * @param coef The part of filter in the result.
 * @return The filtered image.
 */
IMAGEPROC_EXPORT QImage screenFilter(
    QImage const& image, QSize const& window_size, float coef = 0.0f);

/**
 * @brief An in-place version of screenFilter().
 * @see screenFilter()
 */
IMAGEPROC_EXPORT void screenFilterInPlace(
    QImage& image, QSize const& window_size, float coef = 0.0f);

IMAGEPROC_EXPORT QImage colorCurveFilter(
    QImage& image, float coef = 0.5f);

IMAGEPROC_EXPORT void colorCurveFilterInPlace(
    QImage& image, float coef = 0.5f);

IMAGEPROC_EXPORT QImage colorSqrFilter(
    QImage& image, float coef = 0.0f);

IMAGEPROC_EXPORT void colorSqrFilterInPlace(
    QImage& image, float coef = 0.0f);

IMAGEPROC_EXPORT GrayImage coloredSignificanceFilter(
    QImage const& image, float coef = 0.0f);

IMAGEPROC_EXPORT void coloredSignificanceFilterInPlace(
    QImage const& image, GrayImage& gray, float coef = 0.0f);

IMAGEPROC_EXPORT QImage coloredDimmingFilter(
    QImage& image, GrayImage& gray);

IMAGEPROC_EXPORT void coloredDimmingFilterInPlace(
    QImage& image, GrayImage& gray);

IMAGEPROC_EXPORT void coloredMaskInPlace(
    QImage& image, BinaryImage content, BinaryImage mask);

static QImage imageHSVcylinder(QImage const& image);

static QImage imageHSLcylinder(QImage const& image);

static QImage imageYCbCr(QImage const& image);

static float pixelDistance(
    float const h0, float const s0, float const v0,
    float const h1, float const s1, float const v1);

static void paletteHSVcylinderGenerate(
    double* mean_h0, double* mean_s0, double* mean_v0,
    int const ncount, float start_value = 128.0f);

static void paletteHSVcylinderToHSV(
    double* mean_h, double* mean_s, int const ncount);

static void paletteHSVsaturation(
    double* mean_s, float const coef_sat, int const ncount);

static void paletteYCbCrsaturation(
    double* mean_cb, double* mean_cr,
    float const coef_sat, int const ncount);

static void paletteHSVnorm(
    double* mean_v, float const coef_norm, int const ncount);

static void paletteHSVtoRGB(
    double* mean_h, double* mean_s, double* mean_v, int const ncount);

static void paletteHSLtoRGB(
    double* mean_h, double* mean_s, double* mean_l, int const ncount);

static void paletteYCbCrtoRGB(
    double* mean_cb, double* mean_cr, double* mean_cy, int const ncount);

IMAGEPROC_EXPORT void hsvKMeansInPlace(
    QImage& dst, QImage const& image, BinaryImage const& mask,
    int const ncount, int start_value = 0, int color_space = 0,
    float coef_sat = 0.0f, float coef_norm = 0.0f, float coef_bg = 0.0f);

IMAGEPROC_EXPORT void maskMorphologicalErode(
    QImage& image, BinaryImage const& mask, int radius = 0);

IMAGEPROC_EXPORT void maskMorphologicalDilate(
    QImage& image, BinaryImage const& mask, int radius = 0);

IMAGEPROC_EXPORT void maskMorphologicalOpen(
    QImage& image, BinaryImage const& mask, int radius = 0);

IMAGEPROC_EXPORT void maskMorphologicalClose(
    QImage& image, BinaryImage const& mask, int radius = 0);

IMAGEPROC_EXPORT void maskMorphological(
    QImage& image, BinaryImage const& mask, int radius = 0);

IMAGEPROC_EXPORT QImage unPaperFilter(
    QImage const& image, unsigned int const iters, float coef = 0.0f);

IMAGEPROC_EXPORT void unPaperFilterInPlace(
    QImage& image, unsigned int const iters, float coef = 0.0f);

IMAGEPROC_EXPORT void gravureFilterInPlace(
    QImage& image, QSize const& window_size, float coef = 0.0f);

IMAGEPROC_EXPORT QImage gravureFilter(
    QImage& image, float coef = 0.0f);

} // namespace imageproc

#endif
