#include "streamdeckplugin.h"
#include "QtQml/qqml.h"

#include <QPainter>

#include <device.h>
#include <devicemanager.h>

void StreamDeckPlugin::registerTypes(const char *uri)
{
    //Q_ASSERT(uri == QLatin1String("TimeExample"));
    qmlRegisterType<streamdeck::Device>(uri, 1, 0, "StreamDeck");

    qmlRegisterUncreatableType<streamdeck::DeviceTypeGadget>(uri, 1, 0, "StreamDeckType",
                                                             "Not creatable as it is an enum type.");
    qmlRegisterUncreatableType<streamdeck::DeviceId>(uri, 1, 0, "StreamDeckDeviceId",
                                                     "Not creatable as it is a gadget.");
}

