/*******************************************************************************************************
 DkMetaData.h
 Created on:	19.04.2013
 
 nomacs is a fast and small image viewer with the capability of synchronizing multiple instances
 
 Copyright (C) 2011-2013 Markus Diem <markus@nomacs.org>
 Copyright (C) 2011-2013 Stefan Fiel <stefan@nomacs.org>
 Copyright (C) 2011-2013 Florian Kleber <florian@nomacs.org>

 This file is part of nomacs.

 nomacs is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 nomacs is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 *******************************************************************************************************/

#pragma once

#pragma warning(push, 0)	// no warnings from includes - begin
#include <QSharedPointer>
#include <QStringList>
#include <QMap>

//code for metadata crop:
#include "DkMath.h"

#ifdef HAVE_EXIV2_HPP
#include <exiv2/exiv2.hpp>
#else
#include <exiv2/xmpsidecar.hpp>
#include <exiv2/image.hpp>
#include <exiv2/preview.hpp>
#include <iomanip>
#endif
#pragma warning(pop)

#pragma warning(disable: 4251)	// TODO: remove

#ifndef DllCoreExport
#ifdef DK_CORE_DLL_EXPORT
#define DllCoreExport Q_DECL_EXPORT
#elif DK_DLL_IMPORT
#define DllCoreExport Q_DECL_IMPORT
#else
#define DllCoreExport Q_DECL_IMPORT
#endif
#endif

// OS 2 does not define byte so we safely assume, that other programmers agree to call an 8 bit a byte
#ifndef byte
typedef unsigned char byte;
#endif

// Qt defines
class QVector2D;
class QImage;

namespace nmc {

class DllCoreExport DkMetaDataT {

public:
	DkMetaDataT();


	enum ExifOrientationState {
		or_illegal = -1,
		or_not_set,
		or_valid,
	};

	void readMetaData(const QString& filePath, QSharedPointer<QByteArray> ba = QSharedPointer<QByteArray>());
	bool saveMetaData(const QString& filePath, bool force = false);
	bool saveMetaData(QSharedPointer<QByteArray>& ba, bool force = false);

	int getOrientationDegree() const;
	ExifOrientationState checkExifOrientation() const;
	int getRating() const;
	QSize getImageSize() const;
	QString getDescription() const;
	QVector2D getResolution() const;
	QString getNativeExifValue(const QString& key) const;
	QString getXmpValue(const QString& key) const;
	QString getExifValue(const QString& key) const;
	QString getIptcValue(const QString& key) const;
	QString getQtValue(const QString& key) const;
	QImage getThumbnail() const;
	QImage getPreviewImage(int minPreviewWidth = 0) const;
	QStringList getExifKeys() const;
	QStringList getExifValues() const;
	QStringList getIptcKeys() const;
	QStringList getQtKeys() const;
	QStringList getQtValues() const;
	QStringList getIptcValues() const;
	QStringList getXmpKeys() const;

	void getFileMetaData(QStringList& fileKeys, QStringList& fileValues) const;
	void getAllMetaData(QStringList& keys, QStringList& values) const;
	void setResolution(const QVector2D& res);
	void clearOrientation();
	void setOrientation(int o);
	void setRating(int r);
	bool setDescription(const QString& description);
	bool setExifValue(QString key, QString taginfo);
	bool updateImageMetaData(const QImage& img);
	void setThumbnail(QImage thumb);
	void setQtValues(const QImage& cImg);
	static QString exiv2ToQString(std::string exifString);
	void setUseSidecar(bool useSideCar = false);

	bool hasMetaData() const;
	bool isLoaded() const;
	bool isTiff() const;
	bool isJpg() const;
	bool isRaw() const;
	bool isDirty() const;
	bool useSidecar() const;
	void printMetaData() const; //only for debug

	//code for metadata crop:
	bool saveRectToXMP(const DkRotatingRect& rect, const QSize& imgSize);
	bool clearXMPRect();
	DkRotatingRect getXMPRect(const QSize& size) const;
	bool setXMPValue(Exiv2::XmpData& xmpData, QString xmpKey, QString xmpValue);

protected:
	Exiv2::Image::AutoPtr loadSidecar(const QString& filePath) const;

	enum {
		not_loaded,
		no_data,
		loaded,
		dirty,
	};

	Exiv2::Image::AutoPtr mExifImg;
	QString mFilePath;
	QStringList mQtKeys;
	QStringList mQtValues;

	int mExifState = not_loaded;
	bool mUseSidecar = false;
};

class DllCoreExport DkMetaDataHelper {

public:

	static DkMetaDataHelper& getInstance() {

		static DkMetaDataHelper instance;

		return instance;
	}

	QString getApertureValue(QSharedPointer<DkMetaDataT> metaData) const;
	QString getFocalLength(QSharedPointer<DkMetaDataT> metaData) const;
	QString getExposureTime(QSharedPointer<DkMetaDataT> metaData) const;
	QString getExposureMode(QSharedPointer<DkMetaDataT> metaData) const;
	QString getFlashMode(QSharedPointer<DkMetaDataT> metaData) const;
	QString getGpsCoordinates(QSharedPointer<DkMetaDataT> metaData) const;
	QString getGpsAltitude(const QString& val) const;
	QStringList convertGpsCoordinates(const QString& coordString) const;
	float convertRational(const QString& val) const;
	bool hasGPS(QSharedPointer<DkMetaDataT> metaData) const;
	QString translateKey(const QString& key) const;
	QString resolveSpecialValue(QSharedPointer<DkMetaDataT> metaData, const QString& key, const QString& value) const;

	QStringList getCamSearchTags() const;
	QStringList getDescSearchTags() const;
	QStringList getTranslatedCamTags() const;
	QStringList getTranslatedDescTags() const;
	QStringList getAllExposureModes() const;
	QMap<int, QString> getAllFlashModes() const;

protected:
	DkMetaDataHelper() { init(); };
	DkMetaDataHelper(DkMetaDataHelper const&);		// hide
	void operator=(DkMetaDataHelper const&);		// hide
	void init();

	QStringList mCamSearchTags;
	QStringList mDescSearchTags;

	QStringList mTranslatedCamTags;
	QStringList mTranslatedDescTags;

	QStringList mExposureModes;
	QMap<int, QString> mFlashModes;
};

}
