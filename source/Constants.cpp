#include "source/Constants.h"
#include <QObject>

static Constants* instance_ = nullptr;
Constants* Constants::instance() {
    if (!instance_) instance_ = new Constants();
    return instance_;
}

Constants::Constants(QObject *parent) : QObject(parent) {}

QString Constants::textColor() { return "#333333"; }
QString Constants::highlightBlue() { return "#3276C7"; }
QString Constants::textSelectionColor() { return "#cce5ff"; }
QString Constants::textSelectedColor() { return "#000"; }
