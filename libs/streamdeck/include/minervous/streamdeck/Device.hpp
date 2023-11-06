#pragma once

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtCore/QTimer>
#include <QtCore/QUrl>

#include <QtCore/qtmetamacros.h>

#include "DeviceType.hpp"
#include "StreamDeckLib_global.hpp"

class QHidDevice;

namespace minervous::streamdeck
{

	class STREAMDECKLIB_EXPORT Device: public QObject
	{
		Q_OBJECT

		Q_PROPERTY(minervous::streamdeck::DeviceType expectedDeviceType READ expectedDeviceType WRITE
					   setExpectedDeviceType NOTIFY expectedDeviceTypeChanged FINAL)
		Q_PROPERTY(minervous::streamdeck::DeviceType connectedDeviceType READ connectedDeviceType NOTIFY
					   connectedDeviceTypeChanged FINAL)
		Q_PROPERTY(int keyColumns READ keyColumns NOTIFY configurationUpdated FINAL)
		Q_PROPERTY(int keyRows READ keyRows NOTIFY configurationUpdated FINAL)
		Q_PROPERTY(int keyCount READ keyCount NOTIFY configurationUpdated FINAL)
		Q_PROPERTY(bool hasDisplay READ hasDisplay NOTIFY configurationUpdated FINAL)

		Q_PROPERTY(QString modelName READ modelName CONSTANT FINAL)
		Q_PROPERTY(QString manufacturer READ manufacturer CONSTANT FINAL)

		Q_PROPERTY(QString serialNumber READ serialNumber WRITE setSerialNumber NOTIFY serialNumberChanged FINAL)
		Q_PROPERTY(bool isOpen READ isOpen NOTIFY isOpenChanged FINAL)
		Q_PROPERTY(QString firmwareVersion READ firmwareVersion NOTIFY firmwareVersionChanged FINAL)
		Q_PROPERTY(int brightness READ brightness WRITE setBrightness NOTIFY brightnessChanged FINAL)

		Q_PROPERTY(bool valid READ valid NOTIFY validChanged FINAL)

		Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged FINAL)

		Q_PROPERTY(QList<bool> buttonsState READ buttonsState NOTIFY buttonsStateChanged FINAL)

	public:
		explicit Device(QObject * parent = nullptr);
		~Device() override;

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

		QString serialNumber() const;
		void setSerialNumber(const QString & number);

		DeviceType expectedDeviceType() const;
		void setExpectedDeviceType(DeviceType deviceType);

		DeviceType connectedDeviceType() const;

		Q_INVOKABLE void init();

		bool isOpen();
		int brightness();
		void setBrightness(int percentage);

		Q_INVOKABLE bool open();
		Q_INVOKABLE void close();
		Q_INVOKABLE void reset();
		Q_INVOKABLE void setImageUrl(int index, QUrl url);

	signals:
		void isOpenChanged();
		void brightnessChanged();
		void serialNumberChanged();
		void validChanged();
		void firmwareVersionChanged();
		void pressed(int index);
		void released(int index);
		void buttonsStateChanged();
		void connectedChanged();
		void expectedDeviceTypeChanged();
		void connectedDeviceTypeChanged();
		void configurationUpdated();

	protected:
		Q_DISABLE_COPY_MOVE(Device)

	private:
		struct Impl;
		QScopedPointer<Impl> m_pImpl;
	};

}  // namespace minervous::streamdeck
