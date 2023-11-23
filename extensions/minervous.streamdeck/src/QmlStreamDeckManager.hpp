#pragma once

#include <QtCore/QObject>
#include <QtQml/QQmlEngine>

#include "minervous/streamdeck/DeviceManager.hpp"

namespace minervous::streamdeck
{

	class QmlStreamDeckManager: public QObject
	{
		Q_OBJECT
		QML_SINGLETON
		QML_NAMED_ELEMENT(StreamDeckManager)

		Q_PROPERTY(DeviceIdList devices READ devices NOTIFY devicesChanged FINAL)

	public:
		explicit QmlStreamDeckManager(QObject * parent = nullptr);
		~QmlStreamDeckManager() override = default;

		using DeviceIdList = DeviceManager::DeviceIdList;

		DeviceIdList devices();

	signals:
		void devicesChanged();
		void inserted(DeviceId);
		void removed(DeviceId);
	};

}  // namespace minervous::streamdeck
