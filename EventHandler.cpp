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

    // initialize the textFormat
    textformat["heading"] = false;
    textformat["bold"] = false;
    textformat["underline"] = false;
    textformat["italic"] = false;

    // list all the available notes here
    for (const QString &fileName : noteFiles)
    {
        QVariantMap variant;
        variant["title"] = filedb.getFileTitle(fileName);
        variant["fileName"] = fileName;
        allnotes.append(variant);
    }
}

void EventHandler::setTextDocument(QQuickTextDocument *textDocument) {
    this->textDocument = textDocument;
}

QTextCursor EventHandler::textCursor()
{
    QTextCursor cursor = QTextCursor(this->textDocument->textDocument());

    if (hasSelection()) {
        cursor.setPosition(selectionStart);
        cursor.setPosition(selectionEnd, QTextCursor::KeepAnchor);
    } else {
        cursor.setPosition(selectionEnd);
    }
    return cursor;
}

void EventHandler::updateFormat() {
    QTextCursor cursor = textCursor();
    bool isHeading = (cursor.charFormat().fontWeight() == QFont::Bold && cursor.charFormat().fontPointSize() == 24);
    bool isBold = cursor.charFormat().fontWeight() == QFont::Bold;
    bool isItalic = cursor.charFormat().fontItalic();
    bool isUnderline = cursor.charFormat().fontUnderline();

    textformat["heading"] = isHeading ? true : false;
    textformat["bold"] = isBold ? true : false;
    textformat["italic"] = isItalic ? true : false;
    textformat["underline"] = isUnderline ? true : false;
    emit textFormatChanged();
}

void EventHandler::setSelection(int selectionStart, int selectionEnd) {
    this->selectionStart = selectionStart;
    this->selectionEnd = selectionEnd;
    this->updateFormat();
}

bool EventHandler::hasSelection() { return this->selectionStart != this->selectionEnd; }

void EventHandler::setNormalText()
{
    QTextCursor cursor = textCursor();
    QTextCharFormat format;
    format.setFontWeight(QFont::Normal);
    format.setFontPointSize(14);
    cursor.mergeCharFormat(format);

    if (!hasSelection()) {
        cursor.insertText(QString(QString(QChar(0x200B))));
    }
}

void EventHandler::setBlockToNormal() {
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::BlockUnderCursor);
    QTextCharFormat format;
    format.setFontWeight(QFont::Normal);
    format.setFontPointSize(14);
    cursor.mergeCharFormat(format);

    if (!hasSelection()) {
        cursor.setPosition(this->selectionEnd);
        cursor.insertText(QString(QChar(0x200B)));
    }

}

void EventHandler::handleHeadingClick() {
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::BlockUnderCursor);
    bool isHeading = (cursor.charFormat().fontPointSize() == 24);

    if (isHeading) {
        setBlockToNormal();
    } else {
        QTextCharFormat format;
        format.setFontWeight(QFont::Bold);
        format.setFontPointSize(24);
        cursor.mergeCharFormat(format);

        if (!hasSelection()) {
            cursor.setPosition(this->selectionEnd);
            cursor.insertText(QString(QChar(0x200B)));
        }
    }
}

void EventHandler::handleBoldClick()
{
    QTextCursor cursor = textCursor();
    bool isBold = cursor.charFormat().fontWeight() == QFont::Bold;

    if (isBold) {
        setNormalText();
    } else {
        QTextCharFormat format;
        format.setFontWeight(QFont::Bold);
        cursor.mergeCharFormat(format);
        if (!hasSelection()) {
            cursor.insertText(QString(QChar(0x200B)));
        }
    }
}

void EventHandler::handleItalicClick()
{
    QTextCursor cursor = textCursor();
    bool isItalic = cursor.charFormat().fontItalic();
    QTextCharFormat format;
    format.setFontItalic(!isItalic);
    cursor.mergeCharFormat(format);
    if (!hasSelection()) {
        cursor.insertText(QString(QChar(0x200B)));
    }
}

void EventHandler::handleUnderlineClick()
{
    QTextCursor cursor = textCursor();
    bool isUnderline = cursor.charFormat().fontUnderline();
    QTextCharFormat format;
    format.setFontUnderline(!isUnderline);
    cursor.mergeCharFormat(format);
    if (!hasSelection()) {
        cursor.insertText(QString(QChar(0x200B)));
    }
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

QVariantMap EventHandler::textFormat() {return textformat; }
void EventHandler::setTextFormat(QVariantMap format) {}

/**
 * This function is to change the block is the current format is heading.
**/
bool EventHandler::enterPressed() {
    QTextCursor cursor = this->textCursor();
    bool isHeading = (cursor.charFormat().fontPointSize() == 24);
    if (!isHeading || hasSelection()) {
        detectLink();
        return false;
    }

    cursor.insertBlock();
    this->setBlockToNormal();
    return true;
};


void EventHandler::detectLink() {
    QString text = textDocument->textDocument()->toPlainText();
    int i;
    for(i=this->selectionEnd-1;i>0;i--){
        if (text[i] == " " || text[i] == "\n") {
            break;
        }
    }
    QString word = text.mid(i, selectionEnd-i).trimmed();
    QUrl url(word);

    if (url.isValid() && !url.scheme().isEmpty()) {
        QTextCharFormat format;
        format.setAnchor(true);
        format.setForeground(Qt::blue);
        format.setAnchorHref(word);

        QTextCursor cursor = textCursor();
        QTextCharFormat previousFormat = cursor.charFormat();
        cursor.setPosition(i+1);
        cursor.setPosition(selectionEnd, QTextCursor::KeepAnchor);
        cursor.mergeCharFormat(format);
        cursor.clearSelection();
        cursor.setCharFormat(previousFormat);
        cursor.insertText(QChar(0x200B));
    }
}












