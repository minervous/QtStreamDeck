#pragma once

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtQml/QQmlEngine>
#include <QtUsb/QUsb>

#include "DeviceId.hpp"
#include "StreamDeckLib_global.hpp"

#include "QtQml/qqmlengine.h"

namespace minervous::streamdeck
{
	class IDevice;
	class IEmulator;

	class STREAMDECKLIB_EXPORT DeviceManager: public QObject
	{
		Q_OBJECT

		Q_PROPERTY(DeviceIdList devices READ devices NOTIFY devicesChanged FINAL)

	public:
		static DeviceManager * instance();
		static DeviceManager * create(QQmlEngine *, QJSEngine *);
		typedef QList<DeviceId> DeviceIdList;

		DeviceIdList devices();

		static Q_INVOKABLE DeviceId createDeviceId(DeviceType type, QString serialNumber = {});
		static DeviceType convert(quint16 vid, quint16 pid);

		// [TODO] @MJNIKOFF - Move IDevice/IEmulator classes into DeviceManager, or move them to Public API
		using IDevice = minervous::streamdeck::IDevice;
		using IEmulator = minervous::streamdeck::IEmulator;

		// The IDevice instance's lifecycle should be controlled by caller of the function
		IDevice * createInterface(DeviceId);

		bool registerEmulator(IEmulator *);
		void unregisterEmulator(IEmulator *);

	signals:
		void devicesChanged();
		void inserted(DeviceId);
		void removed(DeviceId);

	private:
		DeviceManager();
		Q_DISABLE_COPY_MOVE(DeviceManager)

		DeviceId getDeviceId(QUsb::Id id) const;
		void onDevInserted(QUsb::Id id);
		void onDevRemoved(QUsb::Id id);
		void insert(DeviceId id);
		void remove(DeviceId id);
		QUsb m_usb;
		DeviceIdList m_deviceList;
		QMap<DeviceId, IEmulator *> m_emulators;
	};

}  // namespace minervous::streamdeck
