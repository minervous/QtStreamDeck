// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

#pragma once

#include <QtCore/QObject>

#include "Device.hpp"
#include "LibExport.hpp"

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
		~DeviceEmulator() override;

		int brightness() const;
		bool hasDisplay() const;
		bool isOpen() const;
		int keyCount() const;
		int keyColumns() const;
		int keyRows() const;

		bool connected() const;
		void setConnected(bool);

		DeviceType deviceType() const;
		void setDeviceType(DeviceType);

		QString firmwareVersion() const;
		void setFirmwareVersion(QString);

		QString manufacturer() const;
		void setManufacturer(QString);

		QString modelName() const;
		void setModelName(QString);

		QString serialNumber() const;
		void setSerialNumber(QString);

	public slots:
		void press(int index);
		void release(int index);

		void init();

	signals:
		void configurationUpdated();
		void connectedChanged();

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
