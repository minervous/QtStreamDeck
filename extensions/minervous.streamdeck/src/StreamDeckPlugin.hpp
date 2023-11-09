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
	public:
		StreamDeck(QObject * parent = nullptr)
			: minervous::streamdeck::Device(parent)
		{}

		Q_INVOKABLE bool open()
		{
			return minervous::streamdeck::Device::open();
		}
		Q_INVOKABLE void close()
		{
			minervous::streamdeck::Device::close();
		}
		Q_INVOKABLE void reset()
		{
			minervous::streamdeck::Device::reset();
		}
		Q_INVOKABLE void setImageUrl(int index, QUrl url)
		{
			minervous::streamdeck::Device::setImageUrl(index, url);
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
	public:
		StreamDeckEmulator(QObject * parent = nullptr)
			: minervous::streamdeck::DeviceEmulator(parent)
		{}

		Q_INVOKABLE void press(int index)
		{
			minervous::streamdeck::DeviceEmulator::press(index);
		}
		Q_INVOKABLE void release(int index)
		{
			minervous::streamdeck::DeviceEmulator::release(index);
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
