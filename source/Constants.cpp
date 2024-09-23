#include "source/Constants.h"
#include <QObject>

Constants::Constants(QObject *parent) : QObject(parent) {}

QString Constants::textColor() { return "#333333"; }
QString Constants::highlightBlue() { return "#3276C7"; }
QString Constants::textSelectionColor() { return "#cce5ff"; }
QString Constants::textSelectedColor() { return "#000"; }
