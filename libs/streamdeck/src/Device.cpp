#include "Device.hpp"

#include <QtCore/QDebug>
#include <QtGui/QImage>
#include <QtUsb/QHidDevice>
#include <QtUsb/QUsb>

#include "DeviceId.hpp"
#include "DeviceManager.hpp"
#include "DeviceType.hpp"
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

	void setInterface(DeviceType type);

	Device & _device;
	QString _serialNumber;
	QString _firmwareNumber;
	int _brightness = 70;
	IDevice::Configuration _configuration;
	QScopedPointer<QTimer> _timer;
	bool _valid = false;
	QList<bool> _buttonsState;
	bool _connected = false;
	DeviceType _deviceType = DeviceType::STREAMDECK_ANY;
	DeviceType _realDeviceType = DeviceType::UNKNOWN_DEVICE;

	QScopedPointer<IDevice> _interface;
};

void Device::Impl::setInterface(DeviceType type)
{
	_interface.reset(DeviceManager::instance()->createInterface(DeviceId(type)));
	if (type == DeviceType::UNKNOWN_DEVICE
		|| type == DeviceType::STREAMDECK_ANY)
	{
		_realDeviceType = DeviceType::UNKNOWN_DEVICE;
	} else {
		_realDeviceType = type;
	}
}

