#include "Device.hpp"

#include <QtCore/QDebug>
#include <QtGui/QImage>
#include <QtUsb/QHidDevice>
#include <QtUsb/QUsb>
#include <QtCore/qmetaobject.h>

#include "DeviceId.hpp"
#include "DeviceManager.hpp"
#include "devices/DummyDevice.hpp"
#include "devices/IDevice.hpp"

using namespace minervous::streamdeck;

struct Device::Impl
{
	explicit Impl(Device & device)
		: _device{device}
		, _interface{new DummyDevice}
	{}

	int readButtonsStatus();
	void setSerialNumber(const QString & number);
	void setValid(bool valid);
	void onReadTimeot();
	bool checkOpenOnConnect();

	bool doOpen();
	void doClose();

	void reinit();

	Device & _device;
	bool _initialized = false;
	bool _openOnConnect = true;
	QString _serialNumber;
	QString _firmwareNumber;
	int _brightness = 70;
	IDevice::Configuration _configuration;
	QScopedPointer<QTimer> _timer;
	bool _valid = false;
	QList<bool> _buttonsState;
	bool _connected = false;
	DeviceType _deviceType = DeviceType::Any;
	DeviceType _realDeviceType = DeviceType::Unknown;

	QScopedPointer<IDevice> _interface;
};

bool Device::Impl::checkOpenOnConnect()
{
	qInfo() << "AAAAAA" << _connected << _openOnConnect << !_interface->isOpen();
	return _connected && _openOnConnect && !_interface->isOpen() && doOpen();
}

bool Device::Impl::doOpen()
{
	bool result = _interface->open(_serialNumber);
	if (result)
	{
		qInfo() << "Open";
		result = _interface->setBrightness(_brightness);
		if (_timer.isNull())
		{
			_timer.reset(new QTimer(&_device));
			_timer->setInterval(40);
			_device.connect(_timer.data(), &QTimer::timeout, &_device, [this]() { onReadTimeot(); });
			_timer->start();
		}
	}
	else
	{
		qInfo() << "Could not open";
	}
	setValid(result);
	return result;
}

void Device::Impl::doClose()
{
	if (!_timer.isNull())
	{
		_timer->stop();
		disconnect(_timer.data(), nullptr, &_device, nullptr);
		qInfo() << "close: timer reset";
		_timer.reset();
	}
	qInfo() << "close: call interface::close";
	_interface->close();
}

void Device::Impl::reinit()
{
	DeviceType type{_deviceType};
	QString serial{_serialNumber};

	if (type == DeviceType::Any)
	{
		const auto devices = DeviceManager::instance()->devices();
		for (const auto & id: devices)
		{
			if (id.type != DeviceType::Unknown)
			{
				type = id.type;
				serial = id.serialNumber;
				break;
			}
		}
	}

	_interface.reset(DeviceManager::instance()->createInterface(DeviceId(type, serial)));

	if (type == DeviceType::Any)
	{
		type = DeviceType::Unknown;
	}

	bool deviceChanged = false;
	if (_realDeviceType != type)
	{
		_realDeviceType = type;
		deviceChanged = true;
	}

	qInfo() << "Device init: expected " << _deviceType << "real" << type;

	_configuration = _interface->getConfiguration();
	_buttonsState.clear();
	_buttonsState.fill(false, _configuration.keyColumns * _configuration.keyRows);

	bool connected = DeviceManager::instance()->devices().contains(DeviceId(type, serial));
	bool connectedChanged = false;
	if (_connected != connected)
	{
		_connected = connected;
		connectedChanged = true;
	}

	bool result = _interface->open(serial);
	if (result)
	{
		_serialNumber = _interface->serialNumber();
		_firmwareNumber = _interface->getFirmwareVersion();
		_interface->close();
	}
	setValid(result);

	auto openedOnConnect = checkOpenOnConnect();

	if (connectedChanged) emit _device.connectedChanged();
	if (deviceChanged) emit _device.deviceTypeChanged();
	if (serial != _serialNumber) emit _device.serialNumberChanged();
	emit _device.configurationUpdated();
	if (openedOnConnect) emit _device.isOpenChanged();
}

