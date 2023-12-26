// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlListProperty>
#include <QtQml/QQmlParserStatus>

#include "minervous/streamdeck/KeyModel.hpp"

namespace minervous::streamdeck::qml
{
	class StreamDeck;

	class KeyModel: public minervous::streamdeck::KeyModel
	{
		Q_OBJECT
		QML_NAMED_ELEMENT(KeyModel)
		Q_CLASSINFO("DefaultProperty", "data")

		using DefaultPropertyType = QQmlListProperty<QObject>;
		Q_PROPERTY(DefaultPropertyType data READ qmlData NOTIFY qmlDataChanged)

	public:
		explicit KeyModel(QObject * parent = nullptr);
		~KeyModel() override = default;

		Q_INVOKABLE const BaseKeyEntry * at(qsizetype index) const;
		Q_INVOKABLE BaseKeyEntry * at(qsizetype index);

	signals:
		void qmlDataChanged();

	private:
		DefaultPropertyType qmlData();
		friend class StreamDeck;

		static void qmlAppend(DefaultPropertyType * list, QObject * object);
		static qsizetype qmlCount(DefaultPropertyType * list);
		static QObject * qmlAt(DefaultPropertyType * list, qsizetype index);
		static void qmlClear(DefaultPropertyType * list);
		static void qmlReplace(DefaultPropertyType * list, qsizetype index, QObject * object);
		static void qmlRemoveLast(DefaultPropertyType * list);
	};

}  // namespace minervous::streamdeck::qml
