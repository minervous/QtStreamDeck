#include "AbstractDeviceBase.hpp"

using namespace minervous::streamdeck;

AbstractDeviceBase::AbstractDeviceBase()
	: IDevice()
	, m_hid()
{}

AbstractDeviceBase::~AbstractDeviceBase()
{
	m_hid.close();
}

bool AbstractDeviceBase::open(const QString & serial)
{
	auto & conf{getConfiguration()};
	m_hid.open(static_cast<quint16>(conf.vid), static_cast<quint16>(conf.pid), &serial);

	// Read manufacturer, product, serialNumber here because these methods are not const in Qt::QHidDevice
	m_manufacturer = m_hid.manufacturer();
	m_product = m_hid.product();
	m_serialNumber = m_hid.serialNumber();

	return m_hid.isOpen();
}

void AbstractDeviceBase::close()
{
	m_hid.close();
}

bool AbstractDeviceBase::isOpen() const
{
	return m_hid.isOpen();
}

QString AbstractDeviceBase::manufacturer() const
{
	// Return stored data because corresponding method is not marked as const in Qt::QHidDevice
	return m_manufacturer;
}

QString AbstractDeviceBase::serialNumber() const
{
	// Return stored data because corresponding method is not marked as const in Qt::QHidDevice
	return m_serialNumber;
}

QString AbstractDeviceBase::product() const
{
	// Return stored data because corresponding method is not marked as const in Qt::QHidDevice
	return m_product;
}
