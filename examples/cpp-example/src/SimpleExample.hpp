#pragma once

#include <QtCore/QObject>
#include <QtCore/QTimer>

namespace minervous::streamdeck
{
	class Device;
	class DeviceEmulator;
}

class SimpleExample: public QObject
{
	Q_OBJECT

public:
	explicit SimpleExample(QObject * parent = nullptr);
	~SimpleExample() override;

	bool connected() const;

signals:
	void readyToClose();

private:
	QScopedPointer<minervous::streamdeck::Device> _device;
	QScopedPointer<minervous::streamdeck::DeviceEmulator> _emulator;
	QTimer _timer;


	void onConnected();
};
