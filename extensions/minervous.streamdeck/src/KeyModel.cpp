#include "KeyModel.hpp"

#include <private/qqmlinstantiator_p.h>

using namespace minervous::streamdeck;

namespace minervous::streamdeck::qml {

KeyModel::KeyModel(QObject * parent)
	: Base{parent}
{}

void KeyModel::clear()
{
	Base::clear();
}

void KeyModel::append(BaseKeyEntry * entry)
{
	Base::append(entry);
}

void KeyModel::remove(qsizetype index)
{
	Base::remove(index);
}

void KeyModel::insert(qsizetype index, BaseKeyEntry * entry)
{
	Base::insert(index, entry);
}

void KeyModel::replace(qsizetype index, BaseKeyEntry * entry)
{
	Base::replace(index, entry);
}

const BaseKeyEntry * KeyModel::at(qsizetype index) const
{
	return Base::at(index);
}

BaseKeyEntry * KeyModel::at(qsizetype index)
{
	return Base::operator[](index);
}

KeyModel::DefaultPropertyType KeyModel::qmlData()
{
	return {
		this,
		nullptr,
		&KeyModel::qmlAppend,
		&KeyModel::qmlCount,
		&KeyModel::qmlAt,
		&KeyModel::qmlClear
	};
}

void KeyModel::qmlAppend(DefaultPropertyType * list, QObject * object)
{
	if (KeyModel * o = qobject_cast<KeyModel *>(list->object); o && object)
	{
		if (object)
			object->setParent(o);

		if (auto * entry = qobject_cast<BaseKeyEntry *>(object); entry)
		{
			o->append(entry);
			emit o->qmlDataChanged();
		}
		else if (auto * inst = qobject_cast<QQmlInstantiator *>(object); inst)
		{
			auto addInstantiatorChild = [=]([[maybe_unused]] int index, QObject * child)
			{
				child->setParent(o);
				if (auto * en = qobject_cast<BaseKeyEntry *>(child); en)
				{
					o->append(en);
				}

				emit o->qmlDataChanged();
			};
			auto removeInstantiatorChild = [=]([[maybe_unused]] int index, QObject * child)
			{
				if (auto * en = qobject_cast<BaseKeyEntry *>(child))
				{
					auto childIndex = o->_data.indexOf(en);
					if (childIndex >= 0)
					{
						o->remove(childIndex);
					}
				}
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

qsizetype KeyModel::qmlCount(DefaultPropertyType * list)
{
	auto * o = qobject_cast<KeyModel *>(list->object);
	return o ? o->children().size() : 0;
}

QObject * KeyModel::qmlAt(DefaultPropertyType * list, qsizetype index)
{
	auto * o = qobject_cast<KeyModel *>(list->object);
	return o ? o->children().at(index) : nullptr;
}

void KeyModel::qmlClear(DefaultPropertyType * list)
{
	if (auto * o = qobject_cast<KeyModel *>(list->object); o && o->count())
	{
		o->clear();
		emit o->qmlDataChanged();
	}
	list->clear(list);
}

} // namespace minervous::streamdeck::qml
