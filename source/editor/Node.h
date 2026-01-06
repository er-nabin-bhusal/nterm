#ifndef NODE_H
#define NODE_H

#include <QtQml>
#include <QString>
#include <QFont>

class Node
{
    Q_GADGET
    Q_PROPERTY(QString typeStr READ typeStr WRITE setTypeStr)
    Q_PROPERTY(QString text READ text WRITE setText)
    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize)
    Q_PROPERTY(int fontWeight READ fontWeight WRITE setFontWeight)
    Q_PROPERTY(qreal width READ width WRITE setWidth)

public:
    enum NodeType
    {
        Normal,
        Bold,
        NewLine
    };
    Q_ENUM(NodeType)

    Node();
    Node(const QString &type, const QString &text, int fontSize, int fontWeight, qreal width);
    Node(NodeType type, const QString &text, int fontSize, int fontWeight, qreal width);

    NodeType type() const;
    void setType(NodeType type);
    QString typeStr() const;
    void setTypeStr(const QString &type);

    static QString typeToString(NodeType type);
    static NodeType stringToType(const QString &type);
    static bool canBeInSameBlock(const Node &node1, const Node &node2);

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
    NodeType m_type;
    QString m_text;
    int m_fontSize;
    int m_fontWeight;
    qreal m_width;
};
Q_DECLARE_METATYPE(Node)

#endif // NODE_H
