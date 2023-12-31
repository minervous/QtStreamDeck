// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include "AbstractDeviceBase.hpp"

namespace minervous::streamdeck
{

	class StreamDeckMini: public AbstractDeviceBase
	{
	public:
		enum
		{
			PID_Mini = 0x0063,
			PID_MiniMK2 = 0x0090
		};

		explicit StreamDeckMini(quint16 pid);
		~StreamDeckMini() override = default;

		const Configuration & getConfiguration() const override;
		bool setBrightness(int percentage) override;
		QString getFirmwareVersion() override;

		bool reset() override;
		int readButtonsStatus(QList<bool> & buttonsStates) override;
		bool sendImage(int keyIndex, const QByteArray & imageData) override;

		static Configuration createConfiguration(quint16 pid);

	private:
		Configuration _configuration;
		StreamDeckMini() = delete;
	};

}  // namespace minervous::streamdeck
