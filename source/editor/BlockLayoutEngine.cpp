#include "source/editor/BlockLayoutEngine.h"
#include <QDebug>

BlockLayoutEngine::BlockLayoutEngine(QObject *parent)
    : QObject(parent), m_availableWidth(0.0)
{
}

void BlockLayoutEngine::textChanged(const QString &text, int blockIndex, int itemIndex, int cursorPosition)
{
    Block &block = m_blocks[blockIndex];
    QList<Node> &nodes = block.nodes();

    Node &node = nodes[itemIndex];
    node.setText(text);
    int cursorPositionInBlock = block.getTotalCharactersCountTillIndex(itemIndex) + cursorPosition;
    block.arrangeNodes(m_availableWidth);
    QPair<int, int> cursorPositionInBlockPair = block.getCursorPositionInBlock(cursorPositionInBlock);
    emit blockChanged(blockIndex);
    emit cursorPositionChanged(blockIndex, cursorPositionInBlockPair.first, cursorPositionInBlockPair.second);
}

QVariant BlockLayoutEngine::getBlock(int blockIndex) const
{
    if (blockIndex < 0 || blockIndex >= m_blocks.size())
        return QVariant();
    return QVariant::fromValue(m_blocks[blockIndex]);
}

void BlockLayoutEngine::setAvailableWidth(qreal width)
{
    if (qFuzzyCompare(m_availableWidth, width))
        return;

    m_availableWidth = width;
    emit availableWidthChanged();
}

void BlockLayoutEngine::initializeSampleData()
{
    m_blocks.clear();

    // Block 1 with 2 nodes
    QList<Node> block1Nodes;

    Node node1_1("normal", "Normal text in block 1", 16, QFont::Normal, 0.0);
    block1Nodes.append(node1_1);

    Node node1_2("bold", "Bold text in block 1", 16, QFont::Bold, 0.0);
    block1Nodes.append(node1_2);

    Block block1(block1Nodes);
    m_blocks.append(block1);

    // Block 2 with 2 nodes
    QList<Node> block2Nodes;

    Node node2_1("bold", "Bold text in block 2", 16, QFont::Bold, 0.0);
    block2Nodes.append(node2_1);

    Node node2_2("normal", "Normal text in block 2", 16, QFont::Normal, 0.0);
    block2Nodes.append(node2_2);

    Block block2(block2Nodes);
    m_blocks.append(block2);

    emit blocksChanged();
}
