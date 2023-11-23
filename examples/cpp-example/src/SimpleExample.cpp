#include "SimpleExample.hpp"

#include <QtCore/QDebug>
#include <QtCore/QUrl>

#include "minervous/streamdeck/BaseKeyEntry.hpp"
#include "minervous/streamdeck/Device.hpp"
#include "minervous/streamdeck/DeviceEmulator.hpp"
#include "minervous/streamdeck/KeyModel.hpp"

using namespace minervous::streamdeck;

SimpleExample::SimpleExample(QObject * parent)
	: QObject{parent}
	, _device{new Device}
	, _emulator{new DeviceEmulator}
{
	auto * model = new KeyModel{this};
	QUrl normalImage{"qrc:/examples/images/Released.png"};
	QUrl pressedImage{"qrc:/examples/images/Pressed.png"};
	QUrl exitImage{"qrc:/examples/images/Exit.png"};
	const int keyCount = 15;

	for (int i = 0; i < keyCount - 1; ++i)
	{
		auto * entry = new BaseKeyEntry{model};
		entry->setImageSource(normalImage);
		connect(
			entry,
			&BaseKeyEntry::pressedChanged,
			this,
			[=]()
			{
				qInfo() << "Entry" << i << "pressedChanged" << entry->pressed();
				entry->setImageSource(entry->pressed() ? pressedImage : normalImage);
			}
		);
		model->append(entry);
	}
	auto * entry = new BaseKeyEntry{model};
	entry->setImageSource(exitImage);
	connect(
		entry,
		&BaseKeyEntry::pressedChanged,
		this,
		[=]()
		{
			qInfo() << "The last entry pressedChanged" << entry->pressed();
			entry->setImageSource(entry->pressed() ? pressedImage : exitImage);
		}
	);
	connect(
		entry,
		&BaseKeyEntry::keyReleased,
		this,
		[=]
		{
			qInfo() << "Exit on last key released";
			_device->setModel(nullptr);
			_device->reset();
			emit readyToClose();
		}
	);
	model->append(entry);

	_device->setModel(model);

	connect(
		_device.data(),
		&Device::buttonsStateChanged,
		this,
		[=]() { qInfo() << "buttonsStateChanged:" << _device->buttonsState(); }
	);

	connect(_device.data(), &Device::pressed, this, [=](int index) { qInfo() << "pressed:" << index; });

	connect(_device.data(), &Device::released, this, [=](int index) { qInfo() << "released:" << index; });

	connect(
		_device.data(),
		&Device::isOpenChanged,
		this,
		[=]
		{
			if (_device->isOpen())
			{
				qInfo() << "Example: device is open";
				qInfo() << "serial number:" << _device->serialNumber() << "| manufacturer:" << _device->manufacturer()
						<< "| modelName:" << _device->modelName() << "| firmwareVersion:" << _device->firmwareVersion();

				// Reset
				_device->reset();
				qInfo() << "Reset: valid" << _device->valid();

				// brightness
				_device->setBrightness(100);
				qInfo() << "Set brightness: valid" << _device->valid();
			}
			else
			{
				qInfo() << "Example: device is closed";
			}
		}
	);

	_device->init();

	_timer.setInterval(5000);
	_timer.connect(
		&_timer,
		&QTimer::timeout,
		[=]
		{
			static int cnt = -2;
			if (cnt == -2)
			{
				_emulator->init();
				_timer.setInterval(1000);
			}
			else if (cnt == -1)
			{
				_emulator->setConnected(true);
				if (!_emulator->connected())
				{
					return;
				}
				_timer.setInterval(1000);
			}
			else if (cnt < 2 * (_emulator->keyCount() - 1))
			{
				if (cnt & 1)
				{
					_emulator->release(cnt / 2);
				}
				else
				{
					_emulator->press(cnt / 2);
				}
			}
			else
			{
				_emulator->setConnected(false);
				_timer.setInterval(5000);
			}
			cnt++;
			if (cnt > 2 * (_emulator->keyCount() - 1))
			{
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
