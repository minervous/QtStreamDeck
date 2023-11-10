#pragma once

#include <QtQml/QQmlEngineExtensionPlugin>

#include "minervous/streamdeck/Device.hpp"
#include "minervous/streamdeck/DeviceEmulator.hpp"
#include "minervous/streamdeck/DeviceManager.hpp"
#include "QtCore/qtmetamacros.h"

namespace minervous::streamdeck
{
	class StreamDeckPlugin: public QQmlEngineExtensionPlugin
	{
		Q_OBJECT
		Q_PLUGIN_METADATA(IID QQmlEngineExtensionInterface_iid)
	};

	class StreamDeck : public minervous::streamdeck::Device, public QQmlParserStatus
	{
		Q_OBJECT
		QML_ELEMENT
		Q_INTERFACES(QQmlParserStatus)
		using Base = minervous::streamdeck::Device;
	public:
		StreamDeck(QObject * parent = nullptr)
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

	class StreamDeckEmulator : public minervous::streamdeck::DeviceEmulator, public QQmlParserStatus
	{
		Q_OBJECT
		QML_ELEMENT
		Q_INTERFACES(QQmlParserStatus)
		using Base = minervous::streamdeck::DeviceEmulator;
	public:
		StreamDeckEmulator(QObject * parent = nullptr)
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

	struct StreamDeckDeviceId
	{
		Q_GADGET
		QML_FOREIGN(minervous::streamdeck::DeviceId)
		QML_NAMED_ELEMENT(deviceId)
	};

	struct StreamDeckManager
	{
		Q_GADGET
		QML_FOREIGN(minervous::streamdeck::DeviceManager)
		QML_SINGLETON
		QML_NAMED_ELEMENT(StreamDeckManager)
	};

}  // namespace minervous::streamdeck
