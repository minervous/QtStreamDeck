#pragma once

#include <QtUsb/QHidDevice>

#include "IDevice.hpp"

namespace minervous::streamdeck
{

	class STREAMDECKLIB_EXPORT AbstractDeviceBase: public IDevice
	{
	public:
		AbstractDeviceBase();
		~AbstractDeviceBase() override;

		bool open(const QString & serial) override;
		void close() override;
		bool isOpen() const override;
		QString manufacturer() const override;
		QString serialNumber() const override;
		QString product() const override;

	protected:
		QHidDevice _hid;
		QString _manufacturer;
		QString _product;
		QString _serialNumber;
	};

}  // namespace minervous::streamdeck
