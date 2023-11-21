#include "QmlStreamDeckManager.hpp"

using namespace minervous::streamdeck;

QmlStreamDeckManager::DeviceIdList QmlStreamDeckManager::devices()
{
	return DeviceManager::instance()->devices();
}

QmlStreamDeckManager::QmlStreamDeckManager(QObject * parent)
	: QObject{parent}
{
	connect(DeviceManager::instance(), &DeviceManager::devicesChanged, this, &QmlStreamDeckManager::devicesChanged);
	connect(DeviceManager::instance(), &DeviceManager::inserted, this, &QmlStreamDeckManager::inserted);
	connect(DeviceManager::instance(), &DeviceManager::removed, this, &QmlStreamDeckManager::removed);
}
