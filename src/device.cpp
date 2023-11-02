#include "device.h"

#include <QtUsb/QHidDevice>
#include <QtUsb/QtUsb>
#include <QDebug>
#include <QImage>

#include "devicemanager.h"
#include "devicetype.h"
#include "deviceid.h"
#include "devices/dummydeviceinterface.h"
#include "devices/streamdeckmini.h"
#include "devices/streamdeckoriginal.h"
#include "devices/streamdeckoriginalv2.h"
#include "devices/streamdeckpedal.h"
#include "devices/streamdeckxl.h"

using namespace streamdeck;

struct Device::Impl {
    explicit Impl(Device &device)
        : m_device(device)
    { }

    int readButtonsStatus();
    void setSerialNumber(const QString& number);
    void setValid(bool valid);
    void onReadTimeot();

    void setInterface(DeviceType type);

    Device &m_device;
    QString m_serialNumber;
    QString m_firmwareNumber;
    int m_brightness {70};
    DeviceInterface::Configuration m_configuration;
    QScopedPointer<QTimer> m_timer;
    bool m_valid {false};
    QList<bool> m_buttonsState;
    bool m_connected {false};
    DeviceType m_deviceType {DeviceType::STREAMDECK_ANY};
    DeviceType m_realDeviceType {DeviceType::UNKNOWN_DEVICE};

    QScopedPointer<DeviceInterface> m_interface;
};

void Device::Impl::setInterface(DeviceType type) {
    switch (type) {
    case DeviceType::STREAMDECK_MINI:
        m_interface.reset(new StreamDeckMini(StreamDeckMini::PID_MINI));
        m_realDeviceType = type;
        break;
    case DeviceType::STREAMDECK_MINI_MK2:
        m_interface.reset(new StreamDeckMini(StreamDeckMini::PID_MINI_MK2));
        m_realDeviceType = type;
        break;
    case DeviceType::STREAMDECK_ORIGINAL_V2:
        m_interface.reset(new StreamDeckOriginalV2(StreamDeckOriginalV2::PID_ORIGINAL_V2));
        m_realDeviceType = type;
        break;
    case DeviceType::STREAMDECK_MK2:
        m_interface.reset(new StreamDeckOriginalV2(StreamDeckOriginalV2::PID_MK2));
        m_realDeviceType = type;
        break;
    case DeviceType::STREAMDECK_XL:
        m_interface.reset(new StreamDeckXL(StreamDeckXL::PID_XL));
        m_realDeviceType = type;
        break;
    case DeviceType::STREAMDECK_XL_V2:
        m_interface.reset(new StreamDeckXL(StreamDeckXL::PID_XL_V2));
        m_realDeviceType = type;
        break;
    case DeviceType::STREAMDECK_ORIGINAL:
        m_interface.reset(new StreamDeckOriginal());
        m_realDeviceType = type;
        break;
    case DeviceType::STREAMDECK_PEDAL:
        m_interface.reset(new StreamDeckPedal());
        m_realDeviceType = type;
        break;
    case DeviceType::UNKNOWN_DEVICE:
    case DeviceType::STREAMDECK_ANY:
    default:
        m_interface.reset(new DummyDeviceInterface());
        m_realDeviceType = DeviceType::UNKNOWN_DEVICE;
        break;
    }
}

Device::Device(QObject *parent)
    : QObject{parent}
    , m_pImpl(new Impl(*this))
{
    connect(DeviceManager::instance(), &DeviceManager::removed, this, [=](auto id){
        qInfo() << "Device removed:" << id << m_pImpl->m_configuration.pid << m_pImpl->m_configuration.vid << "serial " << m_pImpl->m_serialNumber;
        if (id.type == DeviceManager::convert(m_pImpl->m_configuration.vid, m_pImpl->m_configuration.pid)
                               /* && (m_serialNumber.isEmpty() || m_serialNumber == id.serialNumber)*/) {
            close();
            m_pImpl->m_connected = false;
            qInfo() << "Device disconnected";
            emit connectedChanged();
            m_pImpl->setValid(false);
            if (m_pImpl->m_deviceType == DeviceType::STREAMDECK_ANY) {
                init();
            }
        }
    });

    connect(DeviceManager::instance(), &DeviceManager::inserted, this, [=](auto id){
        qInfo() << "Device inserted:" << id << "expected" << m_pImpl->m_deviceType << "real" << m_pImpl->m_realDeviceType << m_pImpl->m_configuration.pid << m_pImpl->m_configuration.vid << "serial " << m_pImpl->m_serialNumber;
        if (id.type == DeviceManager::convert(m_pImpl->m_configuration.vid, m_pImpl->m_configuration.pid)
                                /*&& (m_serialNumber.isEmpty() || m_serialNumber == id.serialNumber)*/) {
            m_pImpl->m_connected = true;
            qInfo() << "Device reconnected";
            emit connectedChanged();
        } else if (m_pImpl->m_deviceType == DeviceType::STREAMDECK_ANY) {
            init();
        }
    });
}

