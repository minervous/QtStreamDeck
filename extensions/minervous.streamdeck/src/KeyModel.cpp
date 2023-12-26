// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#include "KeyModel.hpp"

#include <QtQmlModels/private/qqmlinstantiator_p.h>

using namespace minervous::streamdeck::qml;
using minervous::streamdeck::BaseKeyEntry;
using Base = minervous::streamdeck::KeyModel;

KeyModel::KeyModel(QObject * parent)
	: Base{parent}
{}

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
		&KeyModel::qmlClear,
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
		else if (const auto * inst = qobject_cast<QQmlInstantiator *>(object); inst)
		{
			auto addInstantiatorChild = [o]([[maybe_unused]] int index, QObject * child)
			{
				child->setParent(o);
				if (auto * en = qobject_cast<BaseKeyEntry *>(child); en)
				{
					o->append(en);
				}

				emit o->qmlDataChanged();
			};
			auto removeInstantiatorChild = [o]([[maybe_unused]] int index, QObject * child)
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
	const auto * o = qobject_cast<KeyModel *>(list->object);
	return o ? o->children().size() : 0;
}

QObject * KeyModel::qmlAt(DefaultPropertyType * list, qsizetype index)
{
	const auto * o = qobject_cast<KeyModel *>(list->object);
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