void Device::Impl::setSerialNumber(const QString & number)
{
	if (number != _serialNumber)
	{
		_serialNumber = number;
		emit _device.serialNumberChanged();
	}
}

int Device::Impl::readButtonsStatus()
{
	//	if (_hid.isNull() || !_hid->isOpen()) {
	//		setValid(false);
	//		return -1;
	//	}
	QList<bool> data;
	data.fill(false, _configuration.keyColumns * _configuration.keyRows);
	auto result = _interface->readButtonsStatus(data);
	if (result > 0)
	{
		bool changed{false};
		for (auto i = 0; i < std::min(data.size(), _buttonsState.size()); ++i)
		{
			if (data[i] != _buttonsState[i])
			{
				_buttonsState[i] = data[i];
				changed = true;
				data[i] ? emit _device.pressed(i) : emit _device.released(i);
			}
		}
		if (changed)
		{
			emit _device.buttonsStateChanged();
		}
	}
	return result;
}

void Device::Impl::setValid(bool valid)
{
	if (valid != _valid)
	{
		_valid = valid;
		emit _device.validChanged();
	}
}

void Device::Impl::onReadTimeot()
{
	constexpr int maxReadIterations{3};
	int cnt{maxReadIterations};
	while (readButtonsStatus() > 0 && cnt-- > 0)
	{}
}

Device::Device(QObject * parent)
	: QObject{parent}
	, _pImpl{new Impl{*this}}
{

}

Device::~Device()
{
	close();
}

int Device::keyColumns() const
{
	return _pImpl->_configuration.keyColumns;
}

int Device::keyRows() const
{
	return _pImpl->_configuration.keyRows;
}

int Device::keyCount() const
{
	return _pImpl->_configuration.keyColumns * _pImpl->_configuration.keyRows;
}

bool Device::hasDisplay() const
{
	return _pImpl->_configuration.hasDisplay;
}

QString Device::modelName() const
{
	return _pImpl->_interface->product();
}

QString Device::serialNumber() const
{
	return _pImpl->_serialNumber;
}

QString Device::firmwareVersion() const
{
	return _pImpl->_firmwareNumber;
}

QString Device::manufacturer() const
{
	return _pImpl->_interface->manufacturer();
}

bool Device::open()
{
	if (!isOpen())
	{
		if (_pImpl->doOpen())
		{
			emit isOpenChanged();
		}
	}
	else
	{
		qInfo() << "Could not open: already open";
	}
	return isOpen();
}

bool Device::isOpen()
{
	return _pImpl->_interface->isOpen();
}

void Device::close()
{
	if (isOpen())
	{
		_pImpl->doClose();
		emit isOpenChanged();
	}
}

void Device::reset()
{
	_pImpl->setValid(_pImpl->_interface->reset());
}

void Device::setBrightness(int percentage)
{
	if (percentage < 0 || percentage > 100)
	{
		qWarning() << "Could not sent brightness. The value" << percentage << "is out of range";
		return;
	}

	if (percentage != _pImpl->_brightness)
	{
		_pImpl->_brightness = percentage;
		if (isOpen())
		{
			bool result = _pImpl->_interface->setBrightness(_pImpl->_brightness);
			_pImpl->setValid(result);
		}
		emit brightnessChanged();
	}
}

int Device::brightness()
{
	return _pImpl->_brightness;
}

void Device::setSerialNumber(const QString & number)
{
	if (!connected())
	{
		_pImpl->setSerialNumber(number);
	} else {
		qWarning() << "Could not change serialNumber of already connected device";
	}
}

