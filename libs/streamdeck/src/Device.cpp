#include "Device.hpp"

#include <QtCore/QDebug>
#include <QtCore/QRegularExpression>
#include <QtGui/QImage>
#include <QtUsb/QHidDevice>
#include <QtUsb/QUsb>

#include <QtCore/qmetaobject.h>

#include "BaseKeyEntry.hpp"
#include "DeviceId.hpp"
#include "DeviceManager.hpp"
#include "devices/DummyDevice.hpp"
#include "devices/IDevice.hpp"

using namespace minervous::streamdeck;

struct Device::Impl
{
	explicit Impl(Device & device)
		: _device{device}
	{}

	int readButtonsStatus();
	void setSerialNumber(const QString & number);
	void setValid(bool valid);
	void onReadTimeot();
	bool checkOpenOnConnect();

	bool doOpen();
	void doClose();

	void reinit();

	bool validKey(int index) const;
	void updateKeyImage(int index, const BaseKeyEntry * entry = nullptr);
	void applyModel(KeyModel * model);
	void updateAllKeysFromModel();

	void setKeyPressed(BaseKeyEntry * entry, bool state, bool initializationOnly = false) const;

	Device & _device;
	bool _initialized = false;
	bool _openOnConnect = true;
	QString _expectedSerialNumber;
	QString _serialNumber;
	QString _firmwareNumber;
	int _brightness = 70;
	IDevice::Configuration _configuration;
	QScopedPointer<QTimer> _timer;
	bool _valid = false;
	QList<bool> _buttonsState;
	bool _connected = false;
	DeviceType _expectedDeviceType = DeviceType::Any;
	DeviceType _deviceType = DeviceType::Unknown;

	QPointer<KeyModel> _modelProperty;
	QPointer<KeyModel> _finalKeyModel;

	QScopedPointer<IDevice> _interface{new DummyDevice};
};

bool Device::Impl::checkOpenOnConnect()
{
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
	_interface->reset();
	qInfo() << "close: call interface::close";
	_interface->close();
}

void Device::Impl::reinit()
{
	DeviceType type = DeviceType::Unknown;
	QString serial = _expectedSerialNumber;
	bool connected = false;

	const auto devices = DeviceManager::instance()->devices();
	for (const auto & id: devices)
	{
		if (id.type != DeviceType::Unknown)
		{
			if ((_expectedDeviceType == DeviceType::Any) ||
				(_expectedDeviceType == id.type && (serial.isEmpty() || serial == id.serialNumber)))
			{
				type = id.type;
				serial = id.serialNumber;
				connected = true;
				break;
			}
		}
	}

	_interface.reset(DeviceManager::instance()->createInterface(DeviceId(type, serial)));

	bool deviceChanged = false;
	if (_deviceType != type)
	{
		_deviceType = type;
		deviceChanged = true;
	}

	qInfo() << "Device init: expected- " << _expectedDeviceType << _serialNumber << ", real-" << type << serial;

	_configuration = _interface->getConfiguration();
	_buttonsState.clear();
	_buttonsState.fill(false, _configuration.keyColumns * _configuration.keyRows);

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

	if (connectedChanged)
		emit _device.connectedChanged();
	if (deviceChanged)
		emit _device.deviceTypeChanged();
	if (serial != _serialNumber)
		emit _device.serialNumberChanged();

	emit _device.configurationUpdated();

	if (openedOnConnect)
		emit _device.isOpenChanged();
}

void Device::Impl::setSerialNumber(const QString & number)
{
	if (number != _expectedSerialNumber)
	{
		_expectedSerialNumber = number;
		emit _device.serialNumberChanged();
	}
}

