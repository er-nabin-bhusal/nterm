#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QObject>
#include <QtQml>
#include <QQuickTextDocument>
#include <memory>
#include "source/Filedb.h"

/**
 * @brief The EventHandler class manages all user interactions and state for the note-taking application.
 *
 * This class handles text formatting, file operations, search functionality, and UI state management.
 * It serves as the main controller between the UI (QML) and the data layer (Filedb).
 */
class EventHandler : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    // Properties
    Q_PROPERTY(QString currentFile READ currentFile WRITE setCurrentFile NOTIFY currentFileChanged FINAL)
    Q_PROPERTY(QString currentFolder READ currentFolder WRITE setCurrentFolder NOTIFY currentFolderChanged FINAL)
    Q_PROPERTY(QVariantList allNotes READ allNotes WRITE setAllNotes NOTIFY allNotesChanged FINAL)
    Q_PROPERTY(QVariantList allFolders READ allFolders WRITE setAllFolders NOTIFY allFoldersChanged FINAL)
    Q_PROPERTY(QVariantMap textFormat READ textFormat WRITE setTextFormat NOTIFY textFormatChanged FINAL)

private:
    // Core data members
    Filedb filedb;
    QString currentfile;
    QString currentfolder;
    QVariantList allnotes;
    QVariantList allfolders;
    QString searchText;
    QQuickTextDocument *textDocument;

    // Text selection state
    int selectionStart;
    int selectionEnd;
    QVariantMap textformat;

    // Private helper methods
    QTextCursor textCursor();
    void updateFormat();
    void reloadNotes();
    void reloadFolders();
    bool searchInString(const QString &content, const QString &searchTerm);
    void initializeTextFormat();

public:
    explicit EventHandler(QObject *parent = nullptr);
    ~EventHandler() override = default;

    // Text Document Management
    Q_INVOKABLE void setTextDocument(QQuickTextDocument *textDocument);
    Q_INVOKABLE void setSelection(int startPosition, int endPosition);
    bool hasSelection();

    // Text Formatting Methods
    void setNormalText();
    void setBlockToNormal();
    bool isBold(QTextCursor cursor);
    bool isHeading(QTextCursor cursor);
    bool isItalic(QTextCursor cursor);
    bool isUnderline(QTextCursor cursor);

    // Text Formatting Actions
    Q_INVOKABLE void handleBoldClick();
    Q_INVOKABLE void handleHeadingClick();
    Q_INVOKABLE void handleItalicClick();
    Q_INVOKABLE void handleUnderlineClick();
    Q_INVOKABLE void handleParagraphClick();

    // File Operations
    Q_INVOKABLE void createNewNote();
    Q_INVOKABLE void createNewFolder();
    Q_INVOKABLE void saveContentToFile();
    Q_INVOKABLE void deleteNote(int noteIndex);
    Q_INVOKABLE void deleteFolder(int index);
    Q_INVOKABLE bool renameFolder(int index, QString folderName);
    Q_INVOKABLE void emptyTrash();

    // Search Operations
    Q_INVOKABLE void onSearchTextChange(const QString &searchText);

    // Getters and Setters
    Q_INVOKABLE QVariantList allNotes();
    Q_INVOKABLE void setAllNotes(const QVariantList &allNotes);
    Q_INVOKABLE QVariantList allFolders();
    Q_INVOKABLE void setAllFolders(const QVariantList &allFolders);
    Q_INVOKABLE QString currentFile();
    Q_INVOKABLE void setCurrentFile(QString file);
    Q_INVOKABLE QString currentFolder();
    Q_INVOKABLE void setCurrentFolder(QString folder);
    Q_INVOKABLE QString readCurrentFileContent();
    Q_INVOKABLE QVariantMap textFormat();
    Q_INVOKABLE void setTextFormat(QVariantMap format);

    // Special Operations
    Q_INVOKABLE bool handleEnterKey();
    Q_INVOKABLE void detectLink();

signals:
    void currentFileChanged(bool isNew);
    void textFormatChanged();
    void allNotesChanged();
    void allFoldersChanged();
    void currentFolderChanged();
};

#endif // EVENTHANDLER_H
