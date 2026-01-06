#ifndef BLOCK_H
#define BLOCK_H

#include <QtQml>
#include <QString>
#include <QList>
#include <QVariant>
#include "source/editor/Node.h"

class Block
{
    Q_GADGET
    Q_PROPERTY(QVariantList items READ items WRITE setItems)

public:
    Block();
    Block(const QList<Node> &nodes);

    /* Get the total number of characters till the index */
    int getTotalCharactersCountTillIndex(int index) const;
    qreal getTotalWidthTillIndex(int nodeIndex, int cursorPosition) const;
    QPair<int, int> getCursorPositionFromWidth(qreal width) const;

    QPair<int, int> getCursorPositionInBlock(int cursorPositionInBlock) const;

    /* Get the last position in the block (last item index, last character position in that item) */
    QPair<int, int> getLastPositionInBlock() const;
    qreal getTotalWidthOfBlock() const;

    void arrangeNodes(qreal availableMaxWidth);
    Block addNewLineNode(int nodeIndex, int cursorPosition);

    QVariantList items() const;
    void setItems(const QVariantList &items);

    // Direct access to nodes (for C++ code)
    QList<Node> &nodes();
    const QList<Node> &nodes() const;

private:
    QList<Node> m_nodes;

    int findSplitCharIndex(Node node, qreal widthForFirstNode) const;
};
Q_DECLARE_METATYPE(Block)

#endif // BLOCK_H
