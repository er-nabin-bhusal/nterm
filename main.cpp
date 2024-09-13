#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>
#include <QIcon>
#include <EventHandler.h>

// HIGHLIGHT ICON COLOR: #3276C7

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon("nterm/assets/logo.png"));

    QFontDatabase::addApplicationFont(":/assets/fonts/inter/InterRegular.ttf");

    int id = QFontDatabase::addApplicationFont(":/assets/fonts/inter/Inter24ptBlack.ttf");
    qDebug() << "value of ID: " << id;

    QGuiApplication::setFont(QFont("Inter"));


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

    engine.loadFromModule("nterm", "Main");
    return app.exec();
}
