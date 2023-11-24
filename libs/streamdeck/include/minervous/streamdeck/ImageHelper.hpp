#pragma once

#include <QtCore/QByteArray>
#include <QtCore/QUrl>
#include <QtGui/QImage>

#include "StreamDeckLib_global.hpp"

namespace minervous::streamdeck
{
	class STREAMDECKLIB_EXPORT ImageHelper
	{
	public:
		static void imageToPngData(const QImage & image, QByteArray & array);
		static QUrl pngDataToUrl(const QByteArray & array);
		static QUrl imageToUrl(const QImage & image);
	};

}  // namespace minervous::streamdeck
