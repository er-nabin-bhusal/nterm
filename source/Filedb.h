#ifndef FILEDB_H
#define FILEDB_H

#include <QString>
#include <QStringList>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QTextDocument>
#include <QStandardPaths>

class Filedb
{
private:
    QString basePath;
    QString getOrCreateNotesDir();

public:
    Filedb();

    // File operations
    void writeContentToFile(const QString &folder, const QString &file, const QString &content);
    QString readFile(const QString &folder, const QString &file);
    QString getFileTitle(const QString &folder, const QString &file);

    // Folder operations
    QStringList listFolders();
    QStringList listNotes(const QString &folder);
    QString createFolder(const QString &folder);
    bool renameFolder(const QString &oldName, const QString &newName);
    void deleteFolder(const QString &folder);
    bool isEmpty(const QString &folder);

    // Note operations
    QString createNewNote(const QString &folder);
    void deleteFile(const QString &folder, const QString &file);
};

#endif // FILEDB_H
