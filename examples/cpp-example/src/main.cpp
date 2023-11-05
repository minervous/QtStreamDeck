#include <QCoreApplication>
#include <QTimer>

#include "SimpleExample.hpp"

int main(int argc, char * argv[])
{
	QCoreApplication a{argc, argv};

	qInfo("Example");

	SimpleExample example;
	QObject::connect(&example, SIGNAL(readyToClose()), &a, SLOT(quit()));

	// if (example.connected())
	{
		return a.exec();
	}
	// else
	// {
	// 	return 0;
	// }
}
