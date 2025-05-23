// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#ifndef SCANTAILOR_IMAGEPROC_TRANSFORM_H_
#define SCANTAILOR_IMAGEPROC_TRANSFORM_H_

#include <QColor>
#include <QSizeF>
#include <cstdint>
#include <stdexcept>

class QImage;
class QRect;
class QTransform;

namespace imageproc {
class GrayImage;

class OutsidePixels {
  // Member-wise copying is OK.
 public:
  enum Flags { COLOR = 1 << 0, NEAREST = 1 << 1, WEAK = 1 << 2 };

  /**
   * \brief Outside pixels are assumed to be of particular color.
   *
   * Outside pixels may be blended with inside pixels near the edges.
   */
  static OutsidePixels assumeColor(const QColor& color) { return OutsidePixels(COLOR, color.rgba()); }

  /**
   * \brief Outside pixels are assumed to be of particular color.
   *
   * Outside pixels won't participate in blending operations.
   */
  static OutsidePixels assumeWeakColor(const QColor& color) { return OutsidePixels(WEAK | COLOR, color.rgba()); }

  /**
   * \brief An outside pixel is assumed to be the same as the nearest inside pixel.
   *
   * Outside pixels won't participate in blending operations.
   */
  static OutsidePixels assumeWeakNearest() { return OutsidePixels(WEAK | NEAREST, 0xff000000); }

  int flags() const { return m_flags; }

  QRgb rgba() const { return m_rgba; }

  QRgb rgb() const { return m_rgba | 0xff000000; }

  uint8_t grayLevel() const { return static_cast<uint8_t>(qGray(m_rgba)); }

 private:
  OutsidePixels(int flags, QRgb rgba) : m_flags(flags), m_rgba(rgba) {}

  int m_flags;
  QRgb m_rgba;
};


/**
 * \brief Apply an affine transformation to the image.
 *
 * \param src The source image.
 * \param xform The transformation from source to destination.
 *        Only affine transformations are supported.
 * \param dstRect The area in source image coordinates to return
 *        as a destination image.
 * \param background_color Used to fill areas not represented in the source image.
 * \param weak_background If set to true, \p background_color is only taken
 *        into account if a target pixel maps to an area completely outside of
 *        the source image.  That is, if at least one source image pixel
 *        influences a particular target pixel, then any background pixels
 *        that may also influence that target pixel are ignored.\n
 *        If set to false, source image pixels and background pixels are
 *        treated equally.
 * \param minMappingArea Defines the minimum rectangle in the source image
 *        that maps to a destination pixel.  This can be used to control
 *        smoothing.
 * \return The transformed image.  It's format may differ from the
 *         source image format, for example Format_Indexed8 may
 *         be transformed to Format_RGB32, if the source image
 *         contains colors other than shades of gray.
 */
QImage transform(const QImage& src,
                 const QTransform& xform,
                 const QRect& dstRect,
                 OutsidePixels outsidePixels,
                 const QSizeF& minMappingArea = QSizeF(0.9, 0.9));

/**
 * \brief Apply an affine transformation to the image.
 *
 * Same as transform(), except the source image image is converted
 * to grayscale before transforming it.
 */
GrayImage transformToGray(const QImage& src,
                          const QTransform& xform,
                          const QRect& dstRect,
                          OutsidePixels outsidePixels,
                          const QSizeF& minMappingArea = QSizeF(0.9, 0.9));
}  // namespace imageproc
#endif  // ifndef SCANTAILOR_IMAGEPROC_TRANSFORM_H_
