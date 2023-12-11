#pragma once

#include <QtQml/QQmlEngineExtensionPlugin>

namespace minervous::streamdeck::qml
{

	class StreamDeckPlugin: public QQmlEngineExtensionPlugin
	{
		Q_OBJECT
		Q_PLUGIN_METADATA(IID QQmlEngineExtensionInterface_iid)
	};

}  // namespace minervous::streamdeck::qml
