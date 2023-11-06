#pragma once

#include <QtQml/QQmlEngineExtensionPlugin>

namespace minervous::streamdeck
{

	class StreamDeckPlugin: public QQmlEngineExtensionPlugin
	{
		Q_OBJECT
		Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

	public:
		void registerTypes(const char * uri);
	};

}  // namespace minervous::streamdeck
