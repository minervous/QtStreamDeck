#include "StreamDeckManager.hpp"

using namespace minervous::streamdeck;
using namespace minervous::streamdeck::qml;

Manager::Manager(QObject * parent)
	: QObject{parent}
{
	connect(DeviceManager::instance(), &DeviceManager::devicesChanged, this, &Manager::devicesChanged);
	connect(DeviceManager::instance(), &DeviceManager::inserted, this, &Manager::inserted);
	connect(DeviceManager::instance(), &DeviceManager::removed, this, &Manager::removed);
}

Manager::DeviceIdList Manager::devices()
{
	return DeviceManager::instance()->devices();
}
