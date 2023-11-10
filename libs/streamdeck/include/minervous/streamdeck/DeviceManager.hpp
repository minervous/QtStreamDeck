#pragma once

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtQml/QQmlEngine>
#include <QtCore/QScopedPointer>

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
		~DeviceManager() override;

		static DeviceManager * instance();
		static DeviceManager * create(QQmlEngine *, QJSEngine *);
		typedef QList<DeviceId> DeviceIdList;

		DeviceIdList devices();

		static DeviceType convert(quint16 vid, quint16 pid);

		// [TODO] @MJNIKOFF - Move IDevice/IEmulator classes into DeviceManager, or move them to Public API
		using IDevice = minervous::streamdeck::IDevice;
		using IEmulator = minervous::streamdeck::IEmulator;

		// The IDevice instance's lifecycle should be controlled by caller of the function
		IDevice * createInterface(DeviceId) const;

		bool registerEmulator(IEmulator *);
		void unregisterEmulator(IEmulator *);

	signals:
		void devicesChanged();
		void inserted(DeviceId);
		void removed(DeviceId);

	private:
		DeviceManager();
		Q_DISABLE_COPY_MOVE(DeviceManager)

		struct Impl;
		QScopedPointer<Impl> _pImpl;
	};

}  // namespace minervous::streamdeck
