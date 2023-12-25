// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#include "ImageHelper.hpp"

#include <QtCore/QBuffer>

using namespace minervous::streamdeck;
using namespace Qt::Literals;

void ImageHelper::imageToPngData(const QImage & image, QByteArray & array)
{
	QBuffer buffer{&array};
	image.save(&buffer, "PNG", 100);
}

QUrl ImageHelper::pngDataToUrl(const QByteArray & array)
{
	return array.size() > 0 ? u"data:image/png;base64,%1"_s.arg(array.toBase64()) : "";
}

QUrl ImageHelper::imageToUrl(const QImage & image)
{
	QByteArray data;
	imageToPngData(image, data);
	return pngDataToUrl(data);
}
