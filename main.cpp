#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <EventHandler.h>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    // if you're to register a QML component you would do this
    // qmlRegisterType<EventHandler>("EventHandler", 1, 0, "EventHandler");

    QQmlApplicationEngine engine;
    EventHandler eventHandler;

    QQmlContext *rootContext = engine.rootContext();
    rootContext->setContextProperty("eventHandler", &eventHandler);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []()
        { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.loadFromModule("qmltest", "Main");
    return app.exec();
}
