#pragma once

#include "AbstractDeviceBase.hpp"

namespace minervous::streamdeck
{

	class StreamDeckOriginal: public AbstractDeviceBase
	{
	public:
		static const auto PID = 0x0060;

		StreamDeckOriginal() = default;
		~StreamDeckOriginal() override = default;

		const Configuration & getConfiguration() const override;
		bool setBrightness(int percentage) override;
		QString getFirmwareVersion() override;

		bool reset() override;
		int readButtonsStatus(QList<bool> & buttonsStates) override;
		bool sendImage(int keyIndex, const QByteArray & imageData) override;

		static Configuration createConfiguration();
	};

}  // namespace minervous::streamdeck