void Device::init()
{
	if (_pImpl->_initialized) return;

	_pImpl->_initialized = true;

	connect(
		DeviceManager::instance(),
		&DeviceManager::removed,
		this,
		[=](auto id)
		{
			qInfo() << "Device removed:" << id << _pImpl->_configuration.pid << _pImpl->_configuration.vid
					<< "serial " << _pImpl->_serialNumber;
			if (_pImpl->_connected &&
				id == DeviceId(_pImpl->_realDeviceType, _pImpl->_serialNumber))
			{
				close();
				_pImpl->_connected = false;
				qInfo() << "Device disconnected";
				emit connectedChanged();
				_pImpl->setValid(false);
				if (_pImpl->_deviceType == DeviceType::Any)
				{
					_pImpl->reinit();
				}
			}
		}
	);

	connect(
		DeviceManager::instance(),
		&DeviceManager::inserted,
		this,
		[=](auto id)
		{
			if (!_pImpl->_connected)
			{
				qInfo() << "Device inserted:" << id << "expected" << _pImpl->_deviceType << "real"
						<< _pImpl->_realDeviceType << _pImpl->_configuration.pid << _pImpl->_configuration.vid
						<< "serial " << _pImpl->_serialNumber;
				if (id.type == DeviceManager::convert(_pImpl->_configuration.vid, _pImpl->_configuration.pid)
					&& (_pImpl->_serialNumber.isEmpty() || _pImpl->_serialNumber == id.serialNumber))
				{
					_pImpl->_connected = true;
					qInfo() << "Device reconnected";
					auto opened = _pImpl->checkOpenOnConnect();
					emit connectedChanged();
					if (opened) {
						emit isOpenChanged();
					}
				}
				else if (_pImpl->_deviceType == DeviceType::Any)
				{
					_pImpl->reinit();
				}
			}
		}
	);

	_pImpl->reinit();

}

QList<bool> Device::buttonsState() const
{
	return _pImpl->_buttonsState;
}

bool Device::openOnConnect() const
{
	return _pImpl->_openOnConnect;
}
void Device::setOpenOnConnect(bool value)
{
	if (_pImpl->_openOnConnect != value)
	{
		_pImpl->_openOnConnect = value;
		emit openOnConnectChanged();
	}
}

bool Device::valid() const
{
	return _pImpl->_valid;
}

bool Device::connected() const
{
	return _pImpl->_connected;
}

DeviceType Device::expectedDeviceType() const
{
	return _pImpl->_deviceType;
}

DeviceType Device::deviceType() const
{
	return _pImpl->_realDeviceType;
}

void Device::setExpectedDeviceType(DeviceType deviceType)
{
	if (deviceType != _pImpl->_deviceType)
	{
		_pImpl->_deviceType = deviceType;
		emit expectedDeviceTypeChanged();

		if (_pImpl->_initialized)
		{
			if (_pImpl->_realDeviceType != deviceType)
			{
				_pImpl->reinit();
			}
		}
	}
}

void Device::sendImage(int keyIndex, QUrl source)
{
	if (!isOpen())
	{
		qWarning() << "Device::sendImage the device is not open";
		return;
	}

	if (!_pImpl->_configuration.hasDisplay)
	{
		qWarning() << "Device::sendImage the device does not have displays on buttons";
		return;
	}

	if (keyIndex >= keyCount())
	{
		qWarning() << "Device::sendImage keyIndex" << keyIndex << "is out of range!";
		return;
	}

	QString filePath = source.scheme() == "qrc" ? source.path().prepend(":") : source.toLocalFile();

	QFile file{filePath};
	if (!file.exists())
	{
		qWarning() << "Device::sendImage file" << source.fileName() << "is not exist!";
	}
	else
	{
		QImage imageOriginal{file.fileName()};
		QBuffer bf;
		QTransform rotating;
		rotating.rotate(_pImpl->_configuration.imageRotation);
		imageOriginal.convertedTo(QImage::Format_RGB888, Qt::ColorOnly)
			.scaled(
				_pImpl->_configuration.imageWidth,
				_pImpl->_configuration.imageHeight,
				Qt::IgnoreAspectRatio,
				Qt::SmoothTransformation
			)
			.mirrored(_pImpl->_configuration.imageHorizontalFlip, _pImpl->_configuration.imageVerticalFlip)
			.transformed(rotating)
			.save(&bf, _pImpl->_configuration.imageFormatAsString(), 100);
		_pImpl->_interface->sendImage(keyIndex, bf.data());
	}
}

QString Device::deviceTypeToString(DeviceType value)
{
	const QString notValidValue = QStringLiteral("Not valid value");
	QString enumToString{QMetaEnum::fromType<minervous::streamdeck::Device::DeviceType>().valueToKey(value)};
	return (enumToString.isEmpty() ? notValidValue : enumToString);
}
