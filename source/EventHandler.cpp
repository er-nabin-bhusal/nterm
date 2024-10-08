#include <QtQml>
#include <QDebug>
#include <QQuickTextDocument>
#include <QTextCursor>
#include "source/EventHandler.h"
#include "source/Filedb.h"
#include "source/PopupHandler.h"

EventHandler::EventHandler(QObject *parent) : QObject(parent) {
    Filedb filedb;

    reloadFolders();
    currentfolder = allfolders.first().toMap().value("folderName").toString();

    reloadNotes();

    // initialize the textFormat
    textformat["heading"] = false;
    textformat["bold"] = false;
    textformat["underline"] = false;
    textformat["italic"] = false;
    textformat["paragraph"] = true;
}

void EventHandler::reloadFolders() {
    QStringList allFolders = filedb.listFolders();
    if (allFolders.empty()) {
        filedb.createFolder("Notes");
        allFolders.append("Notes");
    }

    for (const QString &folder : allFolders) {
        QVariantMap variant;
        variant["folderName"] = folder;
        variant["editAble"] = false;
        allfolders.append(variant);
    }
}

void EventHandler::reloadNotes() {
    allnotes.clear();
    QStringList noteFiles = filedb.listNotes(currentfolder);
    for (const QString &fileName : noteFiles) {
        QVariantMap variant;
        variant["title"] = filedb.getFileTitle(currentfolder, fileName);
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
    textformat["paragraph"] = !isHeading & !isBold & !isItalic & !isUnderline;
    emit textFormatChanged();
}

void EventHandler::setSelection(int selectionStart, int selectionEnd) {
    this->selectionStart = selectionStart;
    this->selectionEnd = selectionEnd;
    this->updateFormat();
}

bool EventHandler::hasSelection() { return this->selectionStart != this->selectionEnd; }

bool EventHandler::isBold(QTextCursor cursor) {
    QTextCharFormat format = cursor.charFormat();
    return format.fontWeight() == QFont::Bold && format.fontPointSize() == 14;
}

bool EventHandler::isHeading(QTextCursor cursor) {
    QTextCharFormat format = cursor.charFormat();
    return format.fontWeight() == QFont::Bold && format.fontPointSize() == 24;
}

bool EventHandler::isItalic(QTextCursor cursor) {
    QTextCharFormat format = cursor.charFormat();
    return format.fontItalic();
}

bool EventHandler::isUnderline(QTextCursor cursor) {
    QTextCharFormat format = cursor.charFormat();
    return format.fontUnderline();
}

void EventHandler::setNormalText()
{
    QTextCursor cursor = textCursor();
    QTextCharFormat format;
    format.setFontWeight(QFont::Normal);
    format.setFontPointSize(14);
    format.setFontFamilies(QStringList("Inter"));
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
    format.setFontFamilies(QStringList("Inter"));
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
        format.setFontFamilies(QStringList("Inter 24pt Black"));
        cursor.mergeCharFormat(format);

        if (!hasSelection()) {
            cursor.setPosition(this->selectionEnd);
            cursor.insertText(QString(QChar(0x200B)));
        }
    }
}

void EventHandler::handleParagraphClick() {
    QTextCursor cursor = textCursor();
    QTextCharFormat format;
    format.setFontItalic(false);
    format.setFontUnderline(false);
    // format.setForeground(Qt::black);
    // format.setForeground();
    format.setAnchor(false);
    format.setAnchorHref(NULL);

    if (this->isHeading(cursor)) {
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.mergeCharFormat(format);
        this->setBlockToNormal();
    } else {
        cursor.mergeCharFormat(format);
        this->setNormalText();
    }
}

