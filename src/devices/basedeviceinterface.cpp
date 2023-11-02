#include "basedeviceinterface.h"

using namespace streamdeck;

BaseDeviceInterface::BaseDeviceInterface()
	: DeviceInterface()
	, m_hid()
{}

BaseDeviceInterface::~BaseDeviceInterface()
{
	m_hid.close();
}

bool BaseDeviceInterface::open(const QString &serial)
{
	auto &conf {getConfiguration()};
	m_hid.open(static_cast<quint16>(conf.vid), static_cast<quint16>(conf.pid), &serial);

	// Read manufacturer, product, serialNumber here because these methods are not const in Qt::QHidDevice
	m_manufacturer = m_hid.manufacturer();
	m_product = m_hid.product();
	m_serialNumber = m_hid.serialNumber();

	return m_hid.isOpen();
}

void BaseDeviceInterface::close()
{
	m_hid.close();
}

bool BaseDeviceInterface::isOpen() const
{
	return m_hid.isOpen();
}

QString BaseDeviceInterface::manufacturer() const
{
	// Return stored data because corresponding method is not marked as const in Qt::QHidDevice
	return m_manufacturer;
}

QString BaseDeviceInterface::serialNumber() const
{
	// Return stored data because corresponding method is not marked as const in Qt::QHidDevice
	return m_serialNumber;
}

QString BaseDeviceInterface::product() const
{
	// Return stored data because corresponding method is not marked as const in Qt::QHidDevice
	return m_product;
}
