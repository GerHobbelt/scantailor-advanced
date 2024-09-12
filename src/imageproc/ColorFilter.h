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

IMAGEPROC_EXPORT void imageLevelSet(
    QImage& image,  GrayImage const y_new);
IMAGEPROC_EXPORT void imageReLevel(
    QImage& image, GrayImage const y_old,  GrayImage const y_new);

/**
 * @brief S-curve.
 *
 * (0-T)+(T-255), T = Otsu(I)
  */
IMAGEPROC_EXPORT QImage colorCurveFilter(
    QImage& image, float coef = 0.5f);

IMAGEPROC_EXPORT void colorCurveFilterInPlace(
    QImage& image, float coef = 0.5f);

/**
 * @brief C-curve.
 *
 * (0-255)
  */
IMAGEPROC_EXPORT QImage colorSqrFilter(
    QImage& image, float coef = 0.0f);

IMAGEPROC_EXPORT void colorSqrFilterInPlace(
    QImage& image, float coef = 0.0f);

/**
 * @brief Applies the Wiener filter to a color image.
 */
IMAGEPROC_EXPORT QImage wienerColorFilter(
    QImage const& image, int radius = 3, float coef = 0.0f);

IMAGEPROC_EXPORT void wienerColorFilterInPlace(
    QImage& image, int radius = 3, float coef = 0.0f);

/**
 * @brief AutoLevel
 * Ial = a1 * I + a0
 * a1 = Max / (Imax - Imin + 1), Max = 256
 * a0 = -a1 * Imin
 * Modification: The values ​​of Imin and Imax are sought not from the original image I, but from its smoothed version Ib = blur(I, r), r = 10.
 */
IMAGEPROC_EXPORT QImage autoLevelFilter(
    QImage const& image, int radius = 10, float coef = 0.0f);

IMAGEPROC_EXPORT void autoLevelFilterInPlace(
    QImage& image, int radius = 10, float coef = 0.0f);

/**
 * @brief Color Balance
 * Ib = blur(I, r), r = 23.
 * Ib2 = blur(I, r+r)
 * Io1 = overlay(invert(Ib), Ib2)
 * Io2 = overlay(invert(Ib2), Ib)
 * Ic = overlay(I, Io1)
 * Icb = overlay(Ic, Io2)
 */
IMAGEPROC_EXPORT QImage colorBalanceFilter(
    QImage const& image, int radius = 23, float coef = 0.0f);

IMAGEPROC_EXPORT void colorBalanceFilterInPlace(
    QImage& image, int radius = 23, float coef = 0.0f);

/**
 * @brief Color OverBlur
 * Ib = blur(I, r), r = 49.
 * Iob = overlay(I, invert(Ib)).
 */
IMAGEPROC_EXPORT QImage colorOverBlurFilter(
    QImage const& image, int radius = 49, float coef = 0.0f);

IMAGEPROC_EXPORT void colorOverBlurFilterInPlace(
    QImage& image, int radius = 49, float coef = 0.0f);

/**
 * @brief Color Retinex
 * Ib = blur(I, r), r = 31.
 * Ir = retinex(I, Ib) = 127.5 * (I / Ib)
 */
IMAGEPROC_EXPORT QImage colorRetinexFilter(
    QImage const& image, int radius = 31, float coef = 0.0f);

IMAGEPROC_EXPORT void colorRetinexFilterInPlace(
    QImage& image, int radius = 31, float coef = 0.0f);

/**
 * @brief Color Equalize
 * Ie = equalize(I)
 * Modification: The values ​​of Imin and Imax are sought not from the original image I, but from its smoothed version Ib = blur(I, r), r = 6.
 */
IMAGEPROC_EXPORT QImage colorEqualizeFilter(
    QImage const& image, int radius = 6, float coef = 0.0f);

IMAGEPROC_EXPORT void colorEqualizeFilterInPlace(
    QImage& image, int radius = 6, float coef = 0.0f);

/**
 * @brief Simple version KNN denoiser image.
 */
IMAGEPROC_EXPORT QImage knnDenoiserFilter(
    QImage const& image, int radius = 7, float coef = 0.0f);

IMAGEPROC_EXPORT void knnDenoiserFilterInPlace(
    QImage& image, int radius = 7, float coef = 0.0f);

/**
 * @brief Despeckle image.
 *
 * https://students.mimuw.edu.pl/~pz248275/despeckle.c
 */
IMAGEPROC_EXPORT QImage colorDespeckleFilter(
    QImage const& image, int radius = 2, float coef = 0.0f);

