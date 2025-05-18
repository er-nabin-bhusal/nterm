#include "Editor.h"
#include <QDebug>

Editor::Editor(QQuickItem *parent) : QQuickItem(parent)
{
    // Initialize with a default line
    QVariantMap initialLine;
    initialLine["text"] = "|";
    initialLine["index"] = 0;

    m_lines.append(initialLine);
}

void Editor::updateLines(const QString &text)
{
    if (text == "\b" || text == "\177")
    {
        QString currentText = m_lines.last()["text"].toString();
        currentText.chop(1);
        m_lines.last()["text"] = currentText;
    }
    else
    {
        m_lines.last()["text"] = m_lines.last()["text"].toString() + text;
    }

    emit linesChanged();
}

void Editor::setLines(const QList<QVariantMap> &lines)
{
    // TODO: Implement
}

/* ------- Events ------- */
void Editor::onClicked()
{
    qDebug() << "onClicked";
    this->isEditing = true;
}

void Editor::onKeyPressed(const QString &text)
{
    if (this->isEditing)
    {
        updateLines(text);
    }
}

/* ------- Events ------- */