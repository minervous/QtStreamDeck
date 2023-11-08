#pragma once

#include "AbstractDeviceBase.hpp"

namespace minervous::streamdeck
{

	class StreamDeckOriginalV2: public AbstractDeviceBase
	{
	public:
		enum
		{
			PID_ORIGINAL_V2 = 0x006d,
			PID_MK2 = 0x0080
		};

		explicit StreamDeckOriginalV2(quint16 pid);
		~StreamDeckOriginalV2() override = default;

		const Configuration & getConfiguration() const override;
		bool setBrightness(int percentage) override;
		QString getFirmwareVersion() override;

		bool reset() override;
		int readButtonsStatus(QList<bool> & buttonsStates) override;
		bool sendImage(int keyIndex, const QByteArray & imageData) override;

	private:
		Configuration _configuration;
		StreamDeckOriginalV2() = delete;
	};

}  // namespace minervous::streamdeck
