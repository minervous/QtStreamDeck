// #include "streamdeckqmlplugin.h"

// #include <QQmlApplicationEngine>

// #include <device.h>
// #include <devicemanager.h>

// StreamDeckQmlPlugin::StreamDeckQmlPlugin(QObject * parent)
// 	// : QQmlEngineExtensionPlugin(parent)
// 	: QQmlExtensionPlugin(parent)
// {
// 	volatile auto registration = &qml_register_types_StreamDeckQml;
// 	Q_UNUSED(registration)
// 	//    registerTypes("StreamDeckQml");

// 	//    qmlRegisterType<minervous::streamdeck::Device>("StreamDeckQml", 1, 0, "StreamDeck");

// 	//    qmlRegisterUncreatableType<minervous::streamdeck::DeviceTypeGadget>("StreamDeckQml", 1, 0, "StreamDeckType",
// 	//                                                             "Not creatable as it is an enum type.");
// 	//    qmlRegisterUncreatableType<minervous::streamdeck::DeviceId>("StreamDeckQml", 1, 0, "StreamDeckDeviceId",
// 	//                                                     "Not creatable as it is a gadget.");
// 	//   // qmlRegisterUncreatableType<minervous::streamdeck::DeviceManager>("StreamDeckQml", 1, 0,
// 	//   "StreamDecksManager");
// }

// void StreamDeckQmlPlugin::registerTypes(const char * uri)
// {
// 	qmlRegisterType<minervous::streamdeck::Device>(uri, 1, 0, "StreamDeck");

// 	qmlRegisterUncreatableType<minervous::streamdeck::DeviceTypeGadget>(
// 		uri,
// 		1,
// 		0,
// 		"StreamDeckType",
// 		"Not creatable as it is an enum type."
// 	);
// 	qmlRegisterUncreatableType<minervous::streamdeck::DeviceId>(
// 		uri,
// 		1,
// 		0,
// 		"StreamDeckDeviceId",
// 		"Not creatable as it is a gadget."
// 	);
// 	// qmlRegisterUncreatableType<minervous::streamdeck::DeviceManager>("StreamDeckQml", 1, 0, "StreamDecksManager");
// }
