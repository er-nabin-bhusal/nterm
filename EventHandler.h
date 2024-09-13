#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QObject>
#include <QtQml>
#include <QQuickTextDocument>
#include "filedb.h"

class EventHandler : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString currentFile READ currentFile WRITE setCurrentFile NOTIFY currentFileChanged FINAL)
    Q_PROPERTY(QVariantList allNotes READ allNotes WRITE setAllNotes NOTIFY allNotesChanged FINAL)
    Q_PROPERTY(QVariantMap textFormat READ textFormat WRITE setTextFormat NOTIFY textFormatChanged FINAL)

private:

    Filedb filedb;
    QString currentfile;
    QVariantList allnotes;

    QQuickTextDocument *textDocument;

    int selectionStart;
    int selectionEnd;
    QVariantMap textformat;

    QTextCursor textCursor();
    void updateFormat();

public:
    explicit EventHandler(QObject *parent = nullptr);
    Q_INVOKABLE void setTextDocument(QQuickTextDocument *textDocument);
    Q_INVOKABLE void setSelection(int startPosition, int endPosition);
    bool hasSelection();
    void setNormalText();
    void setBlockToNormal();

    bool isBold(QTextCursor cursor);
    bool isHeading(QTextCursor cursor);
    bool isItalic(QTextCursor cursor);
    bool isUnderline(QTextCursor cursor);

    Q_INVOKABLE void handleBoldClick();
    Q_INVOKABLE void handleHeadingClick();
    Q_INVOKABLE void handleItalicClick();
    Q_INVOKABLE void handleUnderlineClick();
    Q_INVOKABLE void handleParagraphClick();
    Q_INVOKABLE QVariantList allNotes();
    Q_INVOKABLE void setAllNotes(const QVariantList &allNotes);
    Q_INVOKABLE void createNewNote();
    Q_INVOKABLE void saveContentToFile();
    Q_INVOKABLE void setCurrentFile(QString filename);
    Q_INVOKABLE QString currentFile();
    Q_INVOKABLE QString readCurrentFileContent();
    Q_INVOKABLE void deleteNote(int noteIndex);
    Q_INVOKABLE QVariantMap textFormat();
    Q_INVOKABLE void setTextFormat(QVariantMap format);
    Q_INVOKABLE bool enterPressed();
    Q_INVOKABLE void detectLink();
signals:
    void currentFileChanged(bool isNew);
    void allNotesChanged();
    void textFormatChanged();
};

#endif
