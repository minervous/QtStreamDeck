// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include <QtCore/QObject>
#include <QtQml/QQmlEngine>

#include "minervous/streamdeck/DeviceManager.hpp"

namespace minervous::streamdeck::qml
{

	class DeviceManager: public QObject
	{
		Q_OBJECT
		QML_SINGLETON
		QML_NAMED_ELEMENT(DeviceManager)

		Q_PROPERTY(DeviceIdList devices READ devices NOTIFY devicesChanged FINAL)

	public:
		explicit DeviceManager(QObject * parent = nullptr);
		~DeviceManager() override = default;

		using DeviceIdList = minervous::streamdeck::DeviceManager::DeviceIdList;
		DeviceIdList devices() const;

	signals:
		void devicesChanged();
		void inserted(DeviceId deviceId);
		void removed(DeviceId deviceId);
	};

}  // namespace minervous::streamdeck::qml
