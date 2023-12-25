// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include <QtQml/QQmlEngine>

#include "minervous/streamdeck/BaseKeyEntry.hpp"
#include "minervous/streamdeck/ImageHelper.hpp"

namespace minervous::streamdeck::qml
{

	class KeyEntry: public minervous::streamdeck::BaseKeyEntry
	{
		Q_OBJECT
		QML_ELEMENT
		QML_NAMED_ELEMENT(KeyEntry)
		using Base = minervous::streamdeck::BaseKeyEntry;

		Q_PROPERTY(QVariant image READ image WRITE setImage NOTIFY imageChanged FINAL)

		using DefaultPropertyType = QQmlListProperty<QObject>;
		Q_PROPERTY(DefaultPropertyType data READ qmlData NOTIFY qmlDataChanged)

		Q_CLASSINFO("DefaultProperty", "data")

	public:
		explicit KeyEntry(QObject * parent = nullptr)
			: Base{parent}
		{}

		QVariant image() const
		{
			const auto & img = Base::image();
			if (img.isNull())
			{
				return {};
			}

			return {img};
		}

		void setImage(QVariant v)
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

		Q_INVOKABLE QUrl imageAsUrl() const { return ImageHelper::imageToUrl(Base::image()); }

	signals:
		void qmlDataChanged();

	private:
		DefaultPropertyType qmlData()
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

		static void qmlAppend(DefaultPropertyType * list, QObject * object)
		{
			if (auto * o = qobject_cast<KeyEntry *>(list->object); o && object)
			{
				object->setParent(o);
				emit o->qmlDataChanged();
			}
		}

		static qsizetype qmlCount(DefaultPropertyType * list)
		{
			auto * o = qobject_cast<KeyEntry *>(list->object);
			return o ? o->children().size() : 0;
		}

		static QObject * qmlAt(DefaultPropertyType * list, qsizetype index)
		{
			auto * o = qobject_cast<KeyEntry *>(list->object);
			return o ? o->children().at(index) : nullptr;
		}
	};

}  // namespace minervous::streamdeck::qml
