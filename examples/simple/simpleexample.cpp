#include "simpleexample.h"

#include <QDebug>
#include <QUrl>
#include <device.h>

using namespace streamdeck;

SimpleExample::SimpleExample(QObject *parent)
    : QObject(parent)
    , device(new streamdeck::Device(this))
{
    device->init();

    if (device->connected()) {
        onConnected();
    }

    connect(device.data(), &streamdeck::Device::buttonsStateChanged, this, [=]() {
        qInfo() << "buttonsStateChanged:" << device->buttonsState();
    });

    connect(device.data(), &streamdeck::Device::pressed, this, [=](int index) {
        qInfo() << "pressed:" << index;
        if (index < device->keyCount() - 1) {
            QUrl file {":/images/Pressed.png"};
            device->setImageUrl(index, file);
        }
    });

    connect(device.data(), &streamdeck::Device::released, this, [=](int index) {
        qInfo() << "released:" << index;
        if (index == device->keyCount() -1) {
            device->reset();
            //device->close();
            emit readyToClose();
        } else {
            QUrl file {":/images/Released.png"};
            device->setImageUrl(index, file);
        }
    });

    connect(device.data(), &Device::connectedChanged, this, [=]() {
        if (device->connected()) {
            onConnected();
        } else {
            qInfo() << "disconnected";
        }
    });
}
bool SimpleExample::conneceted() const
{
    return device->connected();
}

SimpleExample::~SimpleExample()
{
    qWarning("destructor");
}

void SimpleExample::onConnected()
{
    if (device->open()) {
        qInfo("Device open!");

        qInfo() << "serial number:" << device->serialNumber()
                << "| manufacturer:" << device->manufacturer()
                << "| modelName:" << device->modelName()
                << "| firmwareVersion:" << device->firmwareVersion();

        // Reset
        device->reset();
        qInfo() << "Reset: valid" << device->valid();

        // brightness
        device->setBrightness(100);
        qInfo() << "Set brightness: valid" << device->valid();

        QUrl file {":/images/Released.png"};
        QUrl fileExit {":/images/Exit.png"};
        for (int i(0); i < device->keyCount() - 1; ++i) {
            device->setImageUrl(i, file);
        }
        device->setImageUrl(device->keyCount() - 1, fileExit);
    } else {
        qWarning("Could not open device!");
    }
}
