#include "DeviceManager.hpp"

#include <map>

#include <QtCore/QDebug>
#include <QtUsb/QHidDevice>
#include <QtUsb/QUsb>

#include "devices/DummyDevice.hpp"
#include "devices/IDevice.hpp"
#include "devices/StreamDeckMini.hpp"
#include "devices/StreamDeckOriginal.hpp"
#include "devices/StreamDeckOriginalV2.hpp"
#include "devices/StreamDeckPedal.hpp"
#include "devices/StreamDeckPlus.hpp"
#include "devices/StreamDeckXL.hpp"
#include "emulators/IEmulator.hpp"

using namespace minervous::streamdeck;

// [TODO] @MJNIKOFF - add it to Usb lib, and comparision operators also
template <>
struct std::hash<QUsb::Id>
{
	std::size_t operator()(const QUsb::Id & id) const noexcept
	{
		std::size_t h1 = std::hash<quint16>{}(id.vid);
		std::size_t h2 = std::hash<quint16>{}(id.pid);
		std::size_t h3 = std::hash<quint8>{}(id.bus);
		std::size_t h4 = std::hash<quint8>{}(id.port);
		std::size_t h5 = std::hash<quint8>{}(id.dClass);
		std::size_t h6 = std::hash<quint8>{}(id.dSubClass);
		return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 4) ^ (h5 << 5) ^ (h6 << 6);
	}
};

struct DeviceManager::Impl
{
	explicit Impl(DeviceManager & manager)
		: _manager{manager}
	{
		auto usbDevices = _usb.devices();
		for (const auto & id: qAsConst(usbDevices))
		{
			DeviceType type = convert(id.vid, id.pid);
			if (DeviceType::Unknown != type)
			{
				DeviceId devId{getDeviceId(id)};
				if (!_deviceList.contains(devId))
				{
					_deviceList.append(devId);
					_connectedRealDevices[id] = devId;
				}
			}
		}
		qInfo() << "DeviceManager connected devices:" << _deviceList;

		connect(&_usb, &QUsb::deviceInserted, &_manager, [this](auto id) { onDevInserted(id); });
		connect(&_usb, &QUsb::deviceRemoved, &_manager, [this](auto id) { onDevRemoved(id); });
	}

	DeviceId getDeviceId(QUsb::Id usbId) const
	{
		// [TODO] @MJNIKOFF - add and use Hid::open with original path (several devices with the same VID && PID could
		// be connected at the same time...)
		QHidDevice hid;
		hid.open(usbId.vid, usbId.pid);

		DeviceId devId{convert(usbId.vid, usbId.pid), hid.serialNumber()};
		hid.close();

		return devId;
	}

	void onDevInserted(QUsb::Id usbId)
	{
		DeviceType type{convert(usbId.vid, usbId.pid)};
		if (DeviceType::Unknown != type && !_connectedRealDevices.contains(usbId))
		{
			DeviceId devId{getDeviceId(usbId)};
			insert(devId, &usbId);
		}
	}

	void onDevRemoved(QUsb::Id usbId)
	{
		DeviceType type{convert(usbId.vid, usbId.pid)};
		if (DeviceType::Unknown != type && _connectedRealDevices.contains(usbId))
		{
			DeviceId idToRemove = _connectedRealDevices[usbId];  // [NOTE]: explicit copy to avoid a problem with lifetime
			remove(idToRemove, &usbId);
		}
	}

	bool insert(const DeviceId & id, const QUsb::Id * usbId = nullptr)
	{
		if (!_deviceList.contains(id))
		{
			_deviceList.append(id);
			if (usbId)
			{
				_connectedRealDevices[*usbId] = id;
			}
			qInfo() << "DeviceManager device inserted:" << id;
			emit _manager.inserted(id);
			emit _manager.devicesChanged();
			return true;
		}
		else
		{
			return false;
		}
	}

	bool remove(const DeviceId & id, const QUsb::Id * usbId = nullptr)
	{
		if (_deviceList.removeOne(id))
		{
			if (usbId)
			{
				_connectedRealDevices.erase(*usbId);
			}
			qInfo() << "DeviceManager device removed:" << id;
			emit _manager.removed(id);
			emit _manager.devicesChanged();
			return true;
		}
		else
		{
			return false;
		}
	}

