// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#include "Manager.hpp"

using namespace minervous::streamdeck::qml;
using minervous::streamdeck::DeviceManager;

Manager::Manager(QObject * parent)
	: QObject{parent}
{
	connect(DeviceManager::instance(), &DeviceManager::devicesChanged, this, &Manager::devicesChanged);
	connect(DeviceManager::instance(), &DeviceManager::inserted, this, &Manager::inserted);
	connect(DeviceManager::instance(), &DeviceManager::removed, this, &Manager::removed);
}

Manager::DeviceIdList Manager::devices() const
{
	return DeviceManager::instance()->devices();
}
