// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#include "DeviceManager.hpp"

using namespace minervous::streamdeck::qml;
namespace lib = minervous::streamdeck;

DeviceManager::DeviceManager(QObject * parent)
	: QObject{parent}
{
	connect(lib::DeviceManager::instance(), &lib::DeviceManager::devicesChanged, this, &DeviceManager::devicesChanged);
	connect(lib::DeviceManager::instance(), &lib::DeviceManager::inserted, this, &DeviceManager::inserted);
	connect(lib::DeviceManager::instance(), &lib::DeviceManager::removed, this, &DeviceManager::removed);
}

DeviceManager::DeviceIdList DeviceManager::devices() const
{
	return lib::DeviceManager::instance()->devices();
}
