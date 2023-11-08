#include "SimpleExample.hpp"

#include <QtCore/QDebug>
#include <QtCore/QUrl>

#include "minervous/streamdeck/Device.hpp"
#include "minervous/streamdeck/DeviceEmulator.hpp"

namespace streamdeck = minervous::streamdeck;

SimpleExample::SimpleExample(QObject * parent)
	: QObject{parent}
	, _device{new streamdeck::Device}
	, _emulator{new streamdeck::DeviceEmulator}
{
	_device->init();

	if (_device->connected())
	{
		onConnected();
	}

	connect(
		_device.data(),
		&streamdeck::Device::buttonsStateChanged,
		this,
		[=]() { qInfo() << "buttonsStateChanged:" << _device->buttonsState(); }
	);

	connect(
		_device.data(),
		&streamdeck::Device::pressed,
		this,
		[=](int index)
		{
			qInfo() << "pressed:" << index;
			if (index < _device->keyCount() - 1)
			{
				QUrl file{"qrc:/examples/images/Pressed.png"};
				_device->setImageUrl(index, file);
			}
		}
	);

	connect(
		_device.data(),
		&streamdeck::Device::released,
		this,
		[=](int index)
		{
			qInfo() << "released:" << index;
			if (index == _device->keyCount() - 1)
			{
				_device->reset();
				// _device->close();
				emit readyToClose();
			}
			else
			{
				QUrl file{"qrc:/examples/images/Released.png"};
				_device->setImageUrl(index, file);
			}
		}
	);

	connect(
		_device.data(),
		&streamdeck::Device::connectedChanged,
		this,
		[=]()
		{
			if (_device->connected())
			{
				onConnected();
			}
			else
			{
				qInfo() << "disconnected";
			}
		}
	);

	_timer.setInterval(5000);
	_timer.connect(
		&_timer,
		&QTimer::timeout,
		this, [=]
		{
			static int cnt = -2;
			if (cnt == -2)
			{
				_emulator->init();
				_timer.setInterval(1000);
			} else if (cnt == -1)
			{
				_emulator->setConnected(true);
				if (!_emulator->connected()) {
					return;
				}
				_timer.setInterval(1000);
			} else if (cnt < 2 * (_emulator->keyCount() - 1)) {
				if (cnt & 1)
				{
					_emulator->release(cnt / 2);
				} else {
					_emulator->press(cnt / 2);
				}
			} else {
				_emulator->setConnected(false);
				_timer.setInterval(5000);
			}
			cnt++;
			if (cnt > 2 * (_emulator->keyCount() - 1)) {
				cnt = -1;
			}
		}
	);
	_timer.start();
}

bool SimpleExample::connected() const
{
	return _device->connected();
}

SimpleExample::~SimpleExample()
{
	qWarning("destructor");
}

void SimpleExample::onConnected()
{
	if (_device->open())
	{
		qInfo("_device open!");

		qInfo() << "serial number:" << _device->serialNumber() << "| manufacturer:" << _device->manufacturer()
				<< "| modelName:" << _device->modelName() << "| firmwareVersion:" << _device->firmwareVersion();

		// Reset
		_device->reset();
		qInfo() << "Reset: valid" << _device->valid();

		// brightness
		_device->setBrightness(100);
		qInfo() << "Set brightness: valid" << _device->valid();

		QUrl file{"qrc:/examples/images/Released.png"};
		QUrl fileExit{"qrc:/examples/images/Exit.png"};
		for (int i(0); i < _device->keyCount() - 1; ++i)
		{
			_device->setImageUrl(i, file);
		}
		_device->setImageUrl(_device->keyCount() - 1, fileExit);
	}
	else
	{
		qWarning("Could not open _device!");
	}
}
