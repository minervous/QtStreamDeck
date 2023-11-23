#include <QtCore/QCoreApplication>
#include <QtCore/QTimer>

#include "SimpleExample.hpp"

int main(int argc, char * argv[])
{
	Q_INIT_RESOURCE(aux_example_images);

	QCoreApplication a{argc, argv};

	qInfo("Example");

	SimpleExample example;
	QObject::connect(&example, &SimpleExample::readyToClose, &a, &QCoreApplication::quit);

	return a.exec();
}
