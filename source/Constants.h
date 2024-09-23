#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QObject>

class Constants: public QObject {
    Q_OBJECT
    Q_PROPERTY(QString textColor READ textColor CONSTANT)
    Q_PROPERTY(QString highlightBlue READ highlightBlue CONSTANT)
    Q_PROPERTY(QString textSelectionColor READ textSelectionColor CONSTANT)
    Q_PROPERTY(QString textSelectedColor READ textSelectedColor CONSTANT)

public:
    Constants(QObject *parent = nullptr);
    QString textColor();
    QString highlightBlue();
    QString textSelectionColor();
    QString textSelectedColor();
};

#endif // CONSTANTS_H
