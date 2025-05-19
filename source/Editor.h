#ifndef EDITOR_H
#define EDITOR_H

#include <QQuickItem>

class Editor : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QList<QVariantMap> lines READ lines WRITE setLines NOTIFY linesChanged)

private:
    QList<QVariantMap> m_lines;

    int m_currentIndex = 0;
    bool isEditing = false;
    void updateLines(const QString &text, const int &index);

signals:
    void linesChanged();

public:
    Q_INVOKABLE void onClicked();
    Q_INVOKABLE void onKeyPressed(const QString &text, const int &index);

    QList<QVariantMap> lines() const { return m_lines; }
    explicit Editor(QQuickItem *parent = nullptr);
    void setLines(const QList<QVariantMap> &lines);
};

#endif // EDITOR_H
