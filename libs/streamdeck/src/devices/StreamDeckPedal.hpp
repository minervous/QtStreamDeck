#pragma once

#include "AbstractDeviceBase.hpp"

namespace minervous::streamdeck
{

	class StreamDeckPedal: public AbstractDeviceBase
	{
	public:
		enum
		{
			PID = 0x0086
		};

		StreamDeckPedal() = default;
		~StreamDeckPedal() override = default;

		const Configuration & getConfiguration() const override;
		bool setBrightness(int percentage) override;
		QString getFirmwareVersion() override;

		bool reset() override;
		int readButtonsStatus(QList<bool> & buttonsStates) override;
		bool sendImage(int keyIndex, const QByteArray & imageData) override;
	};

}  // namespace minervous::streamdeck
