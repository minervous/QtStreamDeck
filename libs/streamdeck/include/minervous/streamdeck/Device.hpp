#pragma once

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QTimer>
#include <QtCore/QUrl>
#include <QtCore/QDebug>

#include <QtCore/qtmetamacros.h>

#include "StreamDeckLib_global.hpp"

class QHidDevice;

namespace minervous::streamdeck
{

	class STREAMDECKLIB_EXPORT Device: public QObject
	{
		Q_OBJECT

		Q_PROPERTY(minervous::streamdeck::DeviceType expectedDeviceType READ expectedDeviceType WRITE
					   setExpectedDeviceType NOTIFY expectedDeviceTypeChanged FINAL)
		Q_PROPERTY(minervous::streamdeck::DeviceType deviceType READ deviceType NOTIFY
					   deviceTypeChanged FINAL)
		Q_PROPERTY(bool openOnConnect READ openOnConnect WRITE setOpenOnConnect NOTIFY openOnConnectChanged FINAL)
		Q_PROPERTY(int keyColumns READ keyColumns NOTIFY configurationUpdated FINAL)
		Q_PROPERTY(int keyRows READ keyRows NOTIFY configurationUpdated FINAL)
		Q_PROPERTY(int keyCount READ keyCount NOTIFY configurationUpdated FINAL)
		Q_PROPERTY(bool hasDisplay READ hasDisplay NOTIFY configurationUpdated FINAL)

		Q_PROPERTY(QString modelName READ modelName NOTIFY connectedChanged FINAL)
		Q_PROPERTY(QString manufacturer READ manufacturer NOTIFY connectedChanged FINAL)

		Q_PROPERTY(QString serialNumber READ serialNumber WRITE setSerialNumber NOTIFY serialNumberChanged FINAL)
		Q_PROPERTY(bool isOpen READ isOpen NOTIFY isOpenChanged FINAL)
		Q_PROPERTY(QString firmwareVersion READ firmwareVersion NOTIFY connectedChanged FINAL)
		Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged FINAL)

		Q_PROPERTY(bool valid READ valid NOTIFY validChanged FINAL)

		Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged FINAL)

		Q_PROPERTY(QList<bool> buttonsState READ buttonsState NOTIFY buttonsStateChanged FINAL)

	public:
		explicit Device(QObject * parent = nullptr);
		~Device() override;

		enum DeviceType
		{
			Unknown = 0,
			Original,
			OriginalV2,
			MK2,
			Mini,
			MiniMK2,
			XL,
			XLV2,
			Pedal,
			Any
		};
		Q_ENUM(DeviceType)

		int keyColumns() const;
		int keyRows() const;
		int keyCount() const;
		QString modelName() const;
		QString firmwareVersion() const;
		QString manufacturer() const;
		bool hasDisplay() const;
		QList<bool> buttonsState() const;
		bool valid() const;
		bool connected() const;

		bool openOnConnect() const;
		void setOpenOnConnect(bool);

		QString serialNumber() const;
		void setSerialNumber(const QString & number);

		DeviceType expectedDeviceType() const;
		void setExpectedDeviceType(DeviceType deviceType);

		DeviceType deviceType() const;

		void init();

		bool isOpen();
		int brightness();
		void setBrightness(int percentage);

		bool open();
		void close();
		void reset();
		void sendImage(int keyIndex, QUrl source);

		static QString deviceTypeToString(DeviceType value);

	signals:
		void isOpenChanged();
		void brightnessChanged();
		void serialNumberChanged();
		void validChanged();
		void pressed(int index);
		void released(int index);
		void buttonsStateChanged();
		void connectedChanged();
		void expectedDeviceTypeChanged();
		void deviceTypeChanged();
		void configurationUpdated();
		void openOnConnectChanged();

	protected:
		Q_DISABLE_COPY_MOVE(Device)

	private:
		struct Impl;
		QScopedPointer<Impl> _pImpl;
	};

	using DeviceType = Device::DeviceType;

}  // namespace minervous::streamdeck

inline QTextStream & operator<<(QTextStream & outStream, const minervous::streamdeck::Device::DeviceType & value)
{
	return outStream << minervous::streamdeck::Device::deviceTypeToString(value);
}

inline QDebug & operator<<(QDebug & outStream, const minervous::streamdeck::Device::DeviceType & value)
{
	return outStream << minervous::streamdeck::Device::deviceTypeToString(value);
}