void EventHandler::handleBoldClick()
{
    QTextCursor cursor = textCursor();

    if (isBold(cursor)) {
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
    QTextCharFormat format;
    format.setFontItalic(!this->isItalic(cursor));
    cursor.mergeCharFormat(format);
    if (!hasSelection()) {
        cursor.insertText(QString(QChar(0x200B)));
    }
}

void EventHandler::handleUnderlineClick() {
    QTextCursor cursor = textCursor();
    QTextCharFormat format;
    format.setFontUnderline(!this->isUnderline(cursor));
    cursor.mergeCharFormat(format);
    if (!hasSelection()) {
        cursor.insertText(QString(QChar(0x200B)));
    }
}

QVariantList EventHandler::allNotes() { return allnotes; }
QVariantList EventHandler::allFolders() { return allfolders; }
void EventHandler::setAllNotes(const QVariantList &allNotes) {}
void EventHandler::setAllFolders(const QVariantList &allFolders) {}

void EventHandler::createNewNote() {
    QString filename = filedb.createNewNote(currentfolder);
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

void EventHandler::createNewFolder() {
    QVariantMap variant;
    QString folder = filedb.createFolder("Notes");

    variant["folderName"] = folder;
    variant["editAble"] = true;
    allfolders.append(variant);
    emit allFoldersChanged();
}

bool EventHandler::renameFolder(int index, QString folderName) {
    QVariantMap variant = allfolders[index].toMap();
    QString oldFolderName = variant.value("folderName").toString();
    if (folderName == oldFolderName) return false;

    bool res = filedb.renameFolder(oldFolderName, folderName);
    if (!res) {
        PopupHandler *instance = PopupHandler::instance();
        instance->setMessage("Please provide a unique name!!");
        emit instance->messageChanged();
    }
    variant["folderName"] = folderName;
    allfolders[index] = variant;

    if (currentfolder == oldFolderName) currentfolder = folderName;
    emit allFoldersChanged();
    return res;
}

void EventHandler::saveContentToFile()
{
    if (currentfile.isNull() || currentfolder.isNull()){return;}

    filedb.writeContentToFile(currentfolder, currentfile, textDocument->textDocument()->toHtml());

    for (int i=0;i<allnotes.length(); i++) {
        QVariantMap map = allnotes[i].toMap();
        if (currentfile == map.value("fileName").toString()) {
            map["title"] = filedb.getFileTitle(currentfolder, currentfile);
            allnotes.removeAt(i);
            allnotes.insert(0, map);
            break;
        }
    }
    emit allNotesChanged();
}

void EventHandler::setCurrentFile(QString file) {
    if (currentfile != file) {
        currentfile = file;
        emit currentFileChanged(false);
    }
}

void EventHandler::setCurrentFolder(QString folder) {
    if (currentfolder != folder) {
        currentfolder = folder;
        reloadNotes();
        setCurrentFile(NULL);
        emit currentFolderChanged();
        emit allNotesChanged();
    }
}

QString EventHandler::currentFile() { return currentfile; }
QString EventHandler::currentFolder() { return currentfolder; }

QString EventHandler::readCurrentFileContent() {
    QString content = filedb.readFile(currentfolder, currentfile);
    return content;
}

void EventHandler::deleteNote(int noteIndex) {
    QVariant note = allnotes.at(noteIndex);
    QVariantMap map = note.toMap();
    QString fileName = map.value("fileName").toString();

    filedb.deleteFile(currentfolder, fileName);
    if (currentfile == fileName) setCurrentFile(NULL);
    allnotes.removeAt(noteIndex);
    emit allNotesChanged();
}

void EventHandler::deleteFolder(int index) {
    QVariantMap map = allfolders[index].toMap();
    QString folder = map.value("folderName").toString();
    if (filedb.isEmpty(folder)) {
        filedb.deleteFolder(folder);
        allfolders.removeAt(index);
        setCurrentFolder(NULL);
        emit allFoldersChanged();
    } else {
        PopupHandler *instance = PopupHandler::instance();
        instance->setMessage("Folder should be empty!");
        emit instance->messageChanged();
    }
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
        format.setForeground(QBrush(QColor(6, 95, 212)));
        format.setAnchorHref(word);

        QTextCursor cursor = textCursor();
        QTextCharFormat previousFormat = cursor.charFormat();

        if (!previousFormat.isAnchor()) {
            cursor.setPosition(i+1);
            cursor.setPosition(selectionEnd, QTextCursor::KeepAnchor);
            cursor.mergeCharFormat(format);
            cursor.clearSelection();
            cursor.setCharFormat(previousFormat);
            cursor.insertText(QChar(0x200B));
        }
    }
}












