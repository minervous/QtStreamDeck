#pragma once

#include <QtCore/QObject>

namespace minervous::streamdeck
{
	class Device;
}

class SimpleExample: public QObject
{
	Q_OBJECT

public:
	explicit SimpleExample(QObject * parent = Q_NULLPTR);
	~SimpleExample(void) override;

	bool connected() const;

signals:
	void readyToClose();

private:
	QScopedPointer<minervous::streamdeck::Device> device;

	void onConnected();
};
