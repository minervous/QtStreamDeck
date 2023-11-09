#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>

#include "SimpleExample.hpp"

int main(int argc, char * argv[])
{
	QCoreApplication a{argc, argv};

	qInfo("Example");

	SimpleExample example;
	QObject::connect(&example, &SimpleExample::readyToClose, &a, &QCoreApplication::quit);

	return a.exec();
}
