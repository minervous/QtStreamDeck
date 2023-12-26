// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include <QtCore/QVariant>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlParserStatus>

#include "KeyModel.hpp"
#include "minervous/streamdeck/Device.hpp"

namespace minervous::streamdeck::qml
{
	class StreamDeck
		: public minervous::streamdeck::Device
		, public QQmlParserStatus
	{
		Q_OBJECT
		Q_INTERFACES(QQmlParserStatus)
		QML_NAMED_ELEMENT(StreamDeck)
		Q_CLASSINFO("DefaultProperty", "data")

		using DefaultPropertyType = KeyModel::DefaultPropertyType;
		Q_PROPERTY(DefaultPropertyType data READ qmlData NOTIFY qmlDataChanged)

		using Base = minervous::streamdeck::Device;

	public:
		explicit StreamDeck(QObject * parent = nullptr);

		using Device::sendImage;

		Q_INVOKABLE void sendImage(int index, const QVariant & image);

		void classBegin() override;

		void componentComplete() override;

	signals:
		void qmlDataChanged();

	private:
		DefaultPropertyType qmlData();

		KeyModel * _defaultData;
	};

}  // namespace minervous::streamdeck::qml
