#pragma once

#include "AbstractDeviceBase.hpp"

namespace minervous::streamdeck
{

	class StreamDeckMini: public AbstractDeviceBase
	{
	public:
		enum
		{
			PID_MINI = 0x0063,
			PID_MINI_MK2 = 0x0090
		};

		explicit StreamDeckMini(quint16 pid);
		~StreamDeckMini() override = default;

		const Configuration & getConfiguration() const override;
		bool setBrightness(int percentage) override;
		QString getFirmwareVersion() override;

		bool reset() override;
		int readButtonsStatus(QList<bool> & buttonsStates) override;
		bool sendImage(int keyIndex, const QByteArray & imageData) override;

	private:
		Configuration m_configuration;
		StreamDeckMini() = delete;
	};

}  // namespace minervous::streamdeck
