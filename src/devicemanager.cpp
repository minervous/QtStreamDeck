#include "devicemanager.h"

#include <QtUsb/QHidDevice>
#include <QDebug>
#include "devices/streamdeckmini.h"
#include "devices/streamdeckoriginal.h"
#include "devices/streamdeckoriginalv2.h"
#include "devices/streamdeckpedal.h"
#include "devices/streamdeckxl.h"
using namespace streamdeck;

DeviceType DeviceManager::convert(quint16 vid, quint16 pid) {
    if (vid != DeviceInterface::VID) {
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

DeviceManager *DeviceManager::instance()
{
    static DeviceManager* instance = new DeviceManager();
    return instance;
}

DeviceManager *DeviceManager::create(QQmlEngine *, QJSEngine *)
{
    return instance();
}

DeviceId DeviceManager::getDeviceId(QUsb::Id id) const
{
	// [TODO] @MJNIKOFF - add and use Hid::open with original path (several devices with the same VID && PID could be connected at the same time...)
	// [TODO] @MJNIKOFF - uncomment when fixed issue in HID::open with predefined Serial
	//QHidDevice hid;
	//hid.open(id.vid, id.pid);
	DeviceId devId {convert(id.vid, id.pid)};//, hid.serialNumber()}; - commented due to issue in HID::open with predefined Serial
	//hid.close();

	return devId;
}

DeviceManager::DeviceManager()
{
    auto usbDevices {m_usb.devices()};
    for (const auto &id: qAsConst(usbDevices)) {
        DeviceType type {convert(id.vid, id.pid)};
        if (DeviceType::UNKNOWN_DEVICE != type) {
            DeviceId devId {getDeviceId(id)};
            if (!m_deviceList.contains(devId)) {
                m_deviceList.append(devId);
            }
        }
    }
    qInfo() << "DeviceManager connected devices:" <<  m_deviceList;

    connect(&m_usb, &QUsb::deviceInserted, this, &DeviceManager::onDevInserted);
    connect(&m_usb, &QUsb::deviceRemoved, this, &DeviceManager::onDevRemoved);
}

DeviceManager::DeviceIdList DeviceManager::devices()
{
    return m_deviceList;
}

DeviceId DeviceManager::createDeviceId(DeviceType type, QString serialNumber)
{
    return DeviceId(type, serialNumber);
}

void DeviceManager::onDevInserted(QUsb::Id id)
{
    DeviceType type {convert(id.vid, id.pid)};
    if (DeviceType::UNKNOWN_DEVICE != type) {
        QHidDevice hid;
        DeviceId devId {getDeviceId(id)};
        if (!m_deviceList.contains(devId)) {
            m_deviceList.append(devId);
            qInfo() << "DeviceManager device inserted:" <<  devId;
            emit inserted(devId);
            emit devicesChanged();
        }
    }
}

void DeviceManager::onDevRemoved(QUsb::Id id)
{
    DeviceType type {convert(id.vid, id.pid)};
    if (DeviceType::UNKNOWN_DEVICE != type) {
        // [TODO] @MJNIKOFF - store QUsb::Id in the list, then check id = id -> yopu can use correct SerialNumber for related deviceId
        DeviceId devId {type};//, serialNumber()};
        if (m_deviceList.removeOne(devId)) {
            qInfo() << "DeviceManager device removed:" <<  devId;
            emit removed(devId);
            emit devicesChanged();
        }
    }
}
