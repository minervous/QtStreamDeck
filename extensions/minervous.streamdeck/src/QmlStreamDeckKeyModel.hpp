#pragma once

#include "minervous/streamdeck/KeyModel.hpp"
#include "qqmlintegration.h"

namespace minervous::streamdeck
{
	class QmlStreamDeckKeyModel : public minervous::streamdeck::KeyModel
	{
		Q_OBJECT
		QML_ELEMENT
		QML_NAMED_ELEMENT(StreamDeckKeyModel)
		using Base = minervous::streamdeck::KeyModel;

		//Q_PROPERTY(QQmlListProperty<BaseKeyEntry> data READ qmlData NOTIFY dataChanged)
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
		Q_INVOKABLE void replace(BaseKeyEntry * entry, int index)
		{
			Base::replace(index, entry);
		}
		Q_INVOKABLE BaseKeyEntry * at(int index)
		{
			return Base::at(index);
		}
	signals:
		//void dataChanged();
	private:

		//qmlData

	};

}  // namespace minervous::streamdeck
