#include "source/editor/Node.h"
#include <QFontMetrics>

Node::Node() : m_fontSize(16), m_fontWeight(QFont::Normal), m_width(0.0)
{
}

Node::Node(const QString &type, const QString &text, int fontSize, int fontWeight, qreal width)
    : m_type(type), m_text(text), m_fontSize(fontSize), m_fontWeight(fontWeight), m_width(width)
{
    m_font = QFont();
    m_font.setFamily("Inter");
    m_font.setPixelSize(m_fontSize);
    m_font.setWeight(static_cast<QFont::Weight>(m_fontWeight));
    m_width = measureTextWidth(text);
}

QString Node::type() const
{
    return m_type;
}

void Node::setType(const QString &type)
{
    m_type = type;
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