	IDevice * createRealDevicesInterface(DeviceType const type) const
	{
		IDevice * idevice = nullptr;
		switch (type)
		{
		case DeviceType::Mini:
			idevice = new StreamDeckMini(StreamDeckMini::PID_Mini);
			break;
		case DeviceType::MiniMK2:
			idevice = new StreamDeckMini(StreamDeckMini::PID_MiniMK2);
			break;
		case DeviceType::OriginalV2:
			idevice = new StreamDeckOriginalV2(StreamDeckOriginalV2::PID_OriginalV2);
			break;
		case DeviceType::MK2:
			idevice = new StreamDeckOriginalV2(StreamDeckOriginalV2::PID_MK2);
			break;
		case DeviceType::XL:
			idevice = new StreamDeckXL(StreamDeckXL::PID_XL);
			break;
		case DeviceType::XLV2:
			idevice = new StreamDeckXL(StreamDeckXL::PID_XLV2);
			break;
		case DeviceType::Original:
			idevice = new StreamDeckOriginal;
			break;
		case DeviceType::Pedal:
			idevice = new StreamDeckPedal;
			break;
		case DeviceType::Plus:
			idevice = new StreamDeckPlus;
			break;
		case DeviceType::Unknown:
		case DeviceType::Any:
		default:
			idevice = new DummyDevice;
			break;
		}
		return idevice;
	}

	IDevice * createInterface(DeviceId const & id) const
	{
		IDevice * idevice = nullptr;
		if (auto emulator = _emulators.find(id); emulator != _emulators.end())
		{
			idevice = emulator.value()->createInterface();
		}
		else
		{
			idevice = createRealDevicesInterface(id.type);
		}
		return idevice;
	}

	DeviceManager & _manager;
	QUsb _usb;
	DeviceIdList _deviceList;
	QMap<DeviceId, IEmulator *> _emulators;

	// QUsb::Id does not have comparision operators (and hash), so we have to implement map with key=QUsb::Id like this
	class LessUsbIdCase
	{
	public:
		bool operator()(const QUsb::Id & a, const QUsb::Id & b) const
		{
			return std::hash<QUsb::Id>{}(a) < std::hash<QUsb::Id>{}(b);
		}
	};

	std::map<QUsb::Id, DeviceId, LessUsbIdCase> _connectedRealDevices;
};

DeviceType DeviceManager::convert(quint16 vid, quint16 pid)
{
	if (vid != IDevice::VID)
	{
		return DeviceType::Unknown;
	}
	switch (pid)
	{
	case StreamDeckOriginal::PID:
		return DeviceType::Original;
	case StreamDeckMini::PID_Mini:
		return DeviceType::Mini;
	case StreamDeckMini::PID_MiniMK2:
		return DeviceType::MiniMK2;
	case StreamDeckOriginalV2::PID_MK2:
		return DeviceType::MK2;
	case StreamDeckOriginalV2::PID_OriginalV2:
		return DeviceType::OriginalV2;
	case StreamDeckPedal::PID:
		return DeviceType::Pedal;
	case StreamDeckPlus::PID:
		return DeviceType::Plus;
	case StreamDeckXL::PID_XL:
		return DeviceType::XL;
	case StreamDeckXL::PID_XLV2:
		return DeviceType::XLV2;

	default:
		return DeviceType::Unknown;
	}
}

DeviceManager * DeviceManager::instance()
{
	static DeviceManager * instance = new DeviceManager;
	return instance;
}

DeviceManager::DeviceManager()
	: _pImpl{new Impl{*this}}
{}

DeviceManager::~DeviceManager() = default;

DeviceManager::DeviceIdList DeviceManager::devices()
{
	return _pImpl->_deviceList;
}

DeviceManager::IDevice * DeviceManager::createInterface(DeviceId const & id) const
{
	return _pImpl->createInterface(id);
}

bool DeviceManager::registerEmulator(DeviceManager::IEmulator * emu)
{
	if (!emu)
	{
		qWarning() << "Could not add invalid emulator";
		return false;
	}
	auto deviceId = emu->deviceId();
	if (_pImpl->_deviceList.contains(deviceId))
	{
		qWarning() << "Could not add emulator. Device with the same deviceId" << deviceId << "is already registered";
		return false;
	}
	if (deviceId.type == DeviceType::Unknown)
	{
		qWarning() << "Could not add DeviceType::Unknown as emulator";
		return false;
	}

	bool result = _pImpl->_emulators.insert(deviceId, emu) != _pImpl->_emulators.end();

	qInfo() << "registerEmulator" << deviceId << result;
	if (result)
	{
		_pImpl->insert(deviceId);
	}

	return result;
}

void DeviceManager::unregisterEmulator(IEmulator * emu)
{
	if (emu)
	{
		auto deviceId = emu->deviceId();
		qInfo() << "unregisterEmulator" << deviceId;
		if (_pImpl->_emulators.remove(deviceId))
		{
			_pImpl->remove(deviceId);
		}
	}
}
