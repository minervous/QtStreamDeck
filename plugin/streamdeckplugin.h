#ifndef STREAMDECKQMLPLUGIN_H
#define STREAMDECKQMLPLUGIN_H

#include <QQmlEngineExtensionPlugin>

class StreamDeckPlugin : public QQmlEngineExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri);
};

#endif // STREAMDECKQMLPLUGIN_H
