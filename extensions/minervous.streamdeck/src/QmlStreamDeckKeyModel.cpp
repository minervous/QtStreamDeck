#include "QmlStreamDeckKeyModel.hpp"

#include <private/qqmlinstantiator_p.h>

using namespace minervous::streamdeck;

QmlStreamDeckKeyModel::QmlStreamDeckKeyModel(QObject * parent)
	: Base{parent}
{}

void QmlStreamDeckKeyModel::clear()
{
	Base::clear();
}

void QmlStreamDeckKeyModel::append(BaseKeyEntry * entry)
{
	Base::append(entry);
}

void QmlStreamDeckKeyModel::remove(qsizetype index)
{
	Base::remove(index);
}

void QmlStreamDeckKeyModel::insert(qsizetype index, BaseKeyEntry * entry)
{
	Base::insert(index, entry);
}

void QmlStreamDeckKeyModel::replace(qsizetype index, BaseKeyEntry * entry)
{
	Base::replace(index, entry);
}

const BaseKeyEntry * QmlStreamDeckKeyModel::at(qsizetype index) const
{
	return Base::at(index);
}

BaseKeyEntry * QmlStreamDeckKeyModel::at(qsizetype index)
{
	return Base::operator[](index);
}

QmlStreamDeckKeyModel::DefaultPropertyType QmlStreamDeckKeyModel::qmlData()
{
	return {
		this,
		nullptr,
		&QmlStreamDeckKeyModel::qmlAppend,
		&QmlStreamDeckKeyModel::qmlCount,
		&QmlStreamDeckKeyModel::qmlAt,
		&QmlStreamDeckKeyModel::qmlClear
	};
}

void QmlStreamDeckKeyModel::qmlAppend(DefaultPropertyType * list, QObject * object)
{
	QmlStreamDeckKeyModel * o = qobject_cast<QmlStreamDeckKeyModel *>(list->object);
	if (o && object)
	{
		if (object)
			object->setParent(o);

		if (auto entry = qobject_cast<BaseKeyEntry *>(object))
		{
			qDebug() << "Add child entry" << entry;
			o->append(entry);
			emit o->qmlDataChanged();
			qDebug() << "Current count" << o->count();
		}
		else if (auto inst = qobject_cast<QQmlInstantiator *>(object))
		{
			auto addInstantiatorChild = [=]([[maybe_unused]] int index, QObject * child)
			{
				child->setParent(o);
				qDebug() << "Add child" << child << "with index" << index << "from Instantiator";
				if (auto * en = qobject_cast<BaseKeyEntry *>(child))
				{
					o->append(en);
				}
				qDebug() << "Current count" << o->count();

				emit o->qmlDataChanged();
			};
			auto removeInstantiatorChild = [=]([[maybe_unused]] int index, QObject * child)
			{
				qDebug() << "Remove child" << child << "with index" << index << "from Instantiator";
				if (auto * en = qobject_cast<BaseKeyEntry *>(child))
				{
					auto childIndex = o->_data.indexOf(en);
					if (childIndex >= 0)
					{
						qDebug() << "Remove entryIndex" << childIndex;
						o->remove(childIndex);
					}
				}
				qDebug() << "Current count" << o->count();
			};

			connect(inst, &QQmlInstantiator::objectRemoved, o, removeInstantiatorChild);
			connect(inst, &QQmlInstantiator::objectAdded, o, addInstantiatorChild);
			for (int i = 0, max = inst->count(); i < max; ++i)
			{
				addInstantiatorChild(i, inst->objectAt(i));
			}
		}
	}
}

qsizetype QmlStreamDeckKeyModel::qmlCount(DefaultPropertyType * list)
{
	QmlStreamDeckKeyModel * o = qobject_cast<QmlStreamDeckKeyModel *>(list->object);
	return o ? o->children().size() : 0;
}

QObject * QmlStreamDeckKeyModel::qmlAt(DefaultPropertyType * list, qsizetype index)
{
	QmlStreamDeckKeyModel * o = qobject_cast<QmlStreamDeckKeyModel *>(list->object);
	return o ? o->children().at(index) : nullptr;
}

void QmlStreamDeckKeyModel::qmlClear(DefaultPropertyType * list)
{
	QmlStreamDeckKeyModel * o = qobject_cast<QmlStreamDeckKeyModel *>(list->object);
	if (o && o->count())
	{
		o->clear();
		emit o->qmlDataChanged();
	}
	list->clear(list);
}
