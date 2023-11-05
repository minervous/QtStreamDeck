#pragma once

// #include <QtQml/QQmlEngine>

// #include <QtQml/qqmlextensionplugin.h>

// #include "QtQml/qqmlregistration.h"

// extern void qml_register_types_StreamDeckQml();
// Q_GHS_KEEP_REFERENCE(qml_register_types_StreamDeckQml)

// class StreamDeckQmlPlugin: public QQmlExtensionPlugin  // QQmlEngineExtensionPlugin
// {
// 	Q_OBJECT
// 	Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

// public:
// 	StreamDeckQmlPlugin(QObject * parent = nullptr);
// 	void registerTypes(const char * uri) override;
// };

#include "minervous/stream-deck/Device.hpp"
#include "minervous/stream-deck/DeviceManager.hpp"

struct StreamDeckQml
{
	Q_GADGET
	QML_FOREIGN(minervous::streamdeck::Device)
	QML_NAMED_ELEMENT(StreamDeck)
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
