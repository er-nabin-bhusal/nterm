#include "source/editor/BlockLayoutEngine.h"
#include <QDebug>
#include <Qt>

BlockLayoutEngine::BlockLayoutEngine(QObject *parent)
    : QObject(parent), m_availableWidth(0.0), m_selectedTextRange(nullptr)
{
}

BlockLayoutEngine::~BlockLayoutEngine()
{
    delete m_selectedTextRange;
}

bool BlockLayoutEngine::isSelected()
{
    if (m_selectedTextRange == nullptr)
        return false;
    if (m_selectedTextRange->start.blockIndex != m_selectedTextRange->end.blockIndex)
        return false;
    if (m_selectedTextRange->start.itemIndex != m_selectedTextRange->end.itemIndex)
        return false;
    if (m_selectedTextRange->start.cursorPosition != m_selectedTextRange->end.cursorPosition)
        return false;
    return true;
}

void BlockLayoutEngine::selectAll()
{
    for (int i = 0; i < m_blocks.size(); i++)
    {
        for (int j = 0; j < m_blocks[i].nodes().size(); j++)
        {
            emit selectionChanged(i, j, 0, m_blocks[i].nodes()[j].text().length());
        }
    }
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

void BlockLayoutEngine::toggleSelecting(bool isSelecting)
{
    m_isSelecting = isSelecting;
}

void BlockLayoutEngine::setSelectionStart(int blockIndex, int itemIndex, qreal x)
{
    Node &node = m_blocks[blockIndex].nodes()[itemIndex];
    int cursorPosition = node.getCursorPositionFromCoordinates(x);
    if (m_selectedTextRange)
    {
        m_selectedTextRange->start = {
            .blockIndex = blockIndex,
            .itemIndex = itemIndex,
            .cursorPosition = cursorPosition};
        m_selectedTextRange->end = {.blockIndex = blockIndex, .itemIndex = itemIndex, .cursorPosition = cursorPosition};
    }
    else
    {
        m_selectedTextRange = new SelectedTextRange{
            .start = {
                .blockIndex = blockIndex,
                .itemIndex = itemIndex,
                .cursorPosition = cursorPosition},
            .end = {.blockIndex = blockIndex, .itemIndex = itemIndex, .cursorPosition = cursorPosition}};
    }
}

void BlockLayoutEngine::setSelectionEnd(int blockIndex, int itemIndex, qreal x)
{
    Node &node = m_blocks[blockIndex].nodes()[itemIndex];
    int cursorPosition = node.getCursorPositionFromCoordinates(x);

    m_selectedTextRange->end = {
        .blockIndex = blockIndex,
        .itemIndex = itemIndex,
        .cursorPosition = cursorPosition};
    emit cursorPositionChanged(blockIndex, itemIndex, cursorPosition);
}

bool BlockLayoutEngine::compareCursorPositions(const CursorPosition &a, const CursorPosition &b)
{
    if (a.blockIndex != b.blockIndex)
        return a.blockIndex < b.blockIndex;
    if (a.itemIndex != b.itemIndex)
        return a.itemIndex < b.itemIndex;
    return a.cursorPosition < b.cursorPosition;
}

QPair<CursorPosition, CursorPosition> BlockLayoutEngine::normalizeSelectionRange() const
{
    CursorPosition start = m_selectedTextRange->start;
    CursorPosition end = m_selectedTextRange->end;

    if (!compareCursorPositions(start, end))
    {
        // Swap if end comes before start
        CursorPosition temp = start;
        start = end;
        end = temp;
    }

    return QPair<CursorPosition, CursorPosition>(start, end);
}

void BlockLayoutEngine::emitSelectionForRange(const CursorPosition &start, const CursorPosition &end)
{
    for (int blockIdx = start.blockIndex; blockIdx <= end.blockIndex; blockIdx++)
    {

        const Block &block = m_blocks[blockIdx];
        const QList<Node> &nodes = block.nodes();

        int startNodeIdx = (blockIdx == start.blockIndex) ? start.itemIndex : 0;
        int endNodeIdx = (blockIdx == end.blockIndex) ? end.itemIndex : (nodes.size() - 1);

        for (int nodeIdx = startNodeIdx; nodeIdx <= endNodeIdx; nodeIdx++)
        {

            const Node &node = nodes[nodeIdx];
            if (node.type() == Node::NewLine)
                continue;

            int nodeTextLength = node.text().length();

            // Determine selection start and end within this node
            int selectionStart = 0;
            int selectionEnd = nodeTextLength;

            if (blockIdx == start.blockIndex && nodeIdx == start.itemIndex)
            {
                // First node in selection
                selectionStart = start.cursorPosition;
            }

            if (blockIdx == end.blockIndex && nodeIdx == end.itemIndex)
            {
                // Last node in selection
                selectionEnd = end.cursorPosition;
            }

            // Emit selection event for this node
            emit selectionChanged(blockIdx, nodeIdx, selectionStart, selectionEnd);
        }
    }
}

void BlockLayoutEngine::onPositionChanged(int blockIndex, int itemIndex, qreal x)
{
    Node &node = m_blocks[blockIndex].nodes()[itemIndex];
    int cursorPosition = node.getCursorPositionFromCoordinates(x);
    if (!m_isSelecting || m_selectedTextRange == nullptr)
        return;

    m_selectedTextRange->end = {
        .blockIndex = blockIndex,
        .itemIndex = itemIndex,
        .cursorPosition = cursorPosition};

    QPair<CursorPosition, CursorPosition> normalizedRange = normalizeSelectionRange();
    emitSelectionForRange(normalizedRange.first, normalizedRange.second);
}

/**
 * @brief Handles the Enter key press event to split a block at the cursor position.
 *
 * Algorithm:
 * 1. Find the block
 * 2. Find the node
 * 3. If the cursor is at the beginning of the node:
 *    3.1. Add a Node before the current node with empty "" text
 *    3.2. Create a new block with the current node as the first node and all nodes after it
 * 4. If the cursor is at the end of the node:
 *    4.1. Add a Node after the current node with empty "" text
 *    4.2. Create a new block with the current node as the first node plus all nodes after it
 * 5. If the cursor is in the middle of the node:
 *    5.1. Create two nodes from the current node by splitting the node at the cursor position
 *    5.2. In the current block, replace the current node with the first half of the current node
 *    5.3. Also add a new node with empty "" text after the first half of the current node
 *    5.4. Create a new block with the second half of the current node as the first node and all nodes after it
 * 6. Set the cursor to the beginning of the new block
 * 7. During arrangement of the nodes, if you encounter a node with type "new_line", you can exit
 *
 * @param blockIndex The index of the block containing the cursor
 * @param itemIndex The index of the node within the block
 * @param cursorPosition The cursor position within the node
 */
void BlockLayoutEngine::handleEnterKey(int blockIndex, int itemIndex, int cursorPosition)
{
    Block &block = m_blocks[blockIndex];
    Block newBlock = block.addNewLineNode(itemIndex, cursorPosition);
    m_blocks.insert(blockIndex + 1, newBlock);
    emit blocksChanged();
    emit cursorPositionChanged(blockIndex + 1, 0, 0);
}

void BlockLayoutEngine::moveCursorLeft(int blockIndex, int itemIndex, int cursorPosition)
{
    if (cursorPosition > 0)
        return;

    Block &block = m_blocks[blockIndex];
    if (itemIndex == 0)
    {
        if (blockIndex > 0)
        {
            Block &prevBlock = m_blocks[blockIndex - 1];
            int nodeIndex = prevBlock.nodes().size() - 1;
            const Node *prevNode = &prevBlock.nodes()[nodeIndex];

            while (prevNode->type() == Node::NewLine)
            {
                nodeIndex -= 1;
                prevNode = &prevBlock.nodes()[nodeIndex];
            }
            int prevNodeLength = prevNode->text().length();
            emit cursorPositionChanged(blockIndex - 1, nodeIndex, prevNodeLength);
        }
    }
    else
    {
        Node &prevNode = block.nodes()[itemIndex - 1];
        int prevNodeLength = prevNode.text().length();
        emit cursorPositionChanged(blockIndex, itemIndex - 1, prevNodeLength - 1);
    }
}

void BlockLayoutEngine::moveCursorRight(int blockIndex, int itemIndex, int cursorPosition)
{
    Block &block = m_blocks[blockIndex];
    Node &node = block.nodes()[itemIndex];
    if (cursorPosition < node.text().length())
        return;

    bool isLastNode = false;
    if (itemIndex == block.nodes().size() - 1)
        isLastNode = true;
    else if (itemIndex == block.nodes().size() - 2 && block.nodes()[block.nodes().size() - 1].type() == Node::NewLine)
        isLastNode = true;
    if (isLastNode)
    {
        if (blockIndex < m_blocks.size() - 1)
        {
            emit cursorPositionChanged(blockIndex + 1, 0, 0);
        }
    }
    else
    {
        emit cursorPositionChanged(blockIndex, itemIndex + 1, 1);
    }
}

void BlockLayoutEngine::moveCursorUp(int blockIndex, int itemIndex, int cursorPosition)
{
    qreal widthTillCursor = m_blocks[blockIndex].getTotalWidthTillIndex(itemIndex, cursorPosition);
    /* Without this correction, it seems that cursor is always moving to the left at each movement */
    int errorCorrection = int(widthTillCursor / m_availableWidth) * 2;

    if (widthTillCursor > m_availableWidth)
    {
        qreal widthAbove = widthTillCursor - m_availableWidth;
        QPair<int, int> cursorPositionFromWidth = m_blocks[blockIndex].getCursorPositionFromWidth(widthAbove);
        emit cursorPositionChanged(blockIndex, cursorPositionFromWidth.first, cursorPositionFromWidth.second);
        return;
    }

    if (blockIndex == 0)
        return;
    Block &prevBlock = m_blocks[blockIndex - 1];
    qreal totalWidthOfPrevBlock = prevBlock.getTotalWidthOfBlock();
    int numberOfRows = int(totalWidthOfPrevBlock / m_availableWidth);
    errorCorrection = 2 * numberOfRows;
    QPair<int, int> cursorPositionFromWidth = prevBlock.getCursorPositionFromWidth(widthTillCursor + numberOfRows * m_availableWidth - errorCorrection);
    emit cursorPositionChanged(blockIndex - 1, cursorPositionFromWidth.first, cursorPositionFromWidth.second);
}

void BlockLayoutEngine::moveCursorDown(int blockIndex, int itemIndex, int cursorPosition)
{
    qreal widthTillCursor = m_blocks[blockIndex].getTotalWidthTillIndex(itemIndex, cursorPosition);
    qreal nextAvailableWidthInBlock = widthTillCursor + m_availableWidth;
    qreal totalWidthOfBlock = m_blocks[blockIndex].getTotalWidthOfBlock();
    if (totalWidthOfBlock > nextAvailableWidthInBlock)
    {
        QPair<int, int> nextPositionInBlock = m_blocks[blockIndex].getCursorPositionFromWidth(nextAvailableWidthInBlock);
        emit cursorPositionChanged(blockIndex, nextPositionInBlock.first, nextPositionInBlock.second);
        return;
    }
    if (blockIndex == m_blocks.size() - 1)
        return;

    qreal widthTillCursorStartOfRow = widthTillCursor - int(widthTillCursor / m_availableWidth) * m_availableWidth;
    QPair<int, int> cursorPositionFromWidth = m_blocks[blockIndex + 1].getCursorPositionFromWidth(widthTillCursorStartOfRow);
    emit cursorPositionChanged(blockIndex + 1, cursorPositionFromWidth.first, cursorPositionFromWidth.second);
}

void BlockLayoutEngine::handleArrowKeys(int blockIndex, int itemIndex, int cursorPosition, int key)
{
    if (key == Qt::Key_Left)
    {
        moveCursorLeft(blockIndex, itemIndex, cursorPosition);
    }
    else if (key == Qt::Key_Right)
    {
        moveCursorRight(blockIndex, itemIndex, cursorPosition);
    }
    else if (key == Qt::Key_Up)
    {
        moveCursorUp(blockIndex, itemIndex, cursorPosition);
    }
    else if (key == Qt::Key_Down)
    {
        moveCursorDown(blockIndex, itemIndex, cursorPosition);
    }
}

void BlockLayoutEngine::handleBackspaceKey(int blockIndex, int itemIndex, int cursorPosition)
{
    if (blockIndex <= 0 || itemIndex > 0 || cursorPosition > 0)
    {
        return;
    }
    Block &currentBlock = m_blocks[blockIndex];
    Block &prevBlock = m_blocks[blockIndex - 1];
    Node *lastItemInPrevBlock = &prevBlock.nodes()[prevBlock.nodes().size() - 1];
    if (lastItemInPrevBlock->type() == Node::NewLine)
    {
        prevBlock.nodes().removeAt(prevBlock.nodes().size() - 1);
        lastItemInPrevBlock = &prevBlock.nodes()[prevBlock.nodes().size() - 1];
    }

    int lastNodeIndexInPrevBlock = prevBlock.nodes().size() - 1;
    int cursorPositionInLastNode = lastItemInPrevBlock->text().length();

    Node &firstNode = currentBlock.nodes()[0];
    if (Node::canBeInSameBlock(*lastItemInPrevBlock, firstNode))
    {
        for (int i = 0; i < currentBlock.nodes().size(); i++)
        {
            prevBlock.nodes().append(currentBlock.nodes()[i]);
        }
        prevBlock.arrangeNodes(m_availableWidth);
        m_blocks.removeAt(blockIndex);
    }

    emit blocksChanged();
    emit cursorPositionChanged(blockIndex - 1, lastNodeIndexInPrevBlock, cursorPositionInLastNode);
}

QVariant BlockLayoutEngine::getBlock(int blockIndex) const
{
    if (blockIndex < 0 || blockIndex >= m_blocks.size())
        return QVariant();
    return QVariant::fromValue(m_blocks[blockIndex]);
}

void BlockLayoutEngine::setCursorToLastPositionInBlock(int blockIndex)
{
    if (blockIndex < 0 || blockIndex >= m_blocks.size())
        return;

    const Block &block = m_blocks[blockIndex];
    QPair<int, int> lastPos = block.getLastPositionInBlock();
    if (lastPos.first >= 0)
    {
        emit cursorPositionChanged(blockIndex, lastPos.first, lastPos.second);
    }
}

void BlockLayoutEngine::setCursorToLastPositionInLastBlock()
{
    if (m_blocks.isEmpty())
        return;

    int lastBlockIndex = m_blocks.size() - 1;
    setCursorToLastPositionInBlock(lastBlockIndex);
}

void BlockLayoutEngine::setAvailableWidth(qreal width)
{
    if (qFuzzyCompare(m_availableWidth, width))
        return;

    m_availableWidth = width;
    for (Block &block : m_blocks)
    {
        block.arrangeNodes(m_availableWidth);
    }
    emit blocksChanged();
}

void BlockLayoutEngine::initializeSampleData()
{
    m_blocks.clear();

    // Block 1 with 2 nodes
    QList<Node> block1Nodes;

    Node node1_1(Node::Normal, "Normal text in block 1", 16, QFont::Normal, 0.0);
    block1Nodes.append(node1_1);

    Node node1_2(Node::Bold, "Bold text in block 1", 16, QFont::Bold, 0.0);
    block1Nodes.append(node1_2);

    Block block1(block1Nodes);
    m_blocks.append(block1);

    // Block 2 with 2 nodes
    QList<Node> block2Nodes;

    Node node2_1(Node::Bold, "Bold text in block 2", 16, QFont::Bold, 0.0);
    block2Nodes.append(node2_1);

    Node node2_2(Node::Normal, "Normal text in block 2", 16, QFont::Normal, 0.0);
    block2Nodes.append(node2_2);

    Block block2(block2Nodes);
    m_blocks.append(block2);

    emit blocksChanged();
}
