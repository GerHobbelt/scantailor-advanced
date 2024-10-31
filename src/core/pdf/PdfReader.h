/*
    Scan Tailor - Interactive post-processing tool for scanned pages.
    Copyright (C) 2007-2008  Joseph Artsimovich <joseph_a@mail.ru>
		Copyright (C) 2017 Daniel Just
		
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

#ifndef PDFREADER_H_
#define PDFREADER_H_

#include "../ImageMetadataLoader.h"
#include "VirtualFunction.h"
#include "podofo/podofo.h"

#include <unordered_map>

using namespace PoDoFo;

class QFile;
class QImage;
class ImageMetadata;

/**
* \brief Extracts Images from a pdf file.
*
* Only images larger than 1000x1000 pixels and
* only the largest image on a page are extracted.
*/
class PdfReader
{
public:
	/**
	* \brief Searches for the largest image dimensions in the pdf.
	*
	* \param device The device to read from.  This device must be
	*        opened for reading and must be seekable.
	* \return LOADED, if an image larger than 1000 x 1000 px was found.
	*/
	static ImageMetadataLoader::Status readMetadata(QFile& file,
		VirtualFunction1<void, ImageMetadata const&>& out);
	
	/**
	* \brief Checks, if io device seems like a pdf file.
	*
	* \param device The device to read from.  This device must be
	*        opened for reading and must be seekable.
	* \return true, if file seems like a valid pdf file, false otherwise.
	*/
	static bool seemsLikePdf(QFile& file);

	/**
	* \brief Reads the image from io device to QImage.
	*
	* \param device The device to read from.  This device must be
	*        opened for reading and must be seekable.
	* \param page_num A zero-based page number within a multi-page
	*        Pdf file.
	* \return The resulting image, or a null image in case of failure.
	*/
	static QImage readImage(QFile& file, int page_num = 0);
};

#endif
