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

struct CursorPosition
{
    int blockIndex;
    int itemIndex;
    int cursorPosition;
};

struct SelectedTextRange
{
    CursorPosition start;
    CursorPosition end;
};

class BlockLayoutEngine : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(qreal availableWidth READ availableWidth WRITE setAvailableWidth NOTIFY availableWidthChanged FINAL)
    Q_PROPERTY(QVariantList blocks READ blocks NOTIFY blocksChanged FINAL)

private:
    qreal m_availableWidth;
    QList<Block> m_blocks;
    bool m_isSelecting;
    SelectedTextRange *m_selectedTextRange;

    // Helper functions for selection handling
    static bool compareCursorPositions(const CursorPosition &a, const CursorPosition &b);
    QPair<CursorPosition, CursorPosition> normalizeSelectionRange() const;
    void emitSelectionForRange(const CursorPosition &start, const CursorPosition &end);
    bool isSelected();

public:
    explicit BlockLayoutEngine(QObject *parent = nullptr);
    ~BlockLayoutEngine();

    Q_INVOKABLE void selectAll();
    Q_INVOKABLE void textChanged(const QString &text, int blockIndex, int itemIndex, int cursorPosition);
    Q_INVOKABLE QVariant getBlock(int blockIndex) const;
    Q_INVOKABLE void setCursorToLastPositionInBlock(int blockIndex);
    Q_INVOKABLE void setCursorToLastPositionInLastBlock();
    Q_INVOKABLE void handleEnterKey(int blockIndex, int itemIndex, int cursorPosition);
    Q_INVOKABLE void handleBackspaceKey(int blockIndex, int itemIndex, int cursorPosition);
    Q_INVOKABLE void handleArrowKeys(int blockIndex, int itemIndex, int cursorPosition, int key);
    Q_INVOKABLE void toggleSelecting(bool isSelecting);
    Q_INVOKABLE void setSelectionStart(int blockIndex, int itemIndex, qreal x);
    Q_INVOKABLE void setSelectionEnd(int blockIndex, int itemIndex, qreal x);
    Q_INVOKABLE void onPositionChanged(int blockIndex, int itemIndex, qreal x); // Handle mouse position changed
    void moveCursorLeft(int blockIndex, int itemIndex, int cursorPosition);
    void moveCursorRight(int blockIndex, int itemIndex, int cursorPosition);
    void moveCursorUp(int blockIndex, int itemIndex, int cursorPosition);
    void moveCursorDown(int blockIndex, int itemIndex, int cursorPosition);

    qreal availableWidth() const
    {
        return m_availableWidth;
    }
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
    void selectionChanged(int blockIndex, int itemIndex, int start, int end);
    void availableWidthChanged();
    void blocksChanged();
    void blockChanged(int blockIndex);
    void cursorPositionChanged(int blockIndex, int itemIndex, int cursorPosition);
};

#endif // BLOCKLAYOUTENGINE_H
