#include "source/editor/Block.h"
#include <QFontMetrics>
#include <cmath>

Block::Block()
{
}

Block::Block(const QList<Node> &nodes) : m_nodes(nodes)
{
}

/* Get the total number of characters till the index */
int Block::getTotalCharactersCountTillIndex(int index) const
{
    int totalCount = 0;
    for (int i = 0; i < index; i++)
    {
        totalCount += m_nodes[i].text().length();
    }
    return totalCount;
}

QPair<int, int> Block::getCursorPositionInBlock(int cursorPositionInBlock) const
{
    int charCount = 0;
    for (int i = 0; i < m_nodes.size(); i++)
    {
        if (charCount + m_nodes[i].text().length() >= cursorPositionInBlock)
        {
            return QPair<int, int>(i, cursorPositionInBlock - charCount);
        }
        charCount += m_nodes[i].text().length();
    }
    return QPair<int, int>(m_nodes.size() - 1, charCount - m_nodes[m_nodes.size() - 1].text().length());
}

QPair<int, int> Block::getLastPositionInBlock() const
{
    if (m_nodes.isEmpty())
    {
        return QPair<int, int>(-1, 0);
    }
    int lastItemIndex = m_nodes.size() - 1;
    int lastCharPosition = m_nodes[lastItemIndex].text().length();
    return QPair<int, int>(lastItemIndex, lastCharPosition);
}

void Block::arrangeNodes(qreal availableMaxWidth)
{
    qreal availableRowWidth = availableMaxWidth - 5;
    if (m_nodes.isEmpty())
        return;

    QList<Node> newNodes;
    Node firstNode = m_nodes[0];
    Node tempNode(firstNode.type(), "", firstNode.fontSize(), firstNode.fontWeight(), 0.0);

    int idx = 0;
    qreal currentRowWidth = 0.0;

    while (true)
    {
        if (currentRowWidth > availableRowWidth)
        {
            int splitCharIndex = findSplitCharIndex(tempNode, availableRowWidth);
            QString rightText = tempNode.text().mid(splitCharIndex);
            tempNode.setText(tempNode.text().left(splitCharIndex));
            newNodes.append(tempNode);
            tempNode = Node(tempNode.type(), rightText, tempNode.fontSize(), tempNode.fontWeight(), 0.0);
            currentRowWidth = tempNode.width();
            continue;
        }
        if (idx >= m_nodes.size())
        {
            break;
        }
        Node currentNode = m_nodes[idx];

        if (tempNode.type() != currentNode.type())
        {
            newNodes.append(tempNode);
            tempNode = Node(currentNode.type(), "", currentNode.fontSize(), currentNode.fontWeight(), 0.0);
            continue;
        }
        if (currentRowWidth + currentNode.width() > availableRowWidth)
        {
            int splitCharIndex = findSplitCharIndex(currentNode, availableRowWidth - currentRowWidth);
            tempNode.setText(tempNode.text() + currentNode.text().left(splitCharIndex));
            newNodes.append(tempNode);
            tempNode = Node(currentNode.type(), currentNode.text().mid(splitCharIndex), currentNode.fontSize(), currentNode.fontWeight(), 0.0);
            currentRowWidth = tempNode.width();
        }
        else
        {
            currentRowWidth += currentNode.width();
            tempNode.setText(tempNode.text() + currentNode.text());
        }
        idx += 1;
    }
    newNodes.append(tempNode);
    m_nodes = newNodes;
}

QVariantList Block::items() const
{
    QVariantList result;
    for (const Node &node : m_nodes)
    {
        result.append(QVariant::fromValue(node));
    }
    return result;
}

void Block::setItems(const QVariantList &items)
{
    m_nodes.clear();
    for (const QVariant &item : items)
    {
        if (item.canConvert<Node>())
        {
            m_nodes.append(item.value<Node>());
        }
    }
}

// Direct access to nodes (for C++ code)
QList<Node> &Block::nodes()
{
    return m_nodes;
}

const QList<Node> &Block::nodes() const
{
    return m_nodes;
}

int Block::findSplitCharIndex(Node node, qreal widthForFirstNode)
{
    /* Binary search to find the index of the character to split the node at */
    const QString &text = node.text();
    if (text.isEmpty())
    {
        return 0;
    }

    int left = 0, right = text.length();
    const qreal epsilon = 0.1; // Tolerance for floating point comparison

    while (left < right)
    {
        int mid = (left + right + 1) / 2;
        qreal width = node.measureTextWidth(text.left(mid));

        // Use epsilon for floating point comparison
        if (std::abs(width - widthForFirstNode) < epsilon)
        {
            return mid;
        }
        else if (width < widthForFirstNode)
        {
            left = mid;
        }
        else
        {
            right = mid - 1;
        }
    }
    return left;
}
