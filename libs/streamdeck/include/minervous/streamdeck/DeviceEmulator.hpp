#pragma once

#include <QtCore/QObject>

#include "Device.hpp"
#include "StreamDeckLib_global.hpp"

namespace minervous::streamdeck
{

	class STREAMDECKLIB_EXPORT DeviceEmulator: public QObject
	{
		Q_OBJECT

		Q_PROPERTY(DeviceType deviceType READ deviceType WRITE setDeviceType NOTIFY configurationUpdated FINAL)

		Q_PROPERTY(bool connected READ connected WRITE setConnected NOTIFY connectedChanged FINAL)

		Q_PROPERTY(bool isOpen READ isOpen NOTIFY isOpenChanged FINAL)
		Q_PROPERTY(int keyColumns READ keyColumns NOTIFY configurationUpdated FINAL)
		Q_PROPERTY(int keyRows READ keyRows NOTIFY configurationUpdated FINAL)
		Q_PROPERTY(int keyCount READ keyCount NOTIFY configurationUpdated FINAL)
		Q_PROPERTY(bool hasDisplay READ hasDisplay NOTIFY configurationUpdated FINAL)
		Q_PROPERTY(QString manufacturer READ manufacturer WRITE setManufacturer NOTIFY configurationUpdated FINAL)
		Q_PROPERTY(QString serialNumber READ serialNumber WRITE setSerialNumber NOTIFY configurationUpdated FINAL)
		Q_PROPERTY(QString modelName READ modelName WRITE setModelName NOTIFY configurationUpdated FINAL)
		Q_PROPERTY(QString firmwareVersion READ firmwareVersion WRITE setFirmwareVersion NOTIFY configurationUpdated
					   FINAL)

		Q_PROPERTY(int brightness READ brightness NOTIFY brightnessChanged FINAL)

	public:
		DeviceEmulator(QObject * parent = nullptr);
		virtual ~DeviceEmulator();

		bool isOpen() const;
		bool connected() const;
		int brightness() const;
		QString manufacturer() const;
		QString serialNumber() const;
		QString modelName() const;
		QString firmwareVersion() const;
		DeviceType deviceType() const;
		int keyColumns() const;
		int keyRows() const;
		int keyCount() const;
		bool hasDisplay() const;

		void setConnected(bool);
		void setManufacturer(QString);
		void setModelName(QString);
		void setSerialNumber(QString);
		void setFirmwareVersion(QString);
		void setDeviceType(DeviceType);

		void press(int index);
		void release(int index);

		void init();
	signals:
		void configurationUpdated();
		void connectedChanged();
		;

		void isOpenChanged();
		void brightnessChanged();

		void resetCalled();
		void imageSent(int keyIndex, const QByteArray & imageDataBin, const QString & imageDataBase64);

	protected:
		Q_DISABLE_COPY_MOVE(DeviceEmulator)

		struct Impl;
		QScopedPointer<Impl> _pImpl;
	};

}  // namespace minervous::streamdeck