Device::~Device()
{
    close();
}

int Device::keyColumns() const
{
    return m_pImpl->m_configuration.keyColumns;
}

int Device::keyRows() const
{
    return m_pImpl->m_configuration.keyRows;
}

int Device::keyCount() const
{
    return m_pImpl->m_configuration.keyColumns * m_pImpl->m_configuration.keyRows;
}

bool Device::hasDisplay() const
{
    return m_pImpl->m_configuration.keyColumns;
}

QString Device::modelName() const
{
	if (m_pImpl->m_interface) {
		return m_pImpl->m_interface->product();
	} else {
		return {};
	}
}

QString Device::serialNumber() const
{
    return m_pImpl->m_serialNumber;
}

QString Device::firmwareVersion() const
{
    return m_pImpl->m_firmwareNumber;
}

QString Device::manufacturer() const
{
    return m_pImpl->m_interface->manufacturer();
}

bool Device::open()
{
    if (m_pImpl->m_interface.isNull()) {
        qInfo() << "Could not open: not initialized";
        return false;
    }
    if (!isOpen()) {
        bool result = m_pImpl->m_interface->open(QString{});//m_pImpl->m_serialNumber);
        if (result) {
            qInfo() << "Open";
            result = m_pImpl->m_interface->setBrightness(m_pImpl->m_brightness);
            if (m_pImpl->m_timer.isNull()) {
                m_pImpl->m_timer.reset(new QTimer(this));
                m_pImpl->m_timer->setInterval(40);
                connect(m_pImpl->m_timer.data(), &QTimer::timeout, this, [this]() {
                    m_pImpl->onReadTimeot();
                });
                m_pImpl->m_timer->start();
            }
        } else {
            qInfo() << "Could not open";
        }
        m_pImpl->setValid(result);
    } else {
        qInfo() << "Could not open: already open";
    }
    return isOpen();
}

bool Device::isOpen()
{
    return m_pImpl->m_interface->isOpen();
}

void Device::close()
{
    if (!m_pImpl->m_timer.isNull()) {
        m_pImpl->m_timer->stop();
        disconnect(m_pImpl->m_timer.data(), nullptr, this, nullptr);
        qInfo() << "close: timer reset";
        m_pImpl->m_timer.reset();
    }
    qInfo() << "close: call hid::close";
    m_pImpl->m_interface->close();
}


void Device::reset()
{
    if (m_pImpl->m_interface) {
        m_pImpl->m_interface->reset();
    }
}

void Device::setBrightness(int procentage)
{
    if (procentage != m_pImpl->m_brightness) {
        m_pImpl->m_brightness = procentage;
        if (isOpen() && m_pImpl->m_interface) {
            bool result = m_pImpl->m_interface->setBrightness(m_pImpl->m_brightness);
            m_pImpl->setValid(result);
        }
        emit brightnessChanged();
    }
}

int Device::brightness()
{
    return m_pImpl->m_brightness;
}

void Device::setSerialNumber(const QString& number)
{
    if (!isOpen()) {
        m_pImpl->setSerialNumber(number);
    }
}

void Device::Impl::setSerialNumber(const QString &number)
{
    if (number != m_serialNumber) {
        m_serialNumber = number;
        emit m_device.serialNumberChanged();
    }
}