IMAGEPROC_EXPORT void colorDespeckleFilterInPlace(
    QImage& image, int radius = 2, float coef = 0.0f);

/**
 * @brief Sigma (blur manipulation) image.
 */
IMAGEPROC_EXPORT QImage sigmaFilter(
    QImage const& image, int radius = 29, float coef = 0.0f);

IMAGEPROC_EXPORT void sigmaFilterInPlace(
    QImage& image, int radius = 29, float coef = 0.0f);

/**
 * @brief Blur (Gauss) image.
 *
 * Ib = gaussBlur(I, r, r)
 */
IMAGEPROC_EXPORT QImage blurFilter(
    QImage const& image, int radius = 1, float coef = 0.0f);

IMAGEPROC_EXPORT void blurFilterInPlace(
    QImage& image, int radius = 1, float coef = 0.0f);

/**
 * @brief Applies the Screen filter to a image.
 *
 * @param image The image to apply the filter to. A null image is allowed.
 * @param window_size The local neighbourhood around a pixel to use.
 * @param coef The part of filter in the result.
 * @return The filtered image.
 */
IMAGEPROC_EXPORT QImage screenFilter(
    QImage const& image, int radius = 5, float coef = 0.0f);

IMAGEPROC_EXPORT void screenFilterInPlace(
    QImage& image, int radius = 5, float coef = 0.0f);

/**
 * @brief EdgeDiv (EdgePlus+BlurDiv) filter image.
 */
IMAGEPROC_EXPORT void edgedivFilterInPlace(
    QImage& image, int radius = 13, float coef = 0.0f);

IMAGEPROC_EXPORT QImage edgedivFilter(
    QImage& image, int radius = 13, float coef = 0.0f);

/**
 * @brief Robust filter image.
 */
IMAGEPROC_EXPORT void robustFilterInPlace(
    QImage& image, int radius = 10, float coef = 0.0f);

IMAGEPROC_EXPORT QImage robustFilter(
    QImage& image, int radius = 10, float coef = 0.0f);

/**
 * @brief Add engraving texture to image.
 */
IMAGEPROC_EXPORT void gravureFilterInPlace(
    QImage& image, int radius = 15, float coef = 0.0f);

IMAGEPROC_EXPORT QImage gravureFilter(
    QImage& image, int radius = 15, float coef = 0.0f);

/**
 * @brief Add dots8x8 texture to image.
 *
 * Modification: The threshold values are sought not from the original image I, but from its smoothed version Ib = blur(I, r), r = 17.
 */
IMAGEPROC_EXPORT void dots8FilterInPlace(
    QImage& image, int radius = 17, float coef = 0.0f);

IMAGEPROC_EXPORT QImage dots8Filter(
    QImage& image, int radius = 17, float coef = 0.0f);

/**
 * @brief Remove BG.
 *
 * Irbg = ((I - BG) < T) ? BG : (I - BG), BG = mean(I)
 */
IMAGEPROC_EXPORT QImage unPaperFilter(
    QImage const& image, unsigned int iters = 4, float coef = 0.0f);

IMAGEPROC_EXPORT void unPaperFilterInPlace(
    QImage& image, unsigned int iters = 4, float coef = 0.0f);

/**
 * @brief Highlighting and masking colors in image.
 */
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
    double* mean_h, double* mean_s,
    int const start, int const stop);

static void paletteHSVsaturation(
    double* mean_s, float const coef_sat,
    int const start, int const stop);

static void paletteYCbCrsaturation(
    double* mean_cb, double* mean_cr, float const coef_sat,
    int const start, int const stop);

static void paletteHSVnorm(
    double* mean_v, float const coef_norm,
    int const start, int const stop);

static void paletteHSVtoRGB(
    double* mean_h, double* mean_s, double* mean_v, int const ncount);

static void paletteHSLtoRGB(
    double* mean_h, double* mean_s, double* mean_l, int const ncount);

static void paletteYCbCrtoRGB(
    double* mean_cb, double* mean_cr, double* mean_cy, int const ncount);

IMAGEPROC_EXPORT void hsvKMeansInPlace(
    QImage& dst,
    QImage const& image,
    BinaryImage const& mask,
    BinaryImage const& mask_zones,
    int const ncount,
    int start_value = 0,
    int color_space = 0,
    float coef_sat = 0.0f,
    float coef_norm = 0.0f,
    float coef_bg = 0.0f);

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

} // namespace imageproc

#endif
