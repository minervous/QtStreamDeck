#include "DeviceEmulator.hpp"

#include <QtCore/QDebug>
#include <QtGui/QImage>
#include <QtUsb/QHidDevice>
#include <QtUsb/QUsb>

#include "DeviceId.hpp"
#include "DeviceManager.hpp"

#include "devices/StreamDeckMini.hpp"
#include "devices/StreamDeckOriginal.hpp"
#include "devices/StreamDeckOriginalV2.hpp"
#include "devices/StreamDeckPedal.hpp"
#include "devices/StreamDeckXL.hpp"
#include "emulators/IEmulator.hpp"

using namespace minervous::streamdeck;
using namespace Qt::Literals;

struct DeviceEmulator::Impl
{
	explicit Impl(DeviceEmulator & device)
		: _device{device}
	{
		setConfiguration(_deviceType);
	}
	~Impl()
	{
		unregisterEmulator();
	}
	DeviceEmulator & _device;

	bool _connected  = true;
	bool _registered = false;
	bool _isOpen = false;
	QString _manufacturer = "Minervous";
	QString _serialNumber = "1.2.3";
	QString _modelName = "Stream Deck Emulator";
	QString _firmwareVersion = "4.5.6";
	IDevice::Configuration _configuration;
	DeviceType _deviceType = DeviceType::MK2;
	QList<bool> _buttonsStates;
	QQueue<QList<bool>> _queueToSend;

	int _brightness = 50;

	struct DeviceInterfaceWrapper: public IDevice
	{
		DeviceInterfaceWrapper(DeviceEmulator::Impl &emulator)
			: emulator {emulator}
		{}

		~DeviceInterfaceWrapper() override = default;

		bool open(const QString & serial) override
		{
			if (emulator._isOpen ||
				(!serial.isEmpty() && emulator._serialNumber != serial))
			{
				return false;
			} else {
				emulator._isOpen = true;
				emit emulator._device.isOpenChanged();
				return true;
			}
		}
		void close() override
		{
			emulator.close();
		}
		bool isOpen() const override
		{
			return emulator._isOpen;
		}
		QString manufacturer() const override
		{
			return emulator._isOpen ? emulator._manufacturer : "";
		}
		QString serialNumber() const override
		{
			return emulator._isOpen ? emulator._serialNumber : "";
		}
		QString product() const override
		{
			return emulator._isOpen ? emulator._modelName : "";
		}

		const Configuration & getConfiguration() const override
		{
			return emulator._configuration;
		}
		bool setBrightness(int brightness) override
		{
			if (!emulator._configuration.hasDisplay)
			{
				return false;
			}
			if (emulator._isOpen && brightness != emulator._brightness)
			{
				emulator._brightness = brightness;
				emit emulator._device.brightnessChanged();
			}
			return emulator._isOpen;
		}
		QString getFirmwareVersion() override
		{
			if (emulator._isOpen)
			{
				return emulator._firmwareVersion;
			} else {
				return {};
			}
		}

		bool reset() override
		{
			if (emulator._isOpen) {
				emulator.reset();
				emit emulator._device.resetCalled();
			}

			return emulator._isOpen;
		}

		int readButtonsStatus(QList<bool> & buttonsStates) override
		{
			if (emulator._isOpen)
			{
				if (!emulator._queueToSend.isEmpty())
				{
						buttonsStates = emulator._queueToSend.dequeue();
					qInfo() << "Emulator readButtonsStatus data";
					return 1;
				} else {
					return 0;
				}
			} else {
				return -1;
			}
		}

