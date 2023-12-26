// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#include "PagedKeyModel.hpp"

#include <QtCore/QCoreApplication>

using namespace minervous::streamdeck::qml;
using minervous::streamdeck::BaseKeyEntry;

PagedKeyModel::PagedKeyModel(QObject * parent)
	: Base{parent}
{}

void PagedKeyModel::classBegin()
{
	// Nothing to do...
}

void PagedKeyModel::componentComplete()
{
	init();
}

const BaseKeyEntry * PagedKeyModel::at(qsizetype index) const
{
	return Base::at(index);
}

BaseKeyEntry * PagedKeyModel::at(qsizetype index)
{
	return Base::operator[](index);
}

PagedKeyModel::DefaultPropertyType PagedKeyModel::qmlData()
{
	return {
		this,
		nullptr,
		&PagedKeyModel::qmlAppend,
		&PagedKeyModel::qmlCount,
		&PagedKeyModel::qmlAt,
		DefaultPropertyType::ClearFunction()
	};
}

void PagedKeyModel::qmlAppend(DefaultPropertyType * list, QObject * object)
{
	if (auto * o = qobject_cast<PagedKeyModel *>(list->object); o && object)
	{
		object->setParent(o);
		emit o->qmlDataChanged();
	}
}

qsizetype PagedKeyModel::qmlCount(DefaultPropertyType * list)
{
	const auto * o = qobject_cast<PagedKeyModel *>(list->object);
	return o ? o->children().size() : 0;
}

QObject * PagedKeyModel::qmlAt(DefaultPropertyType * list, qsizetype index)
{
	const auto * o = qobject_cast<PagedKeyModel *>(list->object);
	return o ? o->children().at(index) : nullptr;
}
