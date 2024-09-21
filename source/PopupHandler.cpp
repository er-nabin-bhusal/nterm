#include "source/PopupHandler.h"
#include <QDebug>

PopupHandler* instance_=nullptr;

PopupHandler* PopupHandler::instance() {
    if (!instance_) instance_ = new PopupHandler();
    return instance_;
}

PopupHandler::PopupHandler(QObject *parent) : QObject(parent) {}

QString PopupHandler::message() { return _message; }
void PopupHandler::setMessage(QString message) { _message = message; }