		bool sendImage(int keyIndex, const QByteArray & imageData) override
		{
			qInfo() << "DeviceEmulator: the image was received for index" << keyIndex;

			if (!emulator._isOpen || !emulator._configuration.hasDisplay)
				return false;

			// Reverse image transformations
			QTransform rotating;
			rotating.rotate(360 - emulator._configuration.imageRotation);

			auto image =
				QImage::fromData(imageData, emulator._configuration.imageFormatAsString())
					.transformed(rotating)
					.mirrored(emulator._configuration.imageHorizontalFlip, emulator._configuration.imageVerticalFlip);

			QByteArray data;
			QBuffer buffer{&data};
			image.save(&buffer, "PNG", 100);

			QString base64 = data.size() > 0 ? u"data:image/png;base64,%1"_s.arg(data.toBase64()) : "";

			emit emulator._device.imageSent(keyIndex, data, base64);
			return true;
		}

		DeviceEmulator::Impl &emulator;
	};

	struct EmulatorInterface: public IEmulator
	{
		EmulatorInterface(DeviceEmulator::Impl &emulator, DeviceId id)
			: emulator(emulator)
			, id{id}
		{}

		~EmulatorInterface() override
		{}

		IDevice *createInterface() override
		{
			return new DeviceInterfaceWrapper{emulator};
		}

		DeviceId deviceId() override
		{
			return id;
		}
		DeviceEmulator::Impl &emulator;
		DeviceId id;
	};

	QScopedPointer<EmulatorInterface> _interface;

	void setConfiguration(DeviceType type)
	{
		switch (type)
		{
		case DeviceType::Any:
		case DeviceType::MK2:
			_configuration = StreamDeckOriginalV2::deviceConfiguration(StreamDeckOriginalV2::PID_MK2);
			break;

		case DeviceType::Original:
			_configuration = StreamDeckOriginal::deviceConfiguration();
			break;
		case DeviceType::OriginalV2:
			_configuration = StreamDeckOriginalV2::deviceConfiguration(StreamDeckOriginalV2::PID_OriginalV2);
			break;
		case DeviceType::Mini:
			_configuration = StreamDeckMini::deviceConfiguration(StreamDeckMini::PID_Mini);
			break;
		case DeviceType::MiniMK2:
			_configuration = StreamDeckMini::deviceConfiguration(StreamDeckMini::PID_MiniMK2);
			break;
		case DeviceType::XL:
			_configuration = StreamDeckMini::deviceConfiguration(StreamDeckXL::PID_XL);
			break;
		case DeviceType::XLV2:
			_configuration = StreamDeckMini::deviceConfiguration(StreamDeckXL::PID_XLV2);
			break;
		case DeviceType::Pedal:
			_configuration = StreamDeckPedal::deviceConfiguration();
			break;

		case DeviceType::Unknown:
		default:
			_configuration = {};
			break;
		}

		_buttonsStates.resize(_configuration.keyColumns * _configuration.keyRows);
		_buttonsStates.fill(false);
		_queueToSend.clear();
	}

	void setRegistered(bool registered)
	{
		_registered = registered;
	}

	void close()
	{
		if (_isOpen)
		{
			_isOpen = false;
			emit _device.isOpenChanged();
		}
	}

	void reset()
	{
		_brightness = 50;
		_buttonsStates.fill(false);
		_queueToSend.clear();
		emit _device.brightnessChanged();
	}

	void registerEmulator()
	{
		_interface.reset(new EmulatorInterface(*this, {_deviceType, _serialNumber}));
		setRegistered(DeviceManager::instance()->registerEmulator(_interface.data()));
	}

	void unregisterEmulator()
	{
		DeviceManager::instance()->unregisterEmulator(_interface.data());
		setRegistered(false);
		_interface.reset();
		close();
		reset();
	}

	void reconfiguration()
	{
		if (_connected) {
			setConnected(false);
			setConnected(true);
		}
		emit _device.configurationUpdated();
	}

	void setConnected(bool connected)
	{
		if (_connected != connected)
		{
			if (connected)
			{
				registerEmulator();
			} else
			{
				unregisterEmulator();
			}
			if (_connected != _registered)
			{
				_connected = connected;
				emit _device.connectedChanged();
			} else {
				qWarning() << "Could not change connected. Registration failed";
			}
		}
	}
};

