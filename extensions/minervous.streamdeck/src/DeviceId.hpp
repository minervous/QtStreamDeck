// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include <QtQml/QQmlEngine>

#include "minervous/streamdeck/DeviceId.hpp"

namespace minervous::streamdeck::qml
{
	struct DeviceId
	{
		Q_GADGET
		QML_FOREIGN(minervous::streamdeck::DeviceId)
		QML_VALUE_TYPE(deviceId)
	};

}  // namespace minervous::streamdeck::qml
