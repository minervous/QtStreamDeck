#include "QmlKeyModel.hpp"

#include <private/qqmlinstantiator_p.h>

using namespace minervous::streamdeck;

QmlKeyModel::QmlKeyModel(QObject * parent)
	: Base{parent}
{}

void QmlKeyModel::clear()
{
	Base::clear();
}

void QmlKeyModel::append(BaseKeyEntry * entry)
{
	Base::append(entry);
}

void QmlKeyModel::remove(qsizetype index)
{
	Base::remove(index);
}

void QmlKeyModel::insert(qsizetype index, BaseKeyEntry * entry)
{
	Base::insert(index, entry);
}

void QmlKeyModel::replace(qsizetype index, BaseKeyEntry * entry)
{
	Base::replace(index, entry);
}

const BaseKeyEntry * QmlKeyModel::at(qsizetype index) const
{
	return Base::at(index);
}

BaseKeyEntry * QmlKeyModel::at(qsizetype index)
{
	return Base::operator[](index);
}

QmlKeyModel::DefaultPropertyType QmlKeyModel::qmlData()
{
	return {
		this,
		nullptr,
		&QmlKeyModel::qmlAppend,
		&QmlKeyModel::qmlCount,
		&QmlKeyModel::qmlAt,
		&QmlKeyModel::qmlClear
	};
}

void QmlKeyModel::qmlAppend(DefaultPropertyType * list, QObject * object)
{
	if (QmlKeyModel * o = qobject_cast<QmlKeyModel *>(list->object); o && object)
	{
		if (object)
			object->setParent(o);

		if (auto * entry = qobject_cast<BaseKeyEntry *>(object); entry)
		{
			qDebug() << "Add child entry" << entry;
			o->append(entry);
			emit o->qmlDataChanged();
			qDebug() << "Current count" << o->count();
		}
		else if (auto * inst = qobject_cast<QQmlInstantiator *>(object); inst)
		{
			auto addInstantiatorChild = [=]([[maybe_unused]] int index, QObject * child)
			{
				child->setParent(o);
				qDebug() << "Add child" << child << "with index" << index << "from Instantiator";
				if (auto * en = qobject_cast<BaseKeyEntry *>(child); en)
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

qsizetype QmlKeyModel::qmlCount(DefaultPropertyType * list)
{
	auto * o = qobject_cast<QmlKeyModel *>(list->object);
	return o ? o->children().size() : 0;
}

QObject * QmlKeyModel::qmlAt(DefaultPropertyType * list, qsizetype index)
{
	auto * o = qobject_cast<QmlKeyModel *>(list->object);
	return o ? o->children().at(index) : nullptr;
}

void QmlKeyModel::qmlClear(DefaultPropertyType * list)
{
	if (auto * o = qobject_cast<QmlKeyModel *>(list->object); o && o->count())
	{
		o->clear();
		emit o->qmlDataChanged();
	}
	list->clear(list);
}