int Device::Impl::readButtonsStatus()
{
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

bool Device::Impl::validKey(int index) const
{
	return index >= 0 && index < _configuration.keyColumns * _configuration.keyRows && _interface->isOpen();
}

void Device::Impl::updateKeyImage(int index, const BaseKeyEntry * entry)
{
	if (validKey(index))
	{
		if (entry)
		{
			if (entry->image().isNull())
			{
				auto url = entry->imageSource();
				_device.sendImage(index, url.isEmpty() ? emptyImageSource() : url);
			}
			else
			{
				_device.sendImage(index, entry->image());
			}
		}
		else
		{
			_device.sendImage(index, emptyImageSource());
		}
	}
}

void Device::Impl::updateAllKeysFromModel()
{
	if (_finalKeyModel)
	{
		auto & model = *_finalKeyModel.data();
		if (_interface->isOpen())
		{
			auto index(0);
			for (auto max(std::min<qsizetype>(model.count(), _configuration.keyColumns * _configuration.keyRows));
				 index < max;
				 ++index)
			{
				setKeyPressed(model[index], _buttonsState[index], true);
				updateKeyImage(index, model[index]);
			}
			// In case when _finalKeyModel->count() < deck.keyCount
			for (int max(_configuration.keyColumns * _configuration.keyRows); index < max; ++index)
			{
				updateKeyImage(index);
			}
			// In case when _finalKeyModel->count() > deck.keyCount
			for (int max = model.count(); index < max; ++index)
			{
				setKeyPressed(model[index], false, true);
			}
		}
		else
		{
			for (int index = 0, max = model.count(); index < max; ++index)
			{
				setKeyPressed(model[index], false, true);
			}
		}
	}
}

void Device::Impl::applyModel(KeyModel * model)
{
	if (_finalKeyModel != model)
	{
		// disconnect
		if (_finalKeyModel)
		{
			disconnect(_finalKeyModel.data(), nullptr, &_device, nullptr);
			disconnect(&_device, nullptr, _finalKeyModel.data(), nullptr);
		}

		_finalKeyModel = model;

		// connect
		if (_finalKeyModel)
		{
			updateAllKeysFromModel();
			connect(&_device, &Device::isOpenChanged, _finalKeyModel.data(), [this]() { updateAllKeysFromModel(); });

			auto onPressedAction = [this](int pressedIndex)
			{
				if (!_finalKeyModel || pressedIndex < 0 || pressedIndex >= _finalKeyModel->count())
					return;

				auto & model = *_finalKeyModel.data();
				setKeyPressed(model[pressedIndex], _buttonsState[pressedIndex]);
			};
			connect(&_device, &Device::pressed, _finalKeyModel.data(), onPressedAction);
			connect(&_device, &Device::released, _finalKeyModel.data(), onPressedAction);

			connect(
				_finalKeyModel.data(),
				&KeyModel::imageChanged,
				&_device,
				[this](int index, const BaseKeyEntry * entry) { updateKeyImage(index, entry); }
			);
			connect(
				_finalKeyModel.data(),
				&KeyModel::modelEntryChanged,
				&_device,
				[this](int index, const BaseKeyEntry * entry)
				{
					auto & model = *_finalKeyModel.data();
					if (index >= 0 && index < _finalKeyModel->count())
					{
						setKeyPressed(
							model[index],
							index < _configuration.keyColumns * _configuration.keyRows ? _buttonsState[index] : false,
							true
						);
					}
					updateKeyImage(index, entry);
				}
			);
		}
	}
}

void Device::Impl::setKeyPressed(BaseKeyEntry * entry, bool state, bool initializationOnly) const
{
	if (entry)
	{
		entry->setPressed(state, initializationOnly);
	}
}

Device::Device(QObject * parent)
	: QObject{parent}
	, _pImpl{new Impl{*this}}
{}

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
	return _pImpl->_connected ? _pImpl->_serialNumber : _pImpl->_expectedSerialNumber;
}

void Device::setSerialNumber(const QString & number)
{
	if (_pImpl->_connected)
	{
		qWarning() << "Could not change serialNumber of already connected device";
	}
	else
	{
		_pImpl->setSerialNumber(number);
	}
}

QString Device::firmwareVersion() const
{
	return _pImpl->_firmwareNumber;
}

QString Device::manufacturer() const
{
	return _pImpl->_interface->manufacturer();
}

QSize Device::originalKeyImageSize() const
{
	return {_pImpl->_configuration.imageWidth, _pImpl->_configuration.imageHeight};
}

