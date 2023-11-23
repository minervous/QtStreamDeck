#include <QtGui/QGuiApplication>
#include <QtQml/QQmlApplicationEngine>

int main(int argc, char * argv[])
{
	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;

	QObject::connect(
		&engine,
		&QQmlApplicationEngine::objectCreationFailed,
		&app,
		[] { QCoreApplication::exit(EXIT_FAILURE); },
		Qt::QueuedConnection
	);
	engine.loadFromModule("minervous.streamdeck.example", "QmlDeckExample");

	return app.exec();
}