DeviceEmulator::DeviceEmulator(QObject * parent)
	: QObject{parent}
	, _pImpl{new Impl{*this}}
{

}

DeviceEmulator::~DeviceEmulator()
{

}

int DeviceEmulator::keyColumns() const
{
	return _pImpl->_configuration.keyColumns;
}

int DeviceEmulator::keyRows() const
{
	return _pImpl->_configuration.keyRows;
}

int DeviceEmulator::keyCount() const
{
	return _pImpl->_configuration.keyColumns * _pImpl->_configuration.keyRows;
}

bool DeviceEmulator::hasDisplay() const
{
	return _pImpl->_configuration.hasDisplay;
}

QString DeviceEmulator::modelName() const
{
	return _pImpl->_modelName;
}

QString DeviceEmulator::serialNumber() const
{
	return _pImpl->_serialNumber;
}

QString DeviceEmulator::firmwareVersion() const
{
	return _pImpl->_firmwareVersion;
}

QString DeviceEmulator::manufacturer() const
{
	return _pImpl->_manufacturer;
}

bool DeviceEmulator::isOpen() const
{
	return _pImpl->_isOpen;
}

int DeviceEmulator::brightness() const
{
	return _pImpl->_brightness;
}

bool DeviceEmulator::connected() const
{
	return _pImpl->_connected;
}

DeviceType DeviceEmulator::deviceType() const
{
	return _pImpl->_deviceType;
}

void DeviceEmulator::setConnected(bool connected)
{
	_pImpl->setConnected(connected);
}

void DeviceEmulator::setDeviceType(DeviceType deviceType)
{
	if (deviceType != _pImpl->_deviceType)
	{
		// unregister with old deviceId;
		_pImpl->_deviceType = deviceType;
		_pImpl->setConfiguration(deviceType);
		_pImpl->reconfiguration();
	}
}

void DeviceEmulator::setModelName(QString modelName)
{
	if (modelName != _pImpl->_modelName)
	{
		_pImpl->_modelName = modelName;
		_pImpl->reconfiguration();
	}
}

void DeviceEmulator::setManufacturer(QString manufacturer)
{
	if (manufacturer != _pImpl->_manufacturer)
	{
		_pImpl->_manufacturer = manufacturer;
		_pImpl->reconfiguration();
	}
}

void DeviceEmulator::setSerialNumber(QString number)
{
	if (_pImpl->_serialNumber != number)
	{
		_pImpl->_serialNumber = number;
		_pImpl->reconfiguration();
	}
}

void DeviceEmulator::setFirmwareVersion(QString firmwareVersion)
{
	if (_pImpl->_firmwareVersion != firmwareVersion)
	{
		_pImpl->_firmwareVersion = firmwareVersion;
		_pImpl->reconfiguration();
	}
}

void DeviceEmulator::init()
{
	if (_pImpl->_connected && _pImpl->_interface.isNull())
	{
		_pImpl->registerEmulator();
	}
}

void DeviceEmulator::press(int index)
{
	qInfo() << "DeviceEmulator::press" << index;
	if (index >= 0 && index < _pImpl->_buttonsStates.size())
	{
		if (!_pImpl->_buttonsStates[index] && _pImpl->_isOpen)
		{
			_pImpl->_buttonsStates[index] = true;
			_pImpl->_queueToSend.enqueue(_pImpl->_buttonsStates);
		}
	} else {
		qWarning() << "Invalid index";
	}
}

void DeviceEmulator::release(int index)
{
	qInfo() << "DeviceEmulator::release" << index;
	if (index >= 0 && index < _pImpl->_buttonsStates.size())
	{
		if (_pImpl->_buttonsStates[index] && _pImpl->_isOpen)
		{
			_pImpl->_buttonsStates[index] = false;
			_pImpl->_queueToSend.enqueue(_pImpl->_buttonsStates);
		}
	} else {
		qWarning() << "Invalid index";
	}
}
