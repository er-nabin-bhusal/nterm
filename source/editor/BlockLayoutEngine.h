#ifndef BLOCKLAYOUTENGINE_H
#define BLOCKLAYOUTENGINE_H

#include <QObject>
#include <QtQml>
#include <QFont>
#include <QFontMetrics>
#include <QList>
#include <QVariant>
#include "source/editor/Node.h"
#include "source/editor/Block.h"

class BlockLayoutEngine : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(qreal availableWidth READ availableWidth WRITE setAvailableWidth NOTIFY availableWidthChanged FINAL)
    Q_PROPERTY(QVariantList blocks READ blocks NOTIFY blocksChanged FINAL)

private:
    qreal m_availableWidth;
    QList<Block> m_blocks;

public:
    explicit BlockLayoutEngine(QObject *parent = nullptr);

    Q_INVOKABLE void textChanged(const QString &text, int blockIndex, int itemIndex, int cursorPosition);
    Q_INVOKABLE QVariant getBlock(int blockIndex) const;
    Q_INVOKABLE void setCursorToLastPositionInBlock(int blockIndex);
    Q_INVOKABLE void setCursorToLastPositionInLastBlock();

    // Getters and Setters
    qreal availableWidth() const { return m_availableWidth; }
    void setAvailableWidth(qreal width);

    QVariantList blocks() const
    {
        QVariantList result;
        for (const Block &block : m_blocks)
        {
            result.append(QVariant::fromValue(block));
        }
        return result;
    }
    Q_INVOKABLE void initializeSampleData();

signals:
    void availableWidthChanged();
    void blocksChanged();
    void blockChanged(int blockIndex);
    void cursorPositionChanged(int blockIndex, int itemIndex, int cursorPosition);
};

#endif // BLOCKLAYOUTENGINE_H
