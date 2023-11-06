#include "DeviceEmulator.hpp"

#include <QtCore/QDebug>
#include <QtGui/QImage>
#include <QtUsb/QHidDevice>
#include <QtUsb/QUsb>

#include "DeviceId.hpp"
#include "DeviceManager.hpp"

#include "devices/StreamDeckOriginalV2.hpp"
#include "emulators/IEmulator.hpp"

using namespace minervous::streamdeck;

struct DeviceEmulator::Impl
{
	explicit Impl(DeviceEmulator & device)
		: m_device(device)
	{
		setConfiguration(m_deviceType);
	}
	~Impl()
	{
		unregisterEmulator();
	}
	DeviceEmulator & m_device;

	bool m_connected {true};
	bool m_registered {false};
	bool m_isOpen {false};
	QString m_manufacturer {"Elgato"};
	QString m_serialNumber {"1.2.3"};
	QString m_modelName {"StreamDeck MK2 Emulator"};
	QString m_firmwareVersion {"4.5.6"};
	IDevice::Configuration m_configuration {.pid = StreamDeckOriginalV2::PID_MK2,
								  .keyColumns = 5,
								  .keyRows = 3,
								  .imageWidth = 72,
								  .imageFormat = IDevice::ImageFormat::JPEG,
								  .imageHorizontalFlip = true,
								  .imageVerticalFlip = true};
	DeviceType m_deviceType{DeviceType::STREAMDECK_MK2};
	QList<bool> m_buttonsStates;
	QQueue<QList<bool>> m_queueToSend;

	int m_brightness{70};

	struct DeviceInterfaceWrapper: public IDevice
	{
		DeviceInterfaceWrapper(DeviceEmulator::Impl &emulator)
			: emulator {emulator}
		{}

		~DeviceInterfaceWrapper() override = default;

		bool open(const QString & serial) override
		{
			// Temporary ignore SerialNumber
			Q_UNUSED(serial)

			if (emulator.m_isOpen)
			{
				return false;
			} else {
				emulator.m_isOpen = true;
				emit emulator.m_device.isOpenChanged();
				return true;
			}
		}
		void close() override
		{
			emulator.close();
		}
		bool isOpen() const override
		{
			return emulator.m_isOpen;
		}
		QString manufacturer() const override
		{
			if (emulator.m_isOpen)
			{
				return emulator.m_manufacturer;
			} else {
				return {};
			}
		}
		QString serialNumber() const override
		{
			if (emulator.m_isOpen)
			{
				return emulator.m_serialNumber;
			} else {
				return {};
			}
		}
		QString product() const override
		{
			if (emulator.m_isOpen)
			{
				return emulator.m_modelName;
			} else {
				return {};
			}
		}

		const Configuration & getConfiguration() const override
		{
			return emulator.m_configuration;
		}
		bool setBrightness(int brightness) override
		{
			if (emulator.m_isOpen && brightness != emulator.m_brightness)
			{
				emulator.m_brightness = brightness;
				emit emulator.m_device.brightnessChanged();
			}
			return emulator.m_isOpen;
		}
		QString getFirmwareVersion() override
		{
			if (emulator.m_isOpen)
			{
				return emulator.m_firmwareVersion;
			} else {
				return {};
			}
		}

		bool reset() override
		{
			if (emulator.m_isOpen) {
				emulator.reset();
				emit emulator.m_device.resetCalled();
			}

			return emulator.m_isOpen;
		}

