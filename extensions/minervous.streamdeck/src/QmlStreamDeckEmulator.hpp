#pragma once

#include "minervous/streamdeck/DeviceEmulator.hpp"
#include "QtQml/qqmlparserstatus.h"
#include "qqmlintegration.h"

namespace minervous::streamdeck
{
	class QmlStreamDeckEmulator : public minervous::streamdeck::DeviceEmulator, public QQmlParserStatus
	{
		Q_OBJECT
		QML_ELEMENT
		Q_INTERFACES(QQmlParserStatus)
		QML_NAMED_ELEMENT(StreamDeckEmulator)
		using Base = minervous::streamdeck::DeviceEmulator;
	public:
		QmlStreamDeckEmulator(QObject * parent = nullptr)
			: Base{parent}
		{}

		Q_INVOKABLE void press(int index)
		{
			Base::press(index);
		}
		Q_INVOKABLE void release(int index)
		{
			Base::release(index);
		}

		void classBegin() override
		{}
		void componentComplete() override
		{
			init();
		}
	};

}  // namespace minervous::streamdeck
