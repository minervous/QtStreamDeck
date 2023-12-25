// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include <QtCore/QVariant>

#include <QtQml/qqmlparserstatus.h>

#include "KeyModel.hpp"
#include "minervous/streamdeck/Device.hpp"
#include "minervous/streamdeck/StreamDeckLogging.hpp"

#include "qqmlintegration.h"

namespace minervous::streamdeck::qml
{
	class StreamDeck
		: public minervous::streamdeck::Device
		, public QQmlParserStatus
	{
		Q_OBJECT
		QML_ELEMENT
		Q_INTERFACES(QQmlParserStatus)
		QML_NAMED_ELEMENT(StreamDeck)
		using Base = minervous::streamdeck::Device;

		using DefaultPropertyType = KeyModel::DefaultPropertyType;
		Q_PROPERTY(DefaultPropertyType data READ qmlData NOTIFY qmlDataChanged)

		Q_CLASSINFO("DefaultProperty", "data")

	public:
		explicit StreamDeck(QObject * parent = nullptr)
			: Base{parent}
			, _defaultData{new KeyModel(this)}
		{
			connect(_defaultData, &KeyModel::qmlDataChanged, this, &StreamDeck::qmlDataChanged);
		}

		Q_INVOKABLE bool open() { return Base::open(); }

		Q_INVOKABLE void close() { Base::close(); }

		Q_INVOKABLE void reset() { Base::reset(); }

		Q_INVOKABLE void sendImage(int index, const QVariant image)
		{
			if (!image.isNull() && image.canConvert<QImage>())
			{
				QImage img(image.value<QImage>());
				Base::sendImage(index, img);
			}
			else
			{
				qCWarning(minervousStreamDeck) << "Unexpected format. Could not send image" << image;
			}
		}

		Q_INVOKABLE void sendImage(int index, QUrl url) { Base::sendImage(index, url); }

		void classBegin() override {}

		void componentComplete() override
		{
			init();
			if (!model())
			{
				applyModel(_defaultData);
			}
		}

	signals:
		void qmlDataChanged();

	private:
		DefaultPropertyType qmlData() { return _defaultData->qmlData(); }

		KeyModel * _defaultData;
	};

}  // namespace minervous::streamdeck::qml
