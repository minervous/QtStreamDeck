#pragma once

#include "minervous/streamdeck/Device.hpp"
#include "QtQml/qqmlparserstatus.h"
#include "qqmlintegration.h"

namespace minervous::streamdeck
{
	class QmlStreamDeck : public minervous::streamdeck::Device, public QQmlParserStatus
	{
		Q_OBJECT
		QML_ELEMENT
		Q_INTERFACES(QQmlParserStatus)
		QML_NAMED_ELEMENT(StreamDeck)
		using Base = minervous::streamdeck::Device;
	public:
		QmlStreamDeck(QObject * parent = nullptr)
			: Base{parent}
		{}

		Q_INVOKABLE bool open()
		{
			return Base::open();
		}
		Q_INVOKABLE void close()
		{
			Base::close();
		}
		Q_INVOKABLE void reset()
		{
			Base::reset();
		}
		Q_INVOKABLE void sendImage(int index, QUrl url)
		{
			Base::sendImage(index, url);
		}

		void classBegin() override
		{}
		void componentComplete() override
		{
			init();
		}
	};

}  // namespace minervous::streamdeck
