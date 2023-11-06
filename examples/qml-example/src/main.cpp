#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
        QGuiApplication app(argc, argv);

        QQmlApplicationEngine engine;

		//const QUrl url(u"qrc:/qt/qml/stream-deck-qml-example/QmlDeckExample.qml"_qs);
		const QUrl url(u"/Users/imayornykov/StreamDeck/QtStreamDeck/examples/qml-example/qml/minervous/streamdeck/example/QmlDeckExample.qml"_qs);
		QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
            &app, [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);
        engine.load(url);

        return app.exec();
}
