#ifndef NODE_H
#define NODE_H

#include <QtQml>
#include <QString>
#include <QFont>

class Node
{
    Q_GADGET
    Q_PROPERTY(QString type READ type WRITE setType)
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize)
    Q_PROPERTY(int fontWeight READ fontWeight WRITE setFontWeight)
    Q_PROPERTY(qreal width READ width WRITE setWidth)

public:
    Node();
    Node(const QString &type, const QString &text, int fontSize, int fontWeight, qreal width);

    QString type() const;
    void setType(const QString &type);

    QString text() const;
    void setText(const QString &text);

    int fontSize() const;
    void setFontSize(int fontSize);

    int fontWeight() const;
    void setFontWeight(int fontWeight);

    qreal width() const;
    void setWidth(qreal width);

    qreal measureTextWidth(const QString &text) const;

private:
    QFont m_font;
    QString m_type;
    QString m_text;
    int m_fontSize;
    int m_fontWeight;
    qreal m_width;
};
Q_DECLARE_METATYPE(Node)

#endif // NODE_H

