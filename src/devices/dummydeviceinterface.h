#ifndef DUMMYDEVICEINTERFACE_H
#define DUMMYDEVICEINTERFACE_H

#include "../StreamDeckLib_global.h"
#include "deviceinterface.h"

namespace streamdeck {

class STREAMDECKLIB_EXPORT DummyDeviceInterface: public DeviceInterface
{
public:
    DummyDeviceInterface() = default;
    ~DummyDeviceInterface() override = default;

	bool open(const QString &serial) override
	{
		return false;
	}
	void close() override
	{}
	bool isOpen() const override
	{
		return false;
	}
	QString manufacturer() const override
	{
		return {};
	}
	QString serialNumber() const override
	{
		return {};
	}
	QString product() const override
	{
		return {};
	}

	virtual const Configuration & getConfiguration() const override
	{
		static Configuration configuration {};
		return configuration;
	}
	virtual bool setBrightness(int procentage) override
	{
		return false;
	}
	virtual QString getFirmwareVersion() override
	{
		return {};
	}

	virtual bool reset() override
	{
		return false;
	}
	virtual int readButtonsStatus(QList<bool> &buttonsStates) override
	{
		return 0;
	}

	virtual bool sendImage(int keyIndex, const QByteArray& imageData) override
	{
		return false;
	}
};

} // namespace

#endif // DUMMYDEVICEINTERFACE_H

