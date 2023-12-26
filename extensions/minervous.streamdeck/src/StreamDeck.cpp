// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#include "StreamDeck.hpp"

#include "minervous/streamdeck/StreamDeckLogging.hpp"

using namespace minervous::streamdeck::qml;

StreamDeck::StreamDeck(QObject * parent)
	: Base{parent}
	, _defaultData{new KeyModel{this}}
{
	connect(_defaultData, &KeyModel::qmlDataChanged, this, &StreamDeck::qmlDataChanged);
}

void StreamDeck::sendImage(int index, const QVariant & image)
{
	if (!image.isNull() && image.canConvert<QImage>())
	{
		QImage img{image.value<QImage>()};
		Base::sendImage(index, img);
	}
	else
	{
		qCWarning(minervousStreamDeck) << "Unexpected format. Could not send image" << image;
	}
}

void StreamDeck::classBegin()
{
	// Nothign to do...
}

void StreamDeck::componentComplete()
{
	init();
	if (!model())
	{
		applyModel(_defaultData);
	}
}

StreamDeck::DefaultPropertyType StreamDeck::qmlData()
{
	return _defaultData->qmlData();
}
