#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>
#include <QIcon>
#include "source/EventHandler.h"
#include "source/PopupHandler.h"

// HIGHLIGHT ICON COLOR: #3276C7
int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon("nterm/assets/logo.png"));

    /** SetUp Font Family **/
    QFontDatabase::addApplicationFont(":/assets/fonts/inter/InterRegular.ttf");
    QFontDatabase::addApplicationFont(":/assets/fonts/inter/Inter24ptBlack.ttf");
    QGuiApplication::setFont(QFont("Inter"));

    QQmlApplicationEngine engine;
    EventHandler eventHandler;

    QQmlContext *rootContext = engine.rootContext();
    rootContext->setContextProperty("eventHandler", &eventHandler);
    rootContext->setContextProperty("popupHandler", PopupHandler::instance());


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
