#pragma once

#include <QtQml/QQmlEngine>

#include "minervous/streamdeck/BaseKeyEntry.hpp"

namespace minervous::streamdeck
{

	class QmlBaseKeyEntry : public minervous::streamdeck::BaseKeyEntry
	{
		Q_OBJECT
		QML_ELEMENT
		QML_NAMED_ELEMENT(StreamDeckKeyEntry)
		using Base = minervous::streamdeck::BaseKeyEntry;

		using DefaultPropertyType = QQmlListProperty<QObject>;
		Q_PROPERTY(DefaultPropertyType data READ qmlData NOTIFY qmlDataChanged)

		Q_CLASSINFO("DefaultProperty", "data")
	public:
		QmlBaseKeyEntry(QObject * parent = nullptr)
			: Base{parent}
		{}
	signals:
		void qmlDataChanged();

	private:
		DefaultPropertyType qmlData()
		{
			return {this, nullptr,
				&QmlBaseKeyEntry::qmlAppend,
				&QmlBaseKeyEntry::qmlCount,
				&QmlBaseKeyEntry::qmlAt,
				DefaultPropertyType::ClearFunction()
			};
		}

		static void qmlAppend(DefaultPropertyType * list, QObject *object)
		{
			QmlBaseKeyEntry *o = qobject_cast<QmlBaseKeyEntry*>(list->object);
			if (o && object)
			{
				object->setParent(o);
				emit o->qmlDataChanged();
			}
		}
		static qsizetype qmlCount(DefaultPropertyType * list)
		{
			QmlBaseKeyEntry *o = qobject_cast<QmlBaseKeyEntry*>(list->object);
			return o ? o->children().size() : 0;
		}
		static QObject* qmlAt(DefaultPropertyType * list, qsizetype index)
		{
			QmlBaseKeyEntry *o = qobject_cast<QmlBaseKeyEntry*>(list->object);
			return o ? o->children().at(index) : nullptr;
		}
	};

}  // namespace minervous::streamdeck
