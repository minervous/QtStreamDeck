// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#include "KeyEntry.hpp"

#include "minervous/streamdeck/ImageHelper.hpp"

using namespace minervous::streamdeck::qml;

using Base = minervous::streamdeck::BaseKeyEntry;

QVariant KeyEntry::image() const
{
	const auto & img = Base::image();
	if (img.isNull())
		return {};
	else
		return {img};
}

void KeyEntry::setImage(const QVariant & v)
{
	if (v.canConvert<QImage>())
	{
		Base::setImage(v.value<QImage>());
	}
	else
	{
		Base::setImage({});
	}
}

QUrl KeyEntry::imageAsUrl() const
{
	return ImageHelper::imageToUrl(Base::image());
}

KeyEntry::DefaultPropertyType KeyEntry::qmlData()
{
	return {
		this,
		nullptr,
		&KeyEntry::qmlAppend,
		&KeyEntry::qmlCount,
		&KeyEntry::qmlAt,
		DefaultPropertyType::ClearFunction()
	};
}

void KeyEntry::qmlAppend(DefaultPropertyType * list, QObject * object)
{
	if (auto * o = qobject_cast<KeyEntry *>(list->object); o && object)
	{
		object->setParent(o);
		emit o->qmlDataChanged();
	}
}

qsizetype KeyEntry::qmlCount(DefaultPropertyType * list)
{
	const auto * o = qobject_cast<KeyEntry *>(list->object);
	return o ? o->children().size() : 0;
}

QObject * KeyEntry::qmlAt(DefaultPropertyType * list, qsizetype index)
{
	const auto * o = qobject_cast<KeyEntry *>(list->object);
	return o ? o->children().at(index) : nullptr;
}