void Device::init()
{
    DeviceType type {m_pImpl->m_deviceType};
    QString serial {m_pImpl->m_serialNumber};

	if (type == DeviceType::STREAMDECK_ANY) {
		const auto devices = DeviceManager::instance()->devices();
        for (const auto& id: devices) {
            if (id.type != DeviceType::UNKNOWN_DEVICE) {
                type = id.type;
                serial = id.serialNumber;
                break;
            }
        }
    }

    m_pImpl->setInterface(type);

    qInfo() << "init: expected "<< m_pImpl->m_deviceType << "real" << m_pImpl->m_realDeviceType;

    if (m_pImpl->m_interface.isNull()) {
        m_pImpl->m_configuration = DeviceInterface::Configuration();
        m_pImpl->m_buttonsState.clear();
        m_pImpl->setValid(false);
    } else {

        m_pImpl->m_configuration = m_pImpl->m_interface->getConfiguration();
        m_pImpl->m_buttonsState.clear();
        m_pImpl->m_buttonsState.fill(false, m_pImpl->m_configuration.keyColumns * m_pImpl->m_configuration.keyRows);

        bool result = m_pImpl->m_interface->open(serial);
        if (result) {
            QString serialHid = m_pImpl->m_interface->serialNumber();
            m_pImpl->m_firmwareNumber = m_pImpl->m_interface->getFirmwareVersion();
            m_pImpl->m_interface->close();
            m_pImpl->setSerialNumber(serialHid);
            emit firmwareVersionChanged();
        }
        m_pImpl->setValid(result);
    }

    bool connected = DeviceManager::instance()->devices().contains(DeviceId(m_pImpl->m_realDeviceType));//, serial));
    if (m_pImpl->m_connected != connected) {
        m_pImpl->m_connected = connected;
        emit connectedChanged();
    }
    if (m_pImpl->m_realDeviceType != m_pImpl->m_deviceType) {
        emit connectedDeviceTypeChanged();
    }
    emit configurationUpdated();
}

int Device::Impl::readButtonsStatus()
{
//    if (m_hid.isNull() || !m_hid->isOpen()) {
//        setValid(false);
//        return -1;
//    }
    QList<bool> data;
    data.fill(false, m_configuration.keyColumns * m_configuration.keyRows);
    auto result = m_interface->readButtonsStatus(data);
    if (result > 0) {
        bool changed {false};
        for (auto i=0; i < std::min(data.size(), m_buttonsState.size()); ++i) {
            if (data[i] != m_buttonsState[i]) {
                m_buttonsState[i] = data[i];
                changed = true;
                data[i] ?
                    emit m_device.pressed(i) :
                    emit m_device.released(i);
            }
        }
        if (changed) {
            emit m_device.buttonsStateChanged();
        }
    }
    return result;
}

QList<bool> Device::buttonsState() const
{
    return m_pImpl->m_buttonsState;
}

void Device::Impl::setValid(bool valid)
{
    if (valid != m_valid) {
        m_valid = valid;
        emit m_device.validChanged();
    }
}

void Device::Impl::onReadTimeot()
{
    constexpr int maxReadIterations {3};
    int cnt {maxReadIterations};
    while (readButtonsStatus() > 0 && cnt-- > 0) {
    }
}

bool Device::valid() const
{
    return m_pImpl->m_valid;
}

bool Device::connected() const
{
    return m_pImpl->m_connected;
}


streamdeck::DeviceType Device::expectedDeviceType() const
{
    return m_pImpl->m_deviceType;
}

streamdeck::DeviceType Device::connectedDeviceType() const
{
    return m_pImpl->m_realDeviceType;
}
void Device::setExpectedDeviceType(DeviceType deviceType)
{
    if (deviceType != m_pImpl->m_deviceType) {
        m_pImpl->m_deviceType = deviceType;
        if (m_pImpl->m_realDeviceType != deviceType) {
            init();
        }
        emit expectedDeviceTypeChanged();
    }
}

void Device::setImageUrl(int index, QUrl url)
{
    if (m_pImpl->m_interface.isNull()) {
        qWarning() << "Device::setImageUrl the device type is not set";
        return;
    }

    if (!isOpen()) {
        qWarning() << "Device::setImageUrl the device is not open";
        return;
    }

    if (!m_pImpl->m_configuration.hasDisplay) {
        qWarning() << "Device::setImageUrl the device does not have displays on buttons";
        return;
    }

    if (index >= keyCount()) {
        qWarning() << "Device::setImageUrl index" << index << "is out of range!";
        return;
    }

    QFile file(url.path());
    if (!file.exists()) {
        qWarning() << "Device::setImageUrl file" << url.fileName() << "is not exist!";
    } else {
        QImage imageOriginal(file.fileName());
        QBuffer bf;
        imageOriginal.scaled(m_pImpl->m_configuration.imageWidth,
                             m_pImpl->m_configuration.imageHeight,
                             Qt::KeepAspectRatio, Qt::SmoothTransformation)
            .mirrored(m_pImpl->m_configuration.imageHorizontalFlip, m_pImpl->m_configuration.imageVerticalFlip)
            .save(&bf, m_pImpl->m_configuration.imageFormat.toStdString().c_str());
        m_pImpl->m_interface->sendImage(index, bf.data());
    }
}
