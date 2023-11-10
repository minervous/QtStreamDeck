#include "AbstractDeviceBase.hpp"

using namespace minervous::streamdeck;

AbstractDeviceBase::AbstractDeviceBase()
	: IDevice{}
{}

AbstractDeviceBase::~AbstractDeviceBase()
{
	_hid.close();
}

bool AbstractDeviceBase::open([[maybe_unused]]const QString & serial)
{
	const auto & conf = getConfiguration();

	// [TODO] @MJNIKOFF - commented call with defined serial due to issue in QtUsb - it could not open the devive if serial is defined
	_hid.open(static_cast<quint16>(conf.vid), static_cast<quint16>(conf.pid));//, &serial);

	// Read manufacturer, product, serialNumber here because these methods are not const in Qt::QHidDevice
	_manufacturer = _hid.manufacturer();
	_product = _hid.product();
	_serialNumber = _hid.serialNumber();

	return _hid.isOpen();
}

void AbstractDeviceBase::close()
{
	_hid.close();
}

bool AbstractDeviceBase::isOpen() const
{
	return _hid.isOpen();
}

QString AbstractDeviceBase::manufacturer() const
{
	// Return stored data because corresponding method is not marked as const in Qt::QHidDevice
	return _manufacturer;
}

QString AbstractDeviceBase::serialNumber() const
{
	// Return stored data because corresponding method is not marked as const in Qt::QHidDevice
	return _serialNumber;
}

QString AbstractDeviceBase::product() const
{
	// Return stored data because corresponding method is not marked as const in Qt::QHidDevice
	return _product;
}
