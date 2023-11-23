#pragma once

#include <QtQml/QQmlEngine>

#include "minervous/streamdeck/DeviceId.hpp"

namespace minervous::streamdeck
{
	struct QmlStreamDeckDeviceId
	{
		Q_GADGET
		QML_FOREIGN(minervous::streamdeck::DeviceId)
		QML_NAMED_ELEMENT(deviceId)
	};

}  // namespace minervous::streamdeck
