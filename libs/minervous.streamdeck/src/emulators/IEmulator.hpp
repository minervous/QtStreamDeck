// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include <QtCore/qtmetamacros.h>

#include "DeviceId.hpp"
#include "StreamDeckLib_global.hpp"
#include "devices/IDevice.hpp"

namespace minervous::streamdeck
{

	class STREAMDECKLIB_EXPORT IEmulator
	{
	public:
		IEmulator() = default;
		virtual ~IEmulator() = default;

		virtual IDevice * createInterface() = 0;

		virtual DeviceId deviceId() = 0;
	};

}  // namespace minervous::streamdeck
