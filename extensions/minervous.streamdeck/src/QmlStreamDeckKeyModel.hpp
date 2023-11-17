#pragma once

#include <QtQml/QQmlListProperty>
#include "minervous/streamdeck/KeyModel.hpp"
#include "qqmlintegration.h"

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
		QmlStreamDeckKeyModel(QObject * parent = nullptr)
			: Base{parent}
		{}

		Q_INVOKABLE void clear()
		{
			Base::clear();
		}
		Q_INVOKABLE void append(BaseKeyEntry * entry)
		{
			Base::append(entry);
		}
		Q_INVOKABLE void remove(int index)
		{
			Base::remove(index);
		}
		Q_INVOKABLE void insert(int index, BaseKeyEntry * entry)
		{
			Base::insert(index, entry);
		}
		Q_INVOKABLE void replace(int index, BaseKeyEntry * entry)
		{
			Base::replace(index, entry);
		}
		Q_INVOKABLE BaseKeyEntry * at(int index)
		{
			return Base::at(index);
		}

	signals:
		void qmlDataChanged();

	private:

		DefaultPropertyType qmlData()
		{
			return {this, nullptr,
					&QmlStreamDeckKeyModel::qmlAppend,
					&QmlStreamDeckKeyModel::qmlCount,
					&QmlStreamDeckKeyModel::qmlAt,
					&QmlStreamDeckKeyModel::qmlClear,
					&QmlStreamDeckKeyModel::qmlReplace,
					&QmlStreamDeckKeyModel::qmlRemoveLast};
		}
		friend class QmlStreamDeck;

		static void qmlAppend(DefaultPropertyType * list, QObject *object)
		{
			QmlStreamDeckKeyModel *o = qobject_cast<QmlStreamDeckKeyModel*>(list->object);
			BaseKeyEntry * entry = qobject_cast<BaseKeyEntry*>(object);
			if (o && entry)
			{
				o->append(entry);
				emit o->qmlDataChanged();
			}
		}
		static qsizetype qmlCount(DefaultPropertyType * list)
		{
			QmlStreamDeckKeyModel *o = qobject_cast<QmlStreamDeckKeyModel*>(list->object);
			return o ? o->count() : 0;
		}
		static QObject* qmlAt(DefaultPropertyType * list, qsizetype index)
		{
			QmlStreamDeckKeyModel *o = qobject_cast<QmlStreamDeckKeyModel*>(list->object);
			return o ? o->at(index) : nullptr;
		}
		static void qmlClear(DefaultPropertyType * list)
		{
			QmlStreamDeckKeyModel *o = qobject_cast<QmlStreamDeckKeyModel*>(list->object);
			if (o && o->count())
			{
				o->clear();
				emit o->qmlDataChanged();
			}
		}
		static void qmlReplace(DefaultPropertyType * list, qsizetype index, QObject *object)
		{
			QmlStreamDeckKeyModel *o = qobject_cast<QmlStreamDeckKeyModel*>(list->object);
			BaseKeyEntry * entry = qobject_cast<BaseKeyEntry*>(object);
			if (o && entry)
			{
				o->replace(index, entry);
				emit o->qmlDataChanged();
			}
		}
		static void qmlRemoveLast(DefaultPropertyType * list)
		{
			QmlStreamDeckKeyModel *o = qobject_cast<QmlStreamDeckKeyModel*>(list->object);
			if (o && o->count())
			{
				o->remove(o->count() - 1);
				emit o->qmlDataChanged();
			}
		}
	};

}  // namespace minervous::streamdeck
