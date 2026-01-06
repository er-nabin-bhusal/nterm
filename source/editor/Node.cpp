#include "source/editor/Node.h"
#include <QFontMetrics>

Node::Node() : m_type(Normal), m_fontSize(16), m_fontWeight(QFont::Normal), m_width(0.0)
{
}

Node::Node(NodeType type, const QString &text, int fontSize, int fontWeight, qreal width)
    : m_type(type), m_text(text), m_fontSize(fontSize), m_fontWeight(fontWeight), m_width(width)
{
    m_font = QFont();
    m_font.setFamily("Inter");
    m_font.setPixelSize(m_fontSize);
    m_font.setWeight(static_cast<QFont::Weight>(m_fontWeight));
    m_width = measureTextWidth(text);
}

Node::NodeType Node::type() const
{
    return m_type;
}

void Node::setType(NodeType type)
{
    m_type = type;
}

QString Node::typeStr() const
{
    return typeToString(m_type);
}

void Node::setTypeStr(const QString &type)
{
    m_type = stringToType(type);
}

QString Node::typeToString(NodeType type)
{
    switch (type)
    {
    case Normal:
        return "normal";
    case Bold:
        return "bold";
    case NewLine:
        return "newLine";
    default:
        return "normal";
    }
}

Node::NodeType Node::stringToType(const QString &type)
{
    if (type == "normal")
    {
        return Normal;
    }
    else if (type == "bold")
    {
        return Bold;
    }
    else if (type == "newLine")
    {
        return NewLine;
    }
    return Normal; // Default fallback
}

QString Node::text() const
{
    return m_text;
}

void Node::setText(const QString &text)
{
    m_text = text;
    m_width = measureTextWidth(text);
}

int Node::fontSize() const
{
    return m_fontSize;
}

void Node::setFontSize(int fontSize)
{
    m_fontSize = fontSize;
}

int Node::fontWeight() const
{
    return m_fontWeight;
}

void Node::setFontWeight(int fontWeight)
{
    m_fontWeight = fontWeight;
}

qreal Node::width() const
{
    return m_width;
}

void Node::setWidth(qreal width)
{
    m_width = width;
}

qreal Node::measureTextWidth(const QString &text) const
{
    QFontMetrics fontMetrics(m_font);
    return fontMetrics.horizontalAdvance(text);
}

bool Node::canBeInSameBlock(const Node &node1, const Node &node2)
{
    return node1.fontSize() == node2.fontSize();
}
