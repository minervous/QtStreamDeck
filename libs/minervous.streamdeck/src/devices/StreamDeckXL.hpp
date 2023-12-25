// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include "AbstractDeviceBase.hpp"

namespace minervous::streamdeck
{

	class StreamDeckXL: public AbstractDeviceBase
	{
	public:
		enum
		{
			PID_XL = 0x006c,
			PID_XLV2 = 0x008f
		};

		explicit StreamDeckXL(quint16 pid);
		~StreamDeckXL() override = default;

		const Configuration & getConfiguration() const override;
		bool setBrightness(int percentage) override;
		QString getFirmwareVersion() override;

		bool reset() override;
		int readButtonsStatus(QList<bool> & buttonsStates) override;
		bool sendImage(int keyIndex, const QByteArray & imageData) override;

		static Configuration createConfiguration(quint16 pid);

	private:
		Configuration _configuration;
		StreamDeckXL() = delete;
	};

}  // namespace minervous::streamdeck
