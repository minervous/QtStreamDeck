#pragma once

#include <QtQml/QQmlEngineExtensionPlugin>

#include "minervous/streamdeck/Device.hpp"
#include "minervous/streamdeck/DeviceEmulator.hpp"
#include "minervous/streamdeck/DeviceManager.hpp"

namespace minervous::streamdeck
{
	class StreamDeckPlugin: public QQmlEngineExtensionPlugin
	{
		Q_OBJECT
		Q_PLUGIN_METADATA(IID QQmlEngineExtensionInterface_iid)
	};

	struct StreamDeckQml
	{
		Q_GADGET
		QML_FOREIGN(minervous::streamdeck::Device)
		QML_NAMED_ELEMENT(StreamDeck)
		// QML_EXTENDED(minervous::streamdeck::Device)
	};

	struct StreamDeckEmulatorQml
	{
		Q_GADGET
		QML_FOREIGN(minervous::streamdeck::DeviceEmulator)
		QML_NAMED_ELEMENT(StreamDeckEmulator)
		// QML_EXTENDED(minervous::streamdeck::Device)
	};

	struct StreamDeckTypeQml
	{
		Q_GADGET
		QML_FOREIGN(minervous::streamdeck::DeviceTypeGadget)
		QML_NAMED_ELEMENT(StreamDeckType)
	};

	struct StreamDeckManagerQml
	{
		Q_GADGET
		QML_FOREIGN(minervous::streamdeck::DeviceManager)
		QML_SINGLETON
		QML_NAMED_ELEMENT(StreamDeckManager)
	};

}  // namespace minervous::streamdeck