		int readButtonsStatus(QList<bool> & buttonsStates) override
		{
			if (emulator.m_isOpen)
			{
				if (!emulator.m_queueToSend.isEmpty())
				{
						buttonsStates = emulator.m_queueToSend.dequeue();
					qInfo() << "Emulator readButtonsStatus data";
					return 512;
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
			if (emulator.m_isOpen)
			{
				// Restore original image format
				QByteArray data = imageData;
				QBuffer in(&data);
				QImage imageOriginal;
				imageOriginal.load(&in, emulator.m_configuration.imageFormatAsString());
				QBuffer out;
				imageOriginal.mirrored(emulator.m_configuration.imageHorizontalFlip, emulator.m_configuration.imageVerticalFlip)
					.save(&out, emulator.m_configuration.imageFormatAsString());
				emit emulator.m_device.imageSent(keyIndex, out.data());
				QString base64 = out.size() > 0 ? "data:image/png;base64," + out.data().toBase64() : "";
				emit emulator.m_device.imageSentBase64(keyIndex, base64);
			}
			return emulator.m_isOpen;
		}

		DeviceEmulator::Impl &emulator;
	};

	struct EmulatorInterface: public IEmulator
	{
		EmulatorInterface(DeviceEmulator::Impl &emulator, DeviceId id)
			: emulator{emulator}
			, id{id}
		{}

		~EmulatorInterface() override
		{}

		IDevice *createInterface() override
		{
			return new DeviceInterfaceWrapper(emulator);
		}

		DeviceId deviceId() override
		{
			return id;
		}
		DeviceEmulator::Impl &emulator;
		DeviceId id;
	};

	QScopedPointer<EmulatorInterface> m_interface;

	void setConfiguration(DeviceType)
	{
		// [TODO] @MJNIKOFF - add support of other device types
		m_buttonsStates.resize(m_configuration.keyColumns * m_configuration.keyRows);
		m_buttonsStates.fill(false);
		m_queueToSend.clear();
	}

	void setRegistered(bool registered)
	{
		m_registered = registered;
	}

	void close()
	{
		if (m_isOpen)
		{
			m_isOpen = false;
			emit m_device.isOpenChanged();
		}
	}

	void reset()
	{
		m_brightness = 70;
		m_buttonsStates.fill(false);
		m_queueToSend.clear();
		emit m_device.brightnessChanged();
	}

	void registerEmulator()
	{
		m_interface.reset(new EmulatorInterface(*this, {m_deviceType}));
		setRegistered(DeviceManager::instance()->registerEmulator(m_interface.data()));
	}

	void unregisterEmulator()
	{
		DeviceManager::instance()->unregisterEmulator(m_interface.data());
		setRegistered(false);
		m_interface.reset();
		close();
		reset();
	}

	void reconfiguration()
	{
		if (m_connected) {
			unregisterEmulator();
			registerEmulator();
		}
		emit m_device.configurationUpdated();
	}
};

DeviceEmulator::DeviceEmulator(QObject * parent)
	: QObject{parent}
	, m_pImpl(new Impl(*this))
{

}

DeviceEmulator::~DeviceEmulator()
{

}

int DeviceEmulator::keyColumns() const
{
	return m_pImpl->m_configuration.keyColumns;
}

int DeviceEmulator::keyRows() const
{
	return m_pImpl->m_configuration.keyRows;
}

int DeviceEmulator::keyCount() const
{
	return m_pImpl->m_configuration.keyColumns * m_pImpl->m_configuration.keyRows;
}

bool DeviceEmulator::hasDisplay() const
{
	return m_pImpl->m_configuration.keyColumns;
}

QString DeviceEmulator::modelName() const
{
	return m_pImpl->m_modelName;
}

QString DeviceEmulator::serialNumber() const
{
	return m_pImpl->m_serialNumber;
}

QString DeviceEmulator::firmwareVersion() const
{
	return m_pImpl->m_firmwareVersion;
}

QString DeviceEmulator::manufacturer() const
{
	return m_pImpl->m_manufacturer;
}

bool DeviceEmulator::isOpen() const
{
	return m_pImpl->m_isOpen;
}

int DeviceEmulator::brightness() const
{
	return m_pImpl->m_brightness;
}

bool DeviceEmulator::connected() const
{
	return m_pImpl->m_connected;
}

DeviceType DeviceEmulator::deviceType() const
{
	return m_pImpl->m_deviceType;
}

void DeviceEmulator::setConnected(bool connected)
{
	if (connected != m_pImpl->m_connected)
	{
		if (connected)
		{
			m_pImpl->registerEmulator();
		} else
		{
			m_pImpl->unregisterEmulator();
		}
		if (m_pImpl->m_connected != m_pImpl->m_registered)
		{
			m_pImpl->m_connected = connected;
			emit connectedChanged();
		} else {
			qWarning() << "Could not chnage connected. Registration failed";
		}
	}
}

void DeviceEmulator::setDeviceType(DeviceType deviceType)
{
	if (deviceType != m_pImpl->m_deviceType)
	{
		// unregister with old deviceId;
		m_pImpl->m_deviceType = deviceType;
		m_pImpl->setConfiguration(deviceType);
		m_pImpl->reconfiguration();
	}
}

void DeviceEmulator::setModelName(QString modelName)
{
	if (modelName != m_pImpl->m_modelName)
	{
		m_pImpl->m_modelName = modelName;
		m_pImpl->reconfiguration();
	}
}

void DeviceEmulator::setManufacturer(QString manufacturer)
{
	if (manufacturer != m_pImpl->m_manufacturer)
	{
		m_pImpl->m_manufacturer = manufacturer;
		m_pImpl->reconfiguration();
	}
}

void DeviceEmulator::setSerialNumber(QString number)
{
	if (m_pImpl->m_serialNumber != number)
	{
		m_pImpl->m_serialNumber = number;
		m_pImpl->reconfiguration();
	}
}

void DeviceEmulator::setFirmwareVersion(QString firmwareVersion)
{
	if (m_pImpl->m_firmwareVersion != firmwareVersion)
	{
		m_pImpl->m_firmwareVersion = firmwareVersion;
		m_pImpl->reconfiguration();
	}
}

void DeviceEmulator::init()
{
	if (m_pImpl->m_connected && m_pImpl->m_interface.isNull())
	{
		m_pImpl->registerEmulator();
	}
}

void DeviceEmulator::press(int index)
{
	qInfo() << "DeviceEmulator::press" << index;
	if (index >= 0 && index < m_pImpl->m_buttonsStates.size())
	{
		if (!m_pImpl->m_buttonsStates[index] && m_pImpl->m_isOpen)
		{
			m_pImpl->m_buttonsStates[index] = true;
			m_pImpl->m_queueToSend.enqueue(m_pImpl->m_buttonsStates);
		}
	} else {
		qWarning() << "Invalid index";
	}
}

void DeviceEmulator::release(int index)
{
	qInfo() << "DeviceEmulator::release" << index;
	if (index >= 0 && index < m_pImpl->m_buttonsStates.size())
	{
		if (m_pImpl->m_buttonsStates[index] && m_pImpl->m_isOpen)
		{
			m_pImpl->m_buttonsStates[index] = false;
			m_pImpl->m_queueToSend.enqueue(m_pImpl->m_buttonsStates);
		}
	} else {
		qWarning() << "Invalid index";
	}
}
