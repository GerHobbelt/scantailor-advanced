// Copyright (C) 2019  Joseph Artsimovich <joseph.artsimovich@gmail.com>, 4lex4 <4lex49@zoho.com>
// Use of this source code is governed by the GNU GPLv3 license that can be found in the LICENSE file.

#include "TiffWriter.h"

#include <Constants.h>
#include <Grayscale.h>
#include <tiffio.h>

#include <QDebug>
#include <QtCore/QFile>
#include <cassert>
#include <cmath>

#include "ApplicationSettings.h"
#include "Dpm.h"

/**
 * m_reverseBitsLUT[byte] gives the same byte, but with bit order reversed.
 */
const uint8_t TiffWriter::m_reverseBitsLUT[256]
    = {0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0, 0x08, 0x88, 0x48,
       0xc8, 0x28, 0xa8, 0x68, 0xe8, 0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8, 0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4,
       0x64, 0xe4, 0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4, 0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec, 0x1c,
       0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc, 0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2, 0x12, 0x92, 0x52, 0xd2,
       0x32, 0xb2, 0x72, 0xf2, 0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea, 0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a,
       0xfa, 0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6, 0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6, 0x0e, 0x8e,
       0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee, 0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe, 0x01, 0x81, 0x41, 0xc1, 0x21,
       0xa1, 0x61, 0xe1, 0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1, 0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9,
       0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9, 0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5, 0x15, 0x95, 0x55,
       0xd5, 0x35, 0xb5, 0x75, 0xf5, 0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed, 0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd,
       0x7d, 0xfd, 0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3, 0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3, 0x0b,
       0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb, 0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb, 0x07, 0x87, 0x47, 0xc7,
       0x27, 0xa7, 0x67, 0xe7, 0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7, 0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f,
       0xef, 0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff};


class TiffWriter::TiffHandle {
 public:
  explicit TiffHandle(TIFF* handle) : m_handle(handle) {}

  ~TiffHandle() {
    if (m_handle) {
      TIFFClose(m_handle);
    }
  }

  TIFF* handle() const { return m_handle; }

 private:
  TIFF* m_handle;
};


static tsize_t deviceRead(thandle_t context, tdata_t data, tsize_t size) {
  // Not implemented.
  return 0;
}

static tsize_t deviceWrite(thandle_t context, tdata_t data, tsize_t size) {
  auto* dev = (QIODevice*) context;
  return (tsize_t) dev->write(static_cast<char*>(data), size);
}

static toff_t deviceSeek(thandle_t context, toff_t offset, int whence) {
  auto* dev = (QIODevice*) context;

  switch (whence) {
    case SEEK_SET:
      dev->seek(offset);
      break;
    case SEEK_CUR:
      dev->seek(dev->pos() + offset);
      break;
    case SEEK_END:
      dev->seek(dev->size() + offset);
      break;
    default:
      break;
  }
  return dev->pos();
}

static int deviceClose(thandle_t context) {
  auto* dev = (QIODevice*) context;
  dev->close();
  return 0;
}

static toff_t deviceSize(thandle_t context) {
  auto* dev = (QIODevice*) context;
  return dev->size();
}

static int deviceMap(thandle_t, tdata_t*, toff_t*) {
  // Not implemented.
  return 0;
}

static void deviceUnmap(thandle_t, tdata_t, toff_t) {
  // Not implemented.
}

bool TiffWriter::writeImage(const QString& filePath, const QImage& image) {
  if (image.isNull()) {
    return false;
  }

  QFile file(filePath);
  if (!file.open(QFile::WriteOnly)) {
    return false;
  }

  if (!writeImage(file, image)) {
    file.remove();
    return false;
  }
  return true;
}

