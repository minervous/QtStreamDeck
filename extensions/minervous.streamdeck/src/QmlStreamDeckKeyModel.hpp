#pragma once

#include <QtQml/QQmlEngine>
#include <QtQml/QQmlListProperty>

#include "minervous/streamdeck/KeyModel.hpp"

namespace minervous::streamdeck
{
	class QmlStreamDeck;

	class QmlStreamDeckKeyModel : public minervous::streamdeck::KeyModel
	{
		Q_OBJECT
		QML_ELEMENT
		QML_NAMED_ELEMENT(StreamDeckKeyModel)
		using Base = minervous::streamdeck::KeyModel;

		using DefaultPropertyType = QQmlListProperty<QObject>;
		Q_PROPERTY(DefaultPropertyType data READ qmlData NOTIFY qmlDataChanged)

		Q_CLASSINFO("DefaultProperty", "data")
	public:
		QmlStreamDeckKeyModel(QObject * parent = nullptr);
		~QmlStreamDeckKeyModel() override = default;

		Q_INVOKABLE void clear();
		Q_INVOKABLE void append(BaseKeyEntry * entry);
		Q_INVOKABLE void remove(qsizetype index);
		Q_INVOKABLE void insert(qsizetype index, BaseKeyEntry * entry);
		Q_INVOKABLE void replace(qsizetype index, BaseKeyEntry * entry);
		Q_INVOKABLE const BaseKeyEntry * at(qsizetype index) const;
		Q_INVOKABLE BaseKeyEntry * at(qsizetype index);

	signals:
		void qmlDataChanged();

	private:

		DefaultPropertyType qmlData();
		friend class QmlStreamDeck;

		static void qmlAppend(DefaultPropertyType * list, QObject *object);
		static qsizetype qmlCount(DefaultPropertyType * list);
		static QObject* qmlAt(DefaultPropertyType * list, qsizetype index);
		static void qmlClear(DefaultPropertyType * list);
		static void qmlReplace(DefaultPropertyType * list, qsizetype index, QObject *object);
		static void qmlRemoveLast(DefaultPropertyType * list);
	};

}  // namespace minervous::streamdeck
