#include "Editor.h"
#include <QDebug>

Editor::Editor(QQuickItem *parent) : QQuickItem(parent)
{
    // Initialize with a default line
    QVariantMap initialLine;
    initialLine["text"] = "";
    initialLine["index"] = 0;

    m_lines.append(initialLine);
}

void Editor::updateLines(const QString &text, const int &index)
{
    // Handle backspace/delete
    if (text == "\b" || text == "\177")
    {
        QString currentText = m_lines[index]["text"].toString();
        if (!currentText.isEmpty())
        {
            currentText.chop(1);
            m_lines[index]["text"] = currentText;
        }
        emit linesChanged();
        return;
    }

    // Handle newline
    if (text == "\r" || text == "\n")
    {
        QVariantMap newLine;
        newLine["text"] = "";
        newLine["index"] = m_lines.size();
        m_lines.append(newLine);
        emit linesChanged();
        return;
    }

    // Handle regular text input
    QString currentText = m_lines[index]["text"].toString();
    m_lines[index]["text"] = currentText + text;
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

void Editor::onKeyPressed(const QString &text, const int &index)
{
    if (this->isEditing)
    {
        updateLines(text, index);
    }
}

/* ------- Events ------- */