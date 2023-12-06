#include "QmlPagedKeyModel.hpp"

#include <QtCore/QCoreApplication>

using namespace minervous::streamdeck;

QmlPagedKeyModel::QmlPagedKeyModel(QObject * parent)
	: Base{parent}
{}

void QmlPagedKeyModel::classBegin() {}

void QmlPagedKeyModel::componentComplete()
{
	init();
}

void QmlPagedKeyModel::nextPage()
{
	Base::nextPage();
}

void QmlPagedKeyModel::prevPage()
{
	Base::prevPage();
}

const BaseKeyEntry * QmlPagedKeyModel::at(qsizetype index) const
{
	return Base::at(index);
}

BaseKeyEntry * QmlPagedKeyModel::at(qsizetype index)
{
	return Base::operator[](index);
}

QmlPagedKeyModel::DefaultPropertyType QmlPagedKeyModel::qmlData()
{
	return {
		this,
		nullptr,
		&QmlPagedKeyModel::qmlAppend,
		&QmlPagedKeyModel::qmlCount,
		&QmlPagedKeyModel::qmlAt,
		DefaultPropertyType::ClearFunction()
	};
}

void QmlPagedKeyModel::qmlAppend(DefaultPropertyType * list, QObject * object)
{
	if (auto * o = qobject_cast<QmlPagedKeyModel *>(list->object); o && object)
	{
		object->setParent(o);
		emit o->qmlDataChanged();
	}
}

qsizetype QmlPagedKeyModel::qmlCount(DefaultPropertyType * list)
{
	auto * o = qobject_cast<QmlPagedKeyModel *>(list->object);
	return o ? o->children().size() : 0;
}

QObject * QmlPagedKeyModel::qmlAt(DefaultPropertyType * list, qsizetype index)
{
	auto * o = qobject_cast<QmlPagedKeyModel *>(list->object);
	return o ? o->children().at(index) : nullptr;
}
