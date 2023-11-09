#pragma once

#include <QtCore/qtmetamacros.h>

#include "StreamDeckLib_global.hpp"
#include "devices/IDevice.hpp"
#include "DeviceId.hpp"

namespace minervous::streamdeck
{

	class STREAMDECKLIB_EXPORT IEmulator
	{
	public:
		IEmulator() = default;
		virtual ~IEmulator() = default;

		virtual IDevice *createInterface() = 0;

		virtual DeviceId deviceId() = 0;
	};

} // namespace

