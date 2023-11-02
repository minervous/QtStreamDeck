#ifndef BASEDEVICEINTERFACE_H
#define BASEDEVICEINTERFACE_H

#include "deviceinterface.h"
#include <QtUsb/QHidDevice>

namespace streamdeck {

class STREAMDECKLIB_EXPORT BaseDeviceInterface: public DeviceInterface
{
public:
    BaseDeviceInterface();
    ~BaseDeviceInterface() override;

	bool open(const QString &serial) override;
	void close() override;
	bool isOpen() const override;
	QString manufacturer() const override;
	QString serialNumber() const override;
	QString product() const override;

protected:
	QHidDevice m_hid;
	QString m_manufacturer;
	QString m_product;
	QString m_serialNumber;
};

} // namespace

#endif // BASEDEVICEINTERFACE_H

