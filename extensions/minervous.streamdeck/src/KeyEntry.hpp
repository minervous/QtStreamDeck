// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include <QtQml/QQmlEngine>

#include "minervous/streamdeck/BaseKeyEntry.hpp"

namespace minervous::streamdeck::qml
{

	class KeyEntry: public minervous::streamdeck::BaseKeyEntry
	{
		Q_OBJECT
		QML_NAMED_ELEMENT(KeyEntry)
		Q_CLASSINFO("DefaultProperty", "data")

		using DefaultPropertyType = QQmlListProperty<QObject>;
		Q_PROPERTY(DefaultPropertyType data READ qmlData NOTIFY qmlDataChanged)
		Q_PROPERTY(QVariant image READ image WRITE setImage NOTIFY imageChanged FINAL)

	public:
		using minervous::streamdeck::BaseKeyEntry::BaseKeyEntry;

		QVariant image() const;
		void setImage(const QVariant & v);

		Q_INVOKABLE QUrl imageAsUrl() const;

	signals:
		void qmlDataChanged();

	private:
		DefaultPropertyType qmlData();

		static void qmlAppend(DefaultPropertyType * list, QObject * object);
		static qsizetype qmlCount(DefaultPropertyType * list);
		static QObject * qmlAt(DefaultPropertyType * list, qsizetype index);
	};

}  // namespace minervous::streamdeck::qml
