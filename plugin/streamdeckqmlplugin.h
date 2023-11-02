#ifndef STREAMDECKQMLPLUGIN_H
#define STREAMDECKQMLPLUGIN_H

//#include "QtQml/qqmlregistration.h"
//#include <QtQml/qqmlextensionplugin.h>
//#include <QtQml/QQmlEngine>

//extern void qml_register_types_StreamDeckQml();
//Q_GHS_KEEP_REFERENCE(qml_register_types_StreamDeckQml)

//class StreamDeckQmlPlugin : public QQmlExtensionPlugin //QQmlEngineExtensionPlugin
//{
//    Q_OBJECT
//    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

//public:
//    StreamDeckQmlPlugin(QObject *parent = nullptr);
//    void registerTypes(const char *uri) override;
//};

#include <device.h>
#include <devicemanager.h>

struct StreamDeckQml
{
    Q_GADGET
    QML_FOREIGN(streamdeck::Device)
    QML_NAMED_ELEMENT(StreamDeck)
    //QML_EXTENDED(streamdeck::Device)
};

struct StreamDeckTypeQml
{
    Q_GADGET
    QML_FOREIGN(streamdeck::DeviceTypeGadget)
    QML_NAMED_ELEMENT(StreamDeckType)
};

struct StreamDeckManagerQml
{
    Q_GADGET
    QML_FOREIGN(streamdeck::DeviceManager)
    QML_SINGLETON
    QML_NAMED_ELEMENT(StreamDeckManager)
};

#endif // STREAMDECKQMLPLUGIN_H
