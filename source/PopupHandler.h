#ifndef POPUPHANDLER_H
#define POPUPHANDLER_H
#include <QtQml>
#include <QObject>

class PopupHandler : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged FINAL)

private:
    QString _message;

public:
    static PopupHandler* instance();
    explicit PopupHandler(QObject *parent = nullptr);

    QString message();
    void setMessage(QString message);

signals:
    void messageChanged();
};

#endif // POPUPHANDLER_H
