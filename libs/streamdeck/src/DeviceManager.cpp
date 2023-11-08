#include "DeviceManager.hpp"

#include <QtCore/QDebug>
#include <QtUsb/QHidDevice>

#include "devices/IDevice.hpp"
#include "emulators/IEmulator.hpp"

#include "devices/DummyDevice.hpp"
#include "devices/StreamDeckMini.hpp"
#include "devices/StreamDeckOriginal.hpp"
#include "devices/StreamDeckOriginalV2.hpp"
#include "devices/StreamDeckPedal.hpp"
#include "devices/StreamDeckXL.hpp"

using namespace minervous::streamdeck;

DeviceType DeviceManager::convert(quint16 vid, quint16 pid)
{
	if (vid != IDevice::VID)
	{
		return DeviceType::UNKNOWN_DEVICE;
	}
	switch (pid)
	{
	case StreamDeckOriginal::PID:
		return DeviceType::STREAMDECK_ORIGINAL;
	case StreamDeckMini::PID_MINI:
		return DeviceType::STREAMDECK_MINI;
	case StreamDeckMini::PID_MINI_MK2:
		return DeviceType::STREAMDECK_MINI_MK2;
	case StreamDeckOriginalV2::PID_MK2:
		return DeviceType::STREAMDECK_MK2;
	case StreamDeckOriginalV2::PID_ORIGINAL_V2:
		return DeviceType::STREAMDECK_ORIGINAL_V2;
	case StreamDeckPedal::PID:
		return DeviceType::STREAMDECK_PEDAL;
	case StreamDeckXL::PID_XL:
		return DeviceType::STREAMDECK_XL;
	case StreamDeckXL::PID_XL_V2:
		return DeviceType::STREAMDECK_XL_V2;

	default:
		return DeviceType::UNKNOWN_DEVICE;
	}
}

DeviceManager * DeviceManager::instance()
{
	static DeviceManager * instance = new DeviceManager();
	return instance;
}

DeviceManager * DeviceManager::create(QQmlEngine * qmlEngine, QJSEngine *)
{
	DeviceManager* inst = instance();
	if (qmlEngine && inst) {
		qmlEngine->setObjectOwnership(inst, QQmlEngine::CppOwnership);
	}
	return inst;
}

DeviceId DeviceManager::getDeviceId(QUsb::Id id) const
{
	// [TODO] @MJNIKOFF - add and use Hid::open with original path (several devices with the same VID && PID could be
	// connected at the same time...) [TODO] @MJNIKOFF - uncomment when fixed issue in HID::open with predefined Serial
	// QHidDevice hid;
	// hid.open(id.vid, id.pid);
	DeviceId devId{convert(id.vid, id.pid)}; //, hid.serialNumber()}; - commented due to issue in HID::open with predefined Serial
	// hid.close();

	return devId;
}

DeviceManager::DeviceManager()
{
	auto usbDevices = _usb.devices();
	for (const auto & id: qAsConst(usbDevices))
	{
		DeviceType type = convert(id.vid, id.pid);
		if (DeviceType::UNKNOWN_DEVICE != type)
		{
			DeviceId devId{getDeviceId(id)};
			if (!_deviceList.contains(devId))
			{
				_deviceList.append(devId);
			}
		}
	}
	qInfo() << "DeviceManager connected devices:" << _deviceList;

	connect(&_usb, &QUsb::deviceInserted, this, &DeviceManager::onDevInserted);
	connect(&_usb, &QUsb::deviceRemoved, this, &DeviceManager::onDevRemoved);
}

DeviceManager::DeviceIdList DeviceManager::devices()
{
	return _deviceList;
}

DeviceId DeviceManager::createDeviceId(DeviceType type, const QString & serialNumber)
{
	return DeviceId(type, serialNumber);
}