bool TiffWriter::writeImage(QIODevice& device, const QImage& image) {
  if (image.isNull()) {
    return false;
  }
  if (!device.isWritable()) {
    return false;
  }
  if (device.isSequential()) {
    // libtiff needs to be able to seek.
    return false;
  }

  TiffHandle tif(TIFFClientOpen(
      // Libtiff seems to be buggy with L or H flags,
      // so we use B.
      "file", "wBm", &device, &deviceRead, &deviceWrite, &deviceSeek, &deviceClose, &deviceSize, &deviceMap,
      &deviceUnmap));
  if (!tif.handle()) {
    return false;
  }

  TIFFSetField(tif.handle(), TIFFTAG_IMAGEWIDTH, uint32_t(image.width()));
  TIFFSetField(tif.handle(), TIFFTAG_IMAGELENGTH, uint32_t(image.height()));
  TIFFSetField(tif.handle(), TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
  TIFFSetField(tif.handle(), TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
  setDpm(tif, Dpm(image));

  switch (image.format()) {
    case QImage::Format_Mono:
    case QImage::Format_MonoLSB:
    case QImage::Format_Indexed8:
      return writeBitonalOrIndexed8Image(tif, image);
    default:;
  }
  if (image.hasAlphaChannel()) {
    return writeARGB32Image(tif, image.convertToFormat(QImage::Format_ARGB32));
  } else {
    return writeRGB32Image(tif, image.convertToFormat(QImage::Format_RGB32));
  }
}  // TiffWriter::writeImage

/**
 * Set the physical resolution, if it's defined.
 */
void TiffWriter::setDpm(const TiffHandle& tif, const Dpm& dpm) {
  using namespace constants;

  if (dpm.isNull()) {
    return;
  }

  auto xres = 0.01 * dpm.horizontal();  // cm
  auto yres = 0.01 * dpm.vertical();    // cm
  uint16_t unit = RESUNIT_CENTIMETER;

  // If we have a round (or almost round) DPI, then
  // write it as DPI rather than dots per cm.
  const double xdpi = dpm.horizontal() * DPM2DPI;
  const double ydpi = dpm.vertical() * DPM2DPI;
  const double roundedXdpi = std::floor(xdpi + 0.5);
  const double roundedYdpi = std::floor(ydpi + 0.5);
  if ((std::fabs(xdpi - roundedXdpi) < 0.02) && (std::fabs(ydpi - roundedYdpi) < 0.02)) {
    xres = roundedXdpi;
    yres = roundedYdpi;
    unit = RESUNIT_INCH;
  }

  TIFFSetField(tif.handle(), TIFFTAG_XRESOLUTION, xres);
  TIFFSetField(tif.handle(), TIFFTAG_YRESOLUTION, yres);
  TIFFSetField(tif.handle(), TIFFTAG_RESOLUTIONUNIT, unit);
}

bool TiffWriter::writeBitonalOrIndexed8Image(const TiffHandle& tif, const QImage& image) {
  TIFFSetField(tif.handle(), TIFFTAG_SAMPLESPERPIXEL, uint16_t(1));

  uint16_t bitsPerSample = 8;
  uint16_t photometric = PHOTOMETRIC_PALETTE;
  if (image.isGrayscale()) {
    photometric = PHOTOMETRIC_MINISBLACK;
  }

  switch (image.format()) {
    case QImage::Format_Mono:
    case QImage::Format_MonoLSB:
      bitsPerSample = 1;
      if (image.colorCount() < 2) {
        photometric = PHOTOMETRIC_MINISWHITE;
      } else {
        // Some programs don't understand
        // palettized binary images, so don't
        // use a palette for black and white images.
        const uint32_t c0 = image.color(0);
        const uint32_t c1 = image.color(1);
        if ((c0 == 0xffffffff) && (c1 == 0xff000000)) {
          photometric = PHOTOMETRIC_MINISWHITE;
        } else if ((c0 == 0xff000000) && (c1 == 0xffffffff)) {
          photometric = PHOTOMETRIC_MINISBLACK;
        }
      }
      break;
    default:;
  }

  if (image.format() == QImage::Format_Indexed8) {
    uint16 compress = (photometric == PHOTOMETRIC_PALETTE) ?
                      COMPRESSION_LZW :
                 uint16_t(ApplicationSettings::getInstance().getTiffColorCompression()));
    TIFFSetField(tif.handle(), TIFFTAG_COMPRESSION, compress);
  } else {
    TIFFSetField(tif.handle(), TIFFTAG_COMPRESSION,
                 uint16_t(ApplicationSettings::getInstance().getTiffBwCompression()));
  }

  TIFFSetField(tif.handle(), TIFFTAG_BITSPERSAMPLE, bitsPerSample);
  TIFFSetField(tif.handle(), TIFFTAG_PHOTOMETRIC, photometric);

  if (photometric == PHOTOMETRIC_PALETTE) {
    const int numColors = 1 << bitsPerSample;
    QVector<QRgb> colorTable(image.colorTable());
    if (colorTable.size() > numColors) {
      colorTable.resize(numColors);
    }
    std::vector<uint16_t> pr(numColors, 0);
    std::vector<uint16_t> pg(numColors, 0);
    std::vector<uint16_t> pb(numColors, 0);
    for (int i = 0; i < colorTable.size(); ++i) {
      const QRgb rgb = colorTable[i];
      pr[i] = static_cast<unsigned short>((0xFFFF * qRed(rgb) + 128) / 255);
      pg[i] = static_cast<unsigned short>((0xFFFF * qGreen(rgb) + 128) / 255);
      pb[i] = static_cast<unsigned short>((0xFFFF * qBlue(rgb) + 128) / 255);
    }
    TIFFSetField(tif.handle(), TIFFTAG_COLORMAP, &pr[0], &pg[0], &pb[0]);
  }

  if (image.format() == QImage::Format_Indexed8) {
    return write8bitLines(tif, image);
  } else {
    if (image.format() == QImage::Format_MonoLSB) {
      return writeBinaryLinesReversed(tif, image);
    } else {
      return writeBinaryLinesAsIs(tif, image);
    }
  }
}  // TiffWriter::writeBitonalOrIndexed8Image

bool TiffWriter::writeRGB32Image(const TiffHandle& tif, const QImage& image) {
  assert(image.format() == QImage::Format_RGB32);

  TIFFSetField(tif.handle(), TIFFTAG_SAMPLESPERPIXEL, uint16_t(3));
  TIFFSetField(tif.handle(), TIFFTAG_COMPRESSION,
               uint16_t(ApplicationSettings::getInstance().getTiffColorCompression()));
  TIFFSetField(tif.handle(), TIFFTAG_BITSPERSAMPLE, uint16_t(8));
  TIFFSetField(tif.handle(), TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

  const int width = image.width();
  const int height = image.height();

  std::vector<uint8_t> tmpLine(width * 3);

  // Libtiff expects "RR GG BB" sequences regardless of CPU byte order.

  for (int y = 0; y < height; ++y) {
    const auto* pSrc = (const uint32_t*) image.scanLine(y);
    uint8_t* pDst = &tmpLine[0];
    for (int x = 0; x < width; ++x) {
      const uint32_t ARGB = *pSrc;
      pDst[0] = static_cast<uint8_t>(ARGB >> 16);
      pDst[1] = static_cast<uint8_t>(ARGB >> 8);
      pDst[2] = static_cast<uint8_t>(ARGB);
      ++pSrc;
      pDst += 3;
    }
    if (TIFFWriteScanline(tif.handle(), &tmpLine[0], y) == -1) {
      return false;
    }
  }
  return true;
}  // TiffWriter::writeRGB32Image

bool TiffWriter::writeARGB32Image(const TiffHandle& tif, const QImage& image) {
  assert(image.format() == QImage::Format_ARGB32);

  TIFFSetField(tif.handle(), TIFFTAG_SAMPLESPERPIXEL, uint16_t(4));
  TIFFSetField(tif.handle(), TIFFTAG_COMPRESSION,
               uint16_t(ApplicationSettings::getInstance().getTiffColorCompression()));
  TIFFSetField(tif.handle(), TIFFTAG_BITSPERSAMPLE, uint16_t(8));
  TIFFSetField(tif.handle(), TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

  const int width = image.width();
  const int height = image.height();

  std::vector<uint8_t> tmpLine(width * 4);

  // Libtiff expects "RR GG BB AA" sequences regardless of CPU byte order.

  for (int y = 0; y < height; ++y) {
    const auto* pSrc = (const uint32_t*) image.scanLine(y);
    uint8_t* pDst = &tmpLine[0];
    for (int x = 0; x < width; ++x) {
      const uint32_t ARGB = *pSrc;
      pDst[0] = static_cast<uint8_t>(ARGB >> 16);
      pDst[1] = static_cast<uint8_t>(ARGB >> 8);
      pDst[2] = static_cast<uint8_t>(ARGB);
      pDst[3] = static_cast<uint8_t>(ARGB >> 24);
      ++pSrc;
      pDst += 4;
    }
    if (TIFFWriteScanline(tif.handle(), &tmpLine[0], y) == -1) {
      return false;
    }
  }
  return true;
}  // TiffWriter::writeARGB32Image

bool TiffWriter::write8bitLines(const TiffHandle& tif, const QImage& image) {
  const int width = image.width();
  const int height = image.height();

  // TIFFWriteScanline() can actually modify the data you pass it,
  // so we have to use a temporary buffer even when no coversion
  // is required.
  std::vector<uint8_t> tmpLine(width, 0);

  for (int y = 0; y < height; ++y) {
    const uint8_t* srcLine = image.scanLine(y);
    memcpy(&tmpLine[0], srcLine, tmpLine.size());
    if (TIFFWriteScanline(tif.handle(), &tmpLine[0], y) == -1) {
      return false;
    }
  }
  return true;
}

bool TiffWriter::writeBinaryLinesAsIs(const TiffHandle& tif, const QImage& image) {
  const int width = image.width();
  const int height = image.height();
  // TIFFWriteScanline() can actually modify the data you pass it,
  // so we have to use a temporary buffer even when no coversion
  // is required.
  const int bpl = (width + 7) / 8;
  std::vector<uint8_t> tmpLine(bpl, 0);

  for (int y = 0; y < height; ++y) {
    const uint8_t* srcLine = image.scanLine(y);
    memcpy(&tmpLine[0], srcLine, bpl);
    if (TIFFWriteScanline(tif.handle(), &tmpLine[0], y) == -1) {
      return false;
    }
  }
  return true;
}

bool TiffWriter::writeBinaryLinesReversed(const TiffHandle& tif, const QImage& image) {
  const int width = image.width();
  const int height = image.height();

  const int bpl = (width + 7) / 8;
  std::vector<uint8_t> tmpLine(bpl, 0);

  for (int y = 0; y < height; ++y) {
    const uint8_t* srcLine = image.scanLine(y);
    for (int i = 0; i < bpl; ++i) {
      tmpLine[i] = m_reverseBitsLUT[srcLine[i]];
    }
    if (TIFFWriteScanline(tif.handle(), &tmpLine[0], y) == -1) {
      return false;
    }
  }
  return true;
}
