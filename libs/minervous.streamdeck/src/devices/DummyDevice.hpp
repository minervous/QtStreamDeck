// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include "IDevice.hpp"
#include "StreamDeckLib_global.hpp"

namespace minervous::streamdeck
{

	class STREAMDECKLIB_EXPORT DummyDevice: public IDevice
	{
	public:
		DummyDevice() = default;
		~DummyDevice() override = default;

		bool open([[maybe_unused]] const QString & serial) override { return false; }

		void close() override {}

		bool isOpen() const override { return false; }

		QString manufacturer() const override { return {}; }

		QString serialNumber() const override { return {}; }

		QString product() const override { return {}; }

		const Configuration & getConfiguration() const override
		{
			static Configuration configuration;
			return configuration;
		}

		bool setBrightness([[maybe_unused]] int percentage) override { return false; }

		QString getFirmwareVersion() override { return {}; }

		bool reset() override { return false; }

		int readButtonsStatus([[maybe_unused]] QList<bool> & buttonsStates) override { return 0; }

		bool sendImage([[maybe_unused]] int keyIndex, [[maybe_unused]] const QByteArray & imageData) override
		{
			return false;
		}
	};

}  // namespace minervous::streamdeck
