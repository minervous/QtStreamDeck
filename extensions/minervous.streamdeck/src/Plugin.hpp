// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include <QtQml/QQmlEngineExtensionPlugin>

namespace minervous::streamdeck::qml
{

	class Plugin: public QQmlEngineExtensionPlugin
	{
		Q_OBJECT
		Q_PLUGIN_METADATA(IID QQmlEngineExtensionInterface_iid)
	};

}  // namespace minervous::streamdeck::qml