Device::Device(QObject * parent)
	: QObject{parent}
	, _pImpl{new Impl{*this}}
{
	connect(
		DeviceManager::instance(),
		&DeviceManager::removed,
		this,
		[=](auto id)
		{
			qInfo() << "Device removed:" << id << _pImpl->_configuration.pid << _pImpl->_configuration.vid
					<< "serial " << _pImpl->_serialNumber;
			if (id.type == DeviceManager::convert(_pImpl->_configuration.vid, _pImpl->_configuration.pid)
				/* && (_serialNumber.isEmpty() || _serialNumber == id.serialNumber)*/)
			{
				close();
				_pImpl->_connected = false;
				qInfo() << "Device disconnected";
				emit connectedChanged();
				_pImpl->setValid(false);
				if (_pImpl->_deviceType == DeviceType::STREAMDECK_ANY)
				{
					init();
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
			qInfo() << "Device inserted:" << id << "expected" << _pImpl->_deviceType << "real"
					<< _pImpl->_realDeviceType << _pImpl->_configuration.pid << _pImpl->_configuration.vid
					<< "serial " << _pImpl->_serialNumber;
			if (id.type == DeviceManager::convert(_pImpl->_configuration.vid, _pImpl->_configuration.pid)
				/*&& (_serialNumber.isEmpty() || _serialNumber == id.serialNumber)*/)
			{
				_pImpl->_connected = true;
				qInfo() << "Device reconnected";
				emit connectedChanged();
			}
			else if (_pImpl->_deviceType == DeviceType::STREAMDECK_ANY)
			{
				init();
			}
		}
	);
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
		bool result = _pImpl->_interface->open(QString{});  // _pImpl->_serialNumber);
		if (result)
		{
			qInfo() << "Open";
			result = _pImpl->_interface->setBrightness(_pImpl->_brightness);
			if (_pImpl->_timer.isNull())
			{
				_pImpl->_timer.reset(new QTimer(this));
				_pImpl->_timer->setInterval(40);
				connect(_pImpl->_timer.data(), &QTimer::timeout, this, [this]() { _pImpl->onReadTimeot(); });
				_pImpl->_timer->start();
			}
			emit isOpenChanged();
		}
		else
		{
			qInfo() << "Could not open";
		}
		_pImpl->setValid(result);
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
	if (!_pImpl->_timer.isNull())
	{
		_pImpl->_timer->stop();
		disconnect(_pImpl->_timer.data(), nullptr, this, nullptr);
		qInfo() << "close: timer reset";
		_pImpl->_timer.reset();
	}
	qInfo() << "close: call interface::close";
	_pImpl->_interface->close();
	emit isOpenChanged();
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
	if (!isOpen())
	{
		_pImpl->setSerialNumber(number);
	}
}

void Device::Impl::setSerialNumber(const QString & number)
{
	if (number != _serialNumber)
	{
		_serialNumber = number;
		emit _device.serialNumberChanged();
	}
}

void Device::init()
{
	DeviceType type{_pImpl->_deviceType};
	QString serial{_pImpl->_serialNumber};

	if (type == DeviceType::STREAMDECK_ANY)
	{
		const auto devices = DeviceManager::instance()->devices();
		for (const auto & id: devices)
		{
			if (id.type != DeviceType::UNKNOWN_DEVICE)
			{
				type = id.type;
				serial = id.serialNumber;
				break;
			}
		}
	}

	_pImpl->setInterface(type);

	qInfo() << "init: expected " << _pImpl->_deviceType << "real" << _pImpl->_realDeviceType;

	_pImpl->_configuration = _pImpl->_interface->getConfiguration();
	_pImpl->_buttonsState.clear();
	_pImpl->_buttonsState.fill(false, _pImpl->_configuration.keyColumns * _pImpl->_configuration.keyRows);

	bool result = _pImpl->_interface->open(serial);
	if (result)
	{
		QString serialHid = _pImpl->_interface->serialNumber();
		_pImpl->_firmwareNumber = _pImpl->_interface->getFirmwareVersion();
		_pImpl->_interface->close();
		_pImpl->setSerialNumber(serialHid);
		emit firmwareVersionChanged();
	}
	_pImpl->setValid(result);

	bool connected = DeviceManager::instance()->devices().contains(DeviceId(_pImpl->_realDeviceType));  //, serial));
	if (_pImpl->_connected != connected)
	{
		_pImpl->_connected = connected;
		emit connectedChanged();
	}
	if (_pImpl->_realDeviceType != _pImpl->_deviceType)
	{
		emit connectedDeviceTypeChanged();
	}
	emit configurationUpdated();
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

QList<bool> Device::buttonsState() const
{
	return _pImpl->_buttonsState;
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

DeviceType Device::connectedDeviceType() const
{
	return _pImpl->_realDeviceType;
}

void Device::setExpectedDeviceType(DeviceType deviceType)
{
	if (deviceType != _pImpl->_deviceType)
	{
		_pImpl->_deviceType = deviceType;
		if (_pImpl->_realDeviceType != deviceType)
		{
			init();
		}
		emit expectedDeviceTypeChanged();
	}
}

void Device::setImageUrl(int index, QUrl url)
{
	if (!isOpen())
	{
		qWarning() << "Device::setImageUrl the device is not open";
		return;
	}

	if (!_pImpl->_configuration.hasDisplay)
	{
		qWarning() << "Device::setImageUrl the device does not have displays on buttons";
		return;
	}

	if (index >= keyCount())
	{
		qWarning() << "Device::setImageUrl index" << index << "is out of range!";
		return;
	}

	QString filePath = url.scheme() == "qrc" ? url.path().prepend(":") : url.toLocalFile();

	QFile file{filePath};
	if (!file.exists())
	{
		qWarning() << "Device::setImageUrl file" << url.fileName() << "is not exist!";
	}
	else
	{
		QImage imageOriginal{file.fileName()};
		QBuffer bf;
		QTransform rotating;
		rotating.rotate(_pImpl->_configuration.imageRotation);
		imageOriginal
			.scaled(
				_pImpl->_configuration.imageWidth,
				_pImpl->_configuration.imageHeight,
				Qt::KeepAspectRatio,
				Qt::SmoothTransformation
			)
			.mirrored(_pImpl->_configuration.imageHorizontalFlip, _pImpl->_configuration.imageVerticalFlip)
			.transformed(rotating)
			.save(&bf, _pImpl->_configuration.imageFormatAsString());
		_pImpl->_interface->sendImage(index, bf.data());
	}
}
