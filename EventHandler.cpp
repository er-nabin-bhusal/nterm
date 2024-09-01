#include "EventHandler.h"
#include <QtQml>
#include <QDebug>
#include <QQuickTextDocument>
#include <QTextCursor>
#include "filedb.h"

EventHandler::EventHandler(QObject *parent) : QObject(parent)
{
    Filedb filedb;
    QStringList noteFiles = filedb.listAllNotes();

    for (const QString &fileName : noteFiles)
    {
        QVariantMap variant;
        variant["title"] = filedb.getFileTitle(fileName);
        variant["fileName"] = fileName;
        allnotes.append(variant);
    }
}

void EventHandler::setTextDocument(QQuickTextDocument *textDocument)
{
    this->textDocument = textDocument;
}

QTextCursor EventHandler::textCursor()
{
    QTextCursor cursor = QTextCursor(this->textDocument->textDocument());

    if (hasSelection())
    {
        cursor.setPosition(selectionStart);
        cursor.setPosition(selectionEnd, QTextCursor::KeepAnchor);
    }
    else
    {
        cursor.setPosition(selectionStart, QTextCursor::MoveAnchor);
    }
    return cursor;
}

void EventHandler::setSelection(int selectionStart, int selectionEnd)
{
    this->selectionStart = selectionStart;
    this->selectionEnd = selectionEnd;
}

bool EventHandler::hasSelection()
{
    if (this->selectionStart < this->selectionEnd)
    {
        return true;
    }
    return false;
}

void EventHandler::setNormalText()
{
    QTextCursor cursor = textCursor();
    QTextCharFormat format;
    format.setFontWeight(QFont::Normal);
    format.setFontPointSize(14);
    cursor.mergeCharFormat(format);
}

void EventHandler::setBlockToNormal()
{
    QTextCursor cursor = textCursor();
    QTextCharFormat format;
    cursor.select(QTextCursor::BlockUnderCursor);
    format.setFontWeight(QFont::Normal);
    format.setFontPointSize(14);
    cursor.mergeCharFormat(format);
}

void EventHandler::handleHeadingClick()
{
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::BlockUnderCursor);
    bool isHeading = (cursor.charFormat().fontWeight() == QFont::Bold && cursor.charFormat().fontPointSize() == 24);

    if (isHeading)
    {
        setBlockToNormal();
    }
    else
    {
        QTextCharFormat format;
        format.setFontWeight(QFont::Bold);
        format.setFontPointSize(24);
        cursor.mergeCharFormat(format);
    }
}

void EventHandler::handleBoldClick()
{
    QTextCursor cursor = textCursor();
    bool isBold = cursor.charFormat().fontWeight() == QFont::Bold;
    if (isBold)
    {
        setNormalText();
    }
    else
    {
        QTextCharFormat format;
        format.setFontWeight(QFont::Bold);
        cursor.mergeCharFormat(format);
    }
}

void EventHandler::handleItalicClick()
{
    QTextCursor cursor = textCursor();
    bool isItalic = cursor.charFormat().fontItalic();
    QTextCharFormat format;
    format.setFontItalic(!isItalic);
    cursor.mergeCharFormat(format);
}

void EventHandler::handleUnderlineClick()
{
    QTextCursor cursor = textCursor();
    bool isUnderline = cursor.charFormat().fontUnderline();
    QTextCharFormat format;
    format.setFontUnderline(!isUnderline);
    cursor.mergeCharFormat(format);
}

void EventHandler::sayHello()
{
    qDebug() << "Say Hello";
}

QVariantList EventHandler::allNotes()
{
    return allnotes;
}

void EventHandler::setAllNotes(const QVariantList &allNotes) {}

void EventHandler::createNewNote()
{
    QString filename = filedb.createNewNote();
    setCurrentFile(filename);

    QVariantMap variant;
    variant["title"] = "Start with the Title...";
    variant["fileName"] = filename;
    allnotes.insert(0, variant);

    textDocument->textDocument()->setPlainText("Start with the Title...");
    handleHeadingClick();
    saveContentToFile();

    emit allNotesChanged();
    emit currentFileChanged(true);
}

void EventHandler::saveContentToFile()
{
    if (currentfile.isNull()){return;}

    filedb.writeContentToFile(currentfile, textDocument->textDocument()->toHtml());

    for (int i=0;i<allnotes.length(); i++) {
        QVariantMap map = allnotes[i].toMap();
        if (currentfile == map.value("fileName").toString()) {
            map["title"] = filedb.getFileTitle(currentfile);
            allnotes.removeAt(i);
            allnotes.insert(0, map);
            break;
        }
    }
    emit allNotesChanged();
}

void EventHandler::setCurrentFile(QString filename)
{
    if (currentfile != filename)
    {
        currentfile = filename;
        emit currentFileChanged(false);
    }
}

QString EventHandler::currentFile()
{
    return currentfile;
}

QString EventHandler::readCurrentFileContent()
{
    QString content = filedb.readFile(this->currentfile);
    return content;
}

void EventHandler::deleteNote(int noteIndex)
{
    QVariant note = allnotes.at(noteIndex);
    QVariantMap map = note.toMap();
    QString fileName = map.value("fileName").toString();

    this->filedb.deleteFile(fileName);
    setCurrentFile(NULL);
    allnotes.removeAt(noteIndex);
    emit allNotesChanged();
}
