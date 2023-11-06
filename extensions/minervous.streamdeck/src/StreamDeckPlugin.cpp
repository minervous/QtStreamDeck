#include "StreamDeckPlugin.hpp"

#include <QtGui/QPainter>

#include "QtQml/qqml.h"
#include <minervous/streamdeck/Device.hpp>
#include <minervous/streamdeck/DeviceManager.hpp>

using namespace minervous::streamdeck;

void StreamDeckPlugin::registerTypes(const char * uri)
{
	// Q_ASSERT(uri == QLatin1String("TimeExample"));
	qmlRegisterType<minervous::streamdeck::Device>(uri, 1, 0, "StreamDeck");

	qmlRegisterUncreatableType<minervous::streamdeck::DeviceTypeGadget>(
		uri,
		1,
		0,
		"StreamDeckType",
		"Not creatable as it is an enum type."
	);
	qmlRegisterUncreatableType<minervous::streamdeck::DeviceId>(
		uri,
		1,
		0,
		"StreamDeckDeviceId",
		"Not creatable as it is a gadget."
	);
}
