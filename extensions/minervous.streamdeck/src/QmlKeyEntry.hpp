#pragma once

#include <QtQml/QQmlEngine>

#include "minervous/streamdeck/BaseKeyEntry.hpp"
#include "minervous/streamdeck/ImageHelper.hpp"

namespace minervous::streamdeck
{

	class QmlKeyEntry: public minervous::streamdeck::BaseKeyEntry
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
		explicit QmlKeyEntry(QObject * parent = nullptr)
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
				&QmlKeyEntry::qmlAppend,
				&QmlKeyEntry::qmlCount,
				&QmlKeyEntry::qmlAt,
				DefaultPropertyType::ClearFunction()
			};
		}

		static void qmlAppend(DefaultPropertyType * list, QObject * object)
		{
			if (auto * o = qobject_cast<QmlKeyEntry *>(list->object); o && object)
			{
				object->setParent(o);
				emit o->qmlDataChanged();
			}
		}

		static qsizetype qmlCount(DefaultPropertyType * list)
		{
			auto * o = qobject_cast<QmlKeyEntry *>(list->object);
			return o ? o->children().size() : 0;
		}

		static QObject * qmlAt(DefaultPropertyType * list, qsizetype index)
		{
			auto * o = qobject_cast<QmlKeyEntry *>(list->object);
			return o ? o->children().at(index) : nullptr;
		}
	};

}  // namespace minervous::streamdeck
