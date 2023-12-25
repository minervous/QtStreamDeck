// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include <QtCore/QObject>
#include <QtQml/QQmlEngine>

#include "minervous/streamdeck/DeviceManager.hpp"

namespace minervous::streamdeck::qml
{

	class Manager: public QObject
	{
		Q_OBJECT
		QML_SINGLETON
		QML_NAMED_ELEMENT(Manager)

		Q_PROPERTY(DeviceIdList devices READ devices NOTIFY devicesChanged FINAL)

	public:
		explicit Manager(QObject * parent = nullptr);
		~Manager() override = default;

		using DeviceIdList = DeviceManager::DeviceIdList;

		DeviceIdList devices() const;

	signals:
		void devicesChanged();
		void inserted(DeviceId);
		void removed(DeviceId);
	};

}  // namespace minervous::streamdeck::qml
