// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include <QtQml/QQmlEngine>
#include <QtCore/qcoreevent.h>

#include "minervous/streamdeck/PagedKeyModel.hpp"


namespace minervous::streamdeck::qml
{
	class PagedKeyModel: public minervous::streamdeck::PagedKeyModel, public QQmlParserStatus
	{
		Q_OBJECT
		QML_ELEMENT
		Q_INTERFACES(QQmlParserStatus)
		QML_NAMED_ELEMENT(PagedKeyModel)
		using Base = minervous::streamdeck::PagedKeyModel;

		using DefaultPropertyType = QQmlListProperty<QObject>;
		Q_PROPERTY(DefaultPropertyType data READ qmlData NOTIFY qmlDataChanged)

		Q_CLASSINFO("DefaultProperty", "data")
	public:
		explicit PagedKeyModel(QObject * parent = nullptr);
		~PagedKeyModel() override = default;

		void classBegin() override;
		void componentComplete() override;

		Q_INVOKABLE const BaseKeyEntry * at(qsizetype index) const;
		Q_INVOKABLE BaseKeyEntry * at(qsizetype index);

		Q_INVOKABLE void nextPage();

		Q_INVOKABLE void prevPage();

	signals:
		void qmlDataChanged();

	private:
		DefaultPropertyType qmlData();

		static void qmlAppend(DefaultPropertyType * list, QObject * object);
		static qsizetype qmlCount(DefaultPropertyType * list);
		static QObject * qmlAt(DefaultPropertyType * list, qsizetype index);
	};

}  // namespace minervous::streamdeck::qml