void DeviceManager::onDevInserted(QUsb::Id id)
{
	DeviceType type{convert(id.vid, id.pid)};
	if (DeviceType::UNKNOWN_DEVICE != type)
	{
		DeviceId devId{getDeviceId(id)};
		insert(devId);
	}
}

void DeviceManager::onDevRemoved(QUsb::Id id)
{
	DeviceType type{convert(id.vid, id.pid)};
	if (DeviceType::UNKNOWN_DEVICE != type)
	{
		// [TODO] @MJNIKOFF - store QUsb::Id in the list, then check id = id -> yopu can use correct SerialNumber for
		// related deviceId
		DeviceId devId{type};  //, serialNumber()};
		remove(devId);
	}
}

DeviceManager::IDevice * DeviceManager::createInterface(DeviceId const id)
{
	DeviceManager::IDevice * idevice = nullptr;
	auto emulator = _emulators.find(id);
	if (emulator != _emulators.end())
	{
		idevice = emulator.value()->createInterface();
	} else {
		switch (id.type)
		{
		case DeviceType::STREAMDECK_MINI:
			idevice = new StreamDeckMini(StreamDeckMini::PID_MINI);
			break;
		case DeviceType::STREAMDECK_MINI_MK2:
			idevice = new StreamDeckMini(StreamDeckMini::PID_MINI_MK2);
			break;
		case DeviceType::STREAMDECK_ORIGINAL_V2:
			idevice = new StreamDeckOriginalV2(StreamDeckOriginalV2::PID_ORIGINAL_V2);
			break;
		case DeviceType::STREAMDECK_MK2:
			idevice = new StreamDeckOriginalV2(StreamDeckOriginalV2::PID_MK2);
			break;
		case DeviceType::STREAMDECK_XL:
			idevice = new StreamDeckXL(StreamDeckXL::PID_XL);
			break;
		case DeviceType::STREAMDECK_XL_V2:
			idevice = new StreamDeckXL(StreamDeckXL::PID_XL_V2);
			break;
		case DeviceType::STREAMDECK_ORIGINAL:
			idevice = new StreamDeckOriginal;
			break;
		case DeviceType::STREAMDECK_PEDAL:
			idevice = new StreamDeckPedal;
			break;
		case DeviceType::UNKNOWN_DEVICE:
		case DeviceType::STREAMDECK_ANY:
		default:
			idevice = new DummyDevice;
			break;
		}
	}
	return idevice;
}

bool DeviceManager::registerEmulator(DeviceManager::IEmulator * emu)
{
	if (!emu)
	{
		qWarning() << "Could not add invalid emulator";
		return false;
	}
	auto deviceId = emu->deviceId();
	if (_deviceList.contains(deviceId))
	{
		qWarning() << "Could not add emulator. Device with the same deviceId" << deviceId << "is already registered";
		return false;
	}
	if (deviceId.type == DeviceType::UNKNOWN_DEVICE)
	{
		qWarning() << "Could not add DeviceType::UNKNOWN_DEVICE as emulator";
		return false;
	}

	bool result = _emulators.insert(deviceId, emu) != _emulators.end();

	qInfo() << "registerEmulator" << deviceId << result;
	if (result) {
		insert(deviceId);
	}

	return result;
}

void DeviceManager::unregisterEmulator(IEmulator *emu)
{
	if (emu)
	{
		qInfo() << "unregisterEmulator" << emu->deviceId();
		if (_emulators.remove(emu->deviceId()))
		{
			remove(emu->deviceId());
		}
	}
}

void DeviceManager::insert(DeviceId id)
{
	if (!_deviceList.contains(id))
	{
		_deviceList.append(id);
		qInfo() << "DeviceManager device inserted:" << id;
		emit inserted(id);
		emit devicesChanged();
	}
}

void DeviceManager::remove(DeviceId id)
{
	if (_deviceList.removeOne(id))
	{
		qInfo() << "DeviceManager device removed:" << id;
		emit removed(id);
		emit devicesChanged();
	}
}
