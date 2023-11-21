#pragma once

#include "minervous/streamdeck/DeviceManager.hpp"

#include "QtCore/qtmetamacros.h"
#include "qqmlintegration.h"

namespace minervous::streamdeck
{

	class QmlStreamDeckManager: public QObject
	{
		Q_OBJECT
		QML_SINGLETON
		QML_NAMED_ELEMENT(StreamDeckManager)

		Q_PROPERTY(DeviceIdList devices READ devices NOTIFY devicesChanged FINAL)

	public:
		~QmlStreamDeckManager() override = default;
		QmlStreamDeckManager(QObject * parent = nullptr);

		using DeviceIdList = DeviceManager::DeviceIdList;

		DeviceIdList devices();

	signals:
		void devicesChanged();
		void inserted(DeviceId);
		void removed(DeviceId);
	};

}  // namespace minervous::streamdeck
