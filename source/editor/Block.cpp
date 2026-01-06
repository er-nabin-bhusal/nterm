#include "source/editor/Block.h"
#include <QFontMetrics>
#include <cmath>

Block::Block()
{
}

Block::Block(const QList<Node> &nodes) : m_nodes(nodes)
{
}

qreal Block::getTotalWidthOfBlock() const
{
    qreal totalWidth = 0.0;
    for (const Node &node : m_nodes)
    {
        totalWidth += node.width();
    }
    return totalWidth;
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

qreal Block::getTotalWidthTillIndex(int nodeIndex, int cursorPosition) const
{
    qreal totalWidth = 0.0;
    for (int i = 0; i < nodeIndex; i++)
    {
        totalWidth += m_nodes[i].width();
    }
    totalWidth += m_nodes[nodeIndex].measureTextWidth(m_nodes[nodeIndex].text().left(cursorPosition));
    return totalWidth;
}

/**
 * @brief Given the width of the text, calculate what character position it corresponds to
 * It's not going to be exact, but we need the closest character position
 *
 * @param width
 * @return QPair<int, int> <node index, character position in that node>
 */
QPair<int, int> Block::getCursorPositionFromWidth(qreal width) const
{
    int nodeIndex = 0;
    qreal currentWidth = 0.0;
    int i = 0;
    while (i < m_nodes.size())
    {
        Node node = m_nodes[i];
        if (node.type() == Node::NewLine)
            break;
        if (currentWidth + node.width() > width)
        {
            nodeIndex = i;
            int cursorPositionInNode = findSplitCharIndex(node, width - currentWidth);
            return QPair<int, int>(i, cursorPositionInNode);
        }
        currentWidth += m_nodes[i].width();
        i++;
    }
    nodeIndex = m_nodes.size() - 1;

    if (m_nodes[nodeIndex].type() == Node::NewLine)
    {
        nodeIndex = m_nodes.size() - 2;
    }

    return QPair<int, int>(nodeIndex, m_nodes[nodeIndex].text().length());
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
    Node lastNode = m_nodes[lastItemIndex];
    while (lastNode.type() == Node::NewLine) // CRITICAL: (Assumption) Assume that there is always going to a node that is NOT a new line
    {
        lastItemIndex -= 1;
        lastNode = m_nodes[lastItemIndex];
    }
    int lastCharPosition = m_nodes[lastItemIndex].text().length();
    return QPair<int, int>(lastItemIndex, lastCharPosition);
}

void Block::arrangeNodes(qreal availableRowWidth)
{
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
        if (currentNode.type() == Node::NewLine) // If it's a new line, block is complete
        {
            newNodes.append(tempNode);
            tempNode = currentNode;
            break;
        }
        if (currentNode.text().isEmpty()) // CRITICAL: it's going to remove the node that doesn't have any text
        {
            idx += 1;
            continue;
        }
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

int Block::findSplitCharIndex(Node node, qreal widthForFirstNode) const
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

Block Block::addNewLineNode(int nodeIndex, int cursorPosition)
{
    Block newBlock;
    Node currentNode = m_nodes[nodeIndex];
    Node newLineNode(Node::NewLine, "", currentNode.fontSize(), currentNode.fontWeight(), 0.0);

    if (cursorPosition == 0)
    {
        m_nodes.insert(nodeIndex, newLineNode);
        while (m_nodes.size() > nodeIndex + 1)
        {
            Node node = m_nodes[nodeIndex + 1];
            m_nodes.removeAt(nodeIndex + 1);
            newBlock.m_nodes.append(node);
        }
    }
    else if (cursorPosition == currentNode.text().length())
    {
        m_nodes.insert(nodeIndex + 1, newLineNode);
        while (m_nodes.size() > nodeIndex + 2)
        {
            Node node = m_nodes[nodeIndex + 2];
            m_nodes.removeAt(nodeIndex + 2);
            if (node.type() != Node::NewLine) // CRITICAL: There can be newline nodes at the end of the block
            {
                newBlock.m_nodes.append(node);
            }
        }
    }
    else
    {
        Node firstHalfNode = Node(currentNode.type(), currentNode.text().left(cursorPosition), currentNode.fontSize(), currentNode.fontWeight(), 0.0);
        Node secondHalfNode = Node(currentNode.type(), currentNode.text().mid(cursorPosition), currentNode.fontSize(), currentNode.fontWeight(), 0.0);
        m_nodes.removeAt(nodeIndex);
        newBlock.m_nodes.append(secondHalfNode);
        while (m_nodes.size() > nodeIndex)
        {
            Node node = m_nodes[nodeIndex];
            m_nodes.removeAt(nodeIndex);
            newBlock.m_nodes.append(node);
        }
        m_nodes.append(firstHalfNode);
        newBlock.m_nodes.append(newLineNode);
    }
    if (newBlock.m_nodes.isEmpty())
    {
        newBlock.m_nodes.append(Node(currentNode.type(), "", currentNode.fontSize(), currentNode.fontWeight(), 0.0));
    }
    if (m_nodes.size() == 1) // CRITICAL: (Assumption) If the current block has only one, the it must be a new line node
    {
        m_nodes.insert(0, Node(currentNode.type(), "", currentNode.fontSize(), currentNode.fontWeight(), 0.0));
    }
    return newBlock;
}