QString Device::originalKeyImageFormat() const
{
	return _pImpl->_configuration.imageFormatAsString();
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

bool Device::isOpen() const
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

	// What is the expected behavior on reset when model defined?
	_pImpl->updateAllKeysFromModel();
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

int Device::brightness() const
{
	return _pImpl->_brightness;
}

void Device::init()
{
	if (_pImpl->_initialized)
		return;

	_pImpl->_initialized = true;

	connect(
		DeviceManager::instance(),
		&DeviceManager::removed,
		this,
		[this](auto id)
		{
			qInfo() << "Device removed:" << id;
			if (_pImpl->_connected && id == DeviceId(_pImpl->_deviceType, _pImpl->_serialNumber))
			{
				close();
				_pImpl->_connected = false;
				qInfo() << "Device disconnected";
				emit connectedChanged();
				_pImpl->setValid(false);
				if (_pImpl->_expectedDeviceType == DeviceType::Any || _pImpl->_expectedSerialNumber.isEmpty())
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
		[this](auto id)
		{
			if (!_pImpl->_connected)
			{
				qInfo() << "Device inserted:" << id << ", expected" << _pImpl->_expectedDeviceType << "serial "
						<< _pImpl->_expectedSerialNumber;
				if (_pImpl->_expectedDeviceType == DeviceType::Any ||
					(id.type == _pImpl->_expectedDeviceType &&
					 (_pImpl->_expectedSerialNumber.isEmpty() || _pImpl->_expectedSerialNumber == id.serialNumber)))
				{
					qInfo() << "Expected device inserted: reinit";
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
	return _pImpl->_expectedDeviceType;
}

DeviceType Device::deviceType() const
{
	return _pImpl->_deviceType;
}

void Device::setExpectedDeviceType(DeviceType deviceType)
{
	if (deviceType != _pImpl->_expectedDeviceType)
	{
		_pImpl->_expectedDeviceType = deviceType;
		emit expectedDeviceTypeChanged();

		if (_pImpl->_initialized && _pImpl->_deviceType != deviceType)
		{
			_pImpl->reinit();
		}
	}
}

void Device::sendImage(int keyIndex, const QImage & image)
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

	if (keyIndex < 0 || keyIndex >= keyCount())
	{
		qWarning() << "Device::sendImage keyIndex" << keyIndex << "is out of range!";
		return;
	}

	QImage imageOriginal{image};
	QByteArray data;
	QBuffer bf{&data};
	bf.open(QIODevice::WriteOnly);
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
	bf.close();
	_pImpl->_interface->sendImage(keyIndex, data);
}

void Device::sendImage(int keyIndex, QUrl source)
{
	QImage image;
	auto scheme = source.scheme();
	if (source.scheme() == "data")
	{
		static QRegularExpression re("^image/.+;base64,(?<data>.+)");
		QRegularExpressionMatch match = re.match(source.path());
		if (match.hasMatch())
		{
			QString data = match.captured("data");
			image.loadFromData(QByteArray::fromBase64(data.toUtf8()));
		}
		else
		{
			qWarning() << "Could not find image data section in base64 source";
			return;
		}
	}
	else if (scheme == "file" || scheme == "qrc")
	{
		QString filePath = scheme == "qrc" ? source.path().prepend(":") : source.toLocalFile();

		QFile file{filePath};
		if (!file.exists())
		{
			qWarning() << "Device::sendImage file" << source.fileName() << "is not exist!";
			return;
		}
		image.load(file.fileName());
	}
	else
	{
		qWarning() << "Could not load image. Unsupported source scheme" << scheme;
		return;
	}

	sendImage(keyIndex, image);
}

QString Device::deviceTypeToString(DeviceType value)
{
	const QString notValidValue = QStringLiteral("Not valid value");
	QString enumToString{QMetaEnum::fromType<minervous::streamdeck::Device::DeviceType>().valueToKey(value)};
	return (enumToString.isEmpty() ? notValidValue : enumToString);
}

const QUrl & Device::emptyImageSource()
{
	static const QUrl emptyImageSource{
		QStringLiteral("data:image/png;base64,"
					   "iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAABJ2lDQ1BrQ0dDb2xvclNwY"
					   "WNlQWRvYmVSR0IxOTk4AAAokWNgYFJILCjIYRJgYMjNKykKcndSiIiMUmB/zsDNwMcgxM"
					   "DGYJ2YXFzgGBDgwwAEMBoVfLvGwAiiL+uCzMKUxwu4UlKLk4H0HyDOTi4oKmFgYMwAspX"
					   "LSwpA7B4gWyQpG8xeAGIXAR0IZG8BsdMh7BNgNRD2HbCakCBnIPsDkM2XBGYzgeziS4ew"
					   "BUBsqL0gIOiYkp+UqgDyvYahpaWFJol+IAhKUitKQLRzfkFlUWZ6RomCIzCkUhU885L1d"
					   "BSMDIyMGRhA4Q5R/TkQHJ6MYmcQYgiAEJsjwcDgv5SBgeUPQsykl4FhgQ4DA/9UhJiaIQ"
					   "ODgD4Dw745yaVFZVBjGJmAdhLiAwBDL0poYdW97AAAATxlWElmTU0AKgAAAAgACQEOAAI"
					   "AAAA7AAAAegESAAMAAAABAAEAAAEaAAUAAAABAAAAtgEbAAUAAAABAAAAvgEoAAMAAAAB"
					   "AAIAAAExAAIAAAANAAAAxgE7AAIAAAAjAAAA1IKYAAIAAAAOAAAA+IdpAAQAAAABAAABB"
					   "gAAAABPSyBlbW9qaSAgcG5nIHN0aWNrZXIsIDNEIHJlbmRlcmluZyB0cmFuc3BhcmVudC"
					   "BiYWNrZ3JvdW5kAAAAAAEsAAAAAQAAASwAAAABcmF3cGl4ZWwuY29tAAByYXdwaXhlbC5"
					   "jb20gLyBTYWthcmluIFN1a21hbmF0aGFtAABSYXdwaXhlbCBMdGQuAAAEkAAABwAAAAQw"
					   "MjEwoAAABwAAAAQwMTAwoAIABAAAAAEAAAABoAMABAAAAAEAAAABAAAAAMdpYXcAAAAJc"
					   "EhZcwAALiMAAC4jAXilP3YAAAscaVRYdFhNTDpjb20uYWRvYmUueG1wAAAAAAA8eDp4bX"
					   "BtZXRhIHhtbG5zOng9ImFkb2JlOm5zOm1ldGEvIiB4OnhtcHRrPSJYTVAgQ29yZSA2LjA"
					   "uMCI+CiAgIDxyZGY6UkRGIHhtbG5zOnJkZj0iaHR0cDovL3d3dy53My5vcmcvMTk5OS8w"
					   "Mi8yMi1yZGYtc3ludGF4LW5zIyI+CiAgICAgIDxyZGY6RGVzY3JpcHRpb24gcmRmOmFib"
					   "3V0PSIiCiAgICAgICAgICAgIHhtbG5zOnhtcFJpZ2h0cz0iaHR0cDovL25zLmFkb2JlLm"
					   "NvbS94YXAvMS4wL3JpZ2h0cy8iCiAgICAgICAgICAgIHhtbG5zOmV4aWY9Imh0dHA6Ly9"
					   "ucy5hZG9iZS5jb20vZXhpZi8xLjAvIgogICAgICAgICAgICB4bWxuczp0aWZmPSJodHRw"
					   "Oi8vbnMuYWRvYmUuY29tL3RpZmYvMS4wLyIKICAgICAgICAgICAgeG1sbnM6ZGM9Imh0d"
					   "HA6Ly9wdXJsLm9yZy9kYy9lbGVtZW50cy8xLjEvIgogICAgICAgICAgICB4bWxuczpwbH"
					   "VzPSJodHRwOi8vbnMudXNlcGx1cy5vcmcvbGRmL3htcC8xLjAvIgogICAgICAgICAgICB"
					   "4bWxuczp4bXA9Imh0dHA6Ly9ucy5hZG9iZS5jb20veGFwLzEuMC8iPgogICAgICAgICA8"
					   "eG1wUmlnaHRzOldlYlN0YXRlbWVudD5odHRwczovL3d3dy5yYXdwaXhlbC5jb20vc2Vyd"
					   "mljZXMvbGljZW5zZXM8L3htcFJpZ2h0czpXZWJTdGF0ZW1lbnQ+CiAgICAgICAgIDxleG"
					   "lmOkNvbG9yU3BhY2U+NjU1MzU8L2V4aWY6Q29sb3JTcGFjZT4KICAgICAgICAgPGV4aWY"
					   "6UGl4ZWxYRGltZW5zaW9uPjcyPC9leGlmOlBpeGVsWERpbWVuc2lvbj4KICAgICAgICAg"
					   "PGV4aWY6RXhpZlZlcnNpb24+MDIxMDwvZXhpZjpFeGlmVmVyc2lvbj4KICAgICAgICAgP"
					   "GV4aWY6Rmxhc2hQaXhWZXJzaW9uPjAxMDA8L2V4aWY6Rmxhc2hQaXhWZXJzaW9uPgogIC"
					   "AgICAgICA8ZXhpZjpQaXhlbFlEaW1lbnNpb24+NzI8L2V4aWY6UGl4ZWxZRGltZW5zaW9"
					   "uPgogICAgICAgICA8dGlmZjpSZXNvbHV0aW9uVW5pdD4yPC90aWZmOlJlc29sdXRpb25V"
					   "bml0PgogICAgICAgICA8dGlmZjpPcmllbnRhdGlvbj4xPC90aWZmOk9yaWVudGF0aW9uP"
					   "gogICAgICAgICA8dGlmZjpYUmVzb2x1dGlvbj4zMDA8L3RpZmY6WFJlc29sdXRpb24+Ci"
					   "AgICAgICAgIDx0aWZmOllSZXNvbHV0aW9uPjMwMDwvdGlmZjpZUmVzb2x1dGlvbj4KICA"
					   "gICAgICAgPGRjOnRpdGxlPgogICAgICAgICAgICA8cmRmOkFsdD4KICAgICAgICAgICAg"
					   "ICAgPHJkZjpsaSB4bWw6bGFuZz0ieC1kZWZhdWx0Ij5PSyBlbW9qaSAgcG5nIHN0aWNrZ"
					   "XIsIDNEIHJlbmRlcmluZyB0cmFuc3BhcmVudCBiYWNrZ3JvdW5kPC9yZGY6bGk+CiAgIC"
					   "AgICAgICAgIDwvcmRmOkFsdD4KICAgICAgICAgPC9kYzp0aXRsZT4KICAgICAgICAgPGR"
					   "jOmRlc2NyaXB0aW9uPgogICAgICAgICAgICA8cmRmOkFsdD4KICAgICAgICAgICAgICAg"
					   "PHJkZjpsaSB4bWw6bGFuZz0ieC1kZWZhdWx0Ij5PSyBlbW9qaSAgcG5nIHN0aWNrZXIsI"
					   "DNEIHJlbmRlcmluZyB0cmFuc3BhcmVudCBiYWNrZ3JvdW5kPC9yZGY6bGk+CiAgICAgIC"
					   "AgICAgIDwvcmRmOkFsdD4KICAgICAgICAgPC9kYzpkZXNjcmlwdGlvbj4KICAgICAgICA"
					   "gPGRjOnJpZ2h0cz4KICAgICAgICAgICAgPHJkZjpBbHQ+CiAgICAgICAgICAgICAgIDxy"
					   "ZGY6bGkgeG1sOmxhbmc9IngtZGVmYXVsdCI+UmF3cGl4ZWwgTHRkLjwvcmRmOmxpPgogI"
					   "CAgICAgICAgICA8L3JkZjpBbHQ+CiAgICAgICAgIDwvZGM6cmlnaHRzPgogICAgICAgIC"
					   "A8ZGM6c3ViamVjdD4KICAgICAgICAgICAgPHJkZjpCYWc+CiAgICAgICAgICAgICAgIDx"
					   "yZGY6bGk+b2sgZW1vamk8L3JkZjpsaT4KICAgICAgICAgICAgICAgPHJkZjpsaT4zZCBv"
					   "ayBoYW5kPC9yZGY6bGk+CiAgICAgICAgICAgICAgIDxyZGY6bGk+ZW1vamk8L3JkZjpsa"
					   "T4KICAgICAgICAgICAgICAgPHJkZjpsaT5zbWlsZXkgZmFjZTwvcmRmOmxpPgogICAgIC"
					   "AgICAgICAgICA8cmRmOmxpPnllcyBlbW9qaTwvcmRmOmxpPgogICAgICAgICAgICAgICA"
					   "8cmRmOmxpPmVtb2ppIHBuZzwvcmRmOmxpPgogICAgICAgICAgICAgICA8cmRmOmxpPmZh"
					   "Y2UgM2QgaWxsdXN0cmF0aW9uPC9yZGY6bGk+CiAgICAgICAgICAgICAgIDxyZGY6bGk+M"
					   "yBkaW1lbnNpb25hbDwvcmRmOmxpPgogICAgICAgICAgICAgICA8cmRmOmxpPjNkPC9yZG"
					   "Y6bGk+CiAgICAgICAgICAgICAgIDxyZGY6bGk+M2QgZW1vamk8L3JkZjpsaT4KICAgICA"
					   "gICAgICAgICAgPHJkZjpsaT4zZCBlbW90aWNvbjwvcmRmOmxpPgogICAgICAgICAgICAg"
					   "ICA8cmRmOmxpPjNkIGdyYXBoaWNzPC9yZGY6bGk+CiAgICAgICAgICAgIDwvcmRmOkJhZ"
					   "z4KICAgICAgICAgPC9kYzpzdWJqZWN0PgogICAgICAgICA8ZGM6Y3JlYXRvcj4KICAgIC"
					   "AgICAgICAgPHJkZjpTZXE+CiAgICAgICAgICAgICAgIDxyZGY6bGk+cmF3cGl4ZWwuY29"
					   "tIC8gU2FrYXJpbiBTdWttYW5hdGhhbTwvcmRmOmxpPgogICAgICAgICAgICA8L3JkZjpT"
					   "ZXE+CiAgICAgICAgIDwvZGM6Y3JlYXRvcj4KICAgICAgICAgPHBsdXM6TGljZW5zb3I+C"
					   "iAgICAgICAgICAgIDxyZGY6U2VxPgogICAgICAgICAgICAgICA8cmRmOmxpIHJkZjpwYX"
					   "JzZVR5cGU9IlJlc291cmNlIj4KICAgICAgICAgICAgICAgICAgPHBsdXM6TGljZW5zb3J"
					   "VUkw+aHR0cHM6Ly93d3cucmF3cGl4ZWwuY29tL2ltYWdlLzg2NjQ2Nzk8L3BsdXM6TGlj"
					   "ZW5zb3JVUkw+CiAgICAgICAgICAgICAgIDwvcmRmOmxpPgogICAgICAgICAgICA8L3JkZ"
					   "jpTZXE+CiAgICAgICAgIDwvcGx1czpMaWNlbnNvcj4KICAgICAgICAgPHhtcDpSYXRpbm"
					   "c+NjwveG1wOlJhdGluZz4KICAgICAgICAgPHhtcDpDcmVhdG9yVG9vbD5yYXdwaXhlbC5"
					   "jb208L3htcDpDcmVhdG9yVG9vbD4KICAgICAgPC9yZGY6RGVzY3JpcHRpb24+CiAgIDwv"
					   "cmRmOlJERj4KPC94OnhtcG1ldGE+ChAch2gAAAANSURBVAgdY2BgYMgCAABvAGs1y+D5A"
					   "AAAAElFTkSuQmCC")
	};
	return emptyImageSource;
}

KeyModel * Device::model() const
{
	return _pImpl->_modelProperty;
}

void Device::setModel(QObject * model)
{
	KeyModel * castedModel = qobject_cast<KeyModel *>(model);
	if (model && !castedModel)
	{
		qWarning() << "Device::setModel unexpected type of model" << model;
	}
	if (_pImpl->_modelProperty != castedModel)
	{
		_pImpl->_modelProperty = castedModel;
		applyModel(castedModel);
		emit modelChanged();
	}
}

void Device::applyModel(KeyModel * model)
{
	_pImpl->applyModel(model);
}
