// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QUrl>
#include <QtGui/QImage>

#include "StreamDeckLib_global.hpp"

namespace minervous::streamdeck
{
	class Device;

	class STREAMDECKLIB_EXPORT BaseKeyEntry: public QObject
	{
		Q_OBJECT

		using ImageType = QImage;

		Q_PROPERTY(bool down READ down NOTIFY downChanged FINAL)

		Q_PROPERTY(QUrl imageSource READ imageSource WRITE setImageSource NOTIFY imageSourceChanged FINAL)
		Q_PROPERTY(ImageType image READ image WRITE setImage NOTIFY imageChanged)

	public:
		explicit BaseKeyEntry(QObject * parent = nullptr);
		~BaseKeyEntry() override = default;

		QUrl imageSource() const;
		void setImageSource(QUrl url);

		const ImageType & image() const;
		void setImage(const ImageType & image);

		bool down() const;

	signals:
		void imageSourceChanged();
		void imageChanged();
		void downChanged();

		void pressed();
		void released();

	protected:
		void setDown(bool down, bool initializationOnly = false);
		friend class Device;

	private:
		bool _down = false;
		QUrl _imageSource;
		ImageType _image;
	};

}  // namespace minervous::streamdeck
