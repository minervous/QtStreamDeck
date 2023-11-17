#pragma once

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtCore/QPointer>
#include <QtGui/QImage>
#include "StreamDeckLib_global.hpp"

namespace minervous::streamdeck
{
	class KeyModel;

	class STREAMDECKLIB_EXPORT BaseKeyEntry: public QObject
	{
		Q_OBJECT

		using ImageType = QImage;

		Q_PROPERTY(bool pressed READ pressed NOTIFY pressedChanged FINAL)

		Q_PROPERTY(QUrl imageSource READ imageSource WRITE setImageSource NOTIFY imageSourceChanged FINAL)
		Q_PROPERTY(ImageType image READ image WRITE setImage NOTIFY imageChanged FINAL)

	public:
		explicit BaseKeyEntry(QObject * parent = nullptr);
		~BaseKeyEntry() override;

		QUrl imageSource() const;
		ImageType image() const;
		bool pressed() const;

		void setImageSource(QUrl url);
		void setImage(ImageType image);

	signals:
		void imageSourceChanged();
		void imageChanged();
		void pressedChanged();

		void keyPressed();
		void keyReleased();

	protected:
		void setPressed(bool pressed);
		friend class KeyModel;

	private:

		bool _pressed = false;
		QUrl _imageSource;
		ImageType _image;
	};

}  // namespace minervous::streamdeck

