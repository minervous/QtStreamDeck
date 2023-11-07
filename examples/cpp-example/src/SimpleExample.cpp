#include "SimpleExample.hpp"

#include <QtCore/QDebug>
#include <QtCore/QUrl>

#include "minervous/streamdeck/Device.hpp"
#include "minervous/streamdeck/DeviceEmulator.hpp"

namespace streamdeck = minervous::streamdeck;

SimpleExample::SimpleExample(QObject * parent)
	: QObject(parent)
	, device(new streamdeck::Device(this))
	, emulator(new streamdeck::DeviceEmulator(this))
{
	device->init();

	if (device->connected())
	{
		onConnected();
	}

	connect(
		device.data(),
		&streamdeck::Device::buttonsStateChanged,
		this,
		[=]() { qInfo() << "buttonsStateChanged:" << device->buttonsState(); }
	);

	connect(
		device.data(),
		&streamdeck::Device::pressed,
		this,
		[=](int index)
		{
			qInfo() << "pressed:" << index;
			if (index < device->keyCount() - 1)
			{
				QUrl file{"qrc:/examples/images/Pressed.png"};
				device->setImageUrl(index, file);
			}
		}
	);

	connect(
		device.data(),
		&streamdeck::Device::released,
		this,
		[=](int index)
		{
			qInfo() << "released:" << index;
			if (index == device->keyCount() - 1)
			{
				device->reset();
				// device->close();
				emit readyToClose();
			}
			else
			{
				QUrl file{"qrc:/examples/images/Released.png"};
				device->setImageUrl(index, file);
			}
		}
	);

	connect(
		device.data(),
		&streamdeck::Device::connectedChanged,
		this,
		[=]()
		{
			if (device->connected())
			{
				onConnected();
			}
			else
			{
				qInfo() << "disconnected";
			}
		}
	);

	timer.setInterval(5000);
	timer.connect(
		&timer,
		&QTimer::timeout,
		this, [=]
		{
			static int cnt = -2;
			if (cnt == -2)
			{
				emulator->init();
				timer.setInterval(1000);
			} else if (cnt == -1)
			{
				emulator->setConnected(true);
				if (!emulator->connected()) {
					return;
				}
				timer.setInterval(1000);
			} else if (cnt < 2 * (emulator->keyCount() - 1)) {
				if (cnt & 1)
				{
					emulator->release(cnt / 2);
				} else {
					emulator->press(cnt / 2);
				}
			} else {
				emulator->setConnected(false);
				timer.setInterval(5000);
			}
			cnt++;
			if (cnt > 2 * (emulator->keyCount() - 1)) {
				cnt = -1;
			}
		}
	);
	timer.start();
}

bool SimpleExample::connected() const
{
	return device->connected();
}

SimpleExample::~SimpleExample()
{
	qWarning("destructor");
}

void SimpleExample::onConnected()
{
	if (device->open())
	{
		qInfo("Device open!");

		qInfo() << "serial number:" << device->serialNumber() << "| manufacturer:" << device->manufacturer()
				<< "| modelName:" << device->modelName() << "| firmwareVersion:" << device->firmwareVersion();

		// Reset
		device->reset();
		qInfo() << "Reset: valid" << device->valid();

		// brightness
		device->setBrightness(100);
		qInfo() << "Set brightness: valid" << device->valid();

		QUrl file{"qrc:/examples/images/Released.png"};
		QUrl fileExit{"qrc:/examples/images/Exit.png"};
		for (int i(0); i < device->keyCount() - 1; ++i)
		{
			device->setImageUrl(i, file);
		}
		device->setImageUrl(device->keyCount() - 1, fileExit);
	}
	else
	{
		qWarning("Could not open device!");
	}
}
