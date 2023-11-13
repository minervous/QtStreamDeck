#pragma once

#include "minervous/streamdeck/DeviceId.hpp"
#include <QtQml/QQmlEngine>

namespace minervous::streamdeck
{
	struct QmlStreamDeckDeviceId
	{
		Q_GADGET
		QML_FOREIGN(minervous::streamdeck::DeviceId)
		QML_NAMED_ELEMENT(deviceId)
	};

}  // namespace minervous::streamdeck
