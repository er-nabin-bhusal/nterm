#include "Filedb.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QTextDocument>

Filedb::Filedb()
{
    basePath = getOrCreateNtermDir();
    QStringList allFolders = listFolders(true);

    if (!allFolders.contains("Trash"))
        createFolder("Trash");
}

QString Filedb::getOrCreateNtermDir()
{
    QString homeDirPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString notesDirPath = QDir(homeDirPath).filePath("nterm");

    QDir notesDir(notesDirPath);
    if (!notesDir.exists())
    {
        notesDir.mkpath(notesDirPath);
    }

    return notesDirPath;
}

// File operations
void Filedb::writeContentToFile(const QString &folder, const QString &file, const QString &content)
{
    QString folderPath = QString("%1/%2").arg(basePath, folder);
    QFile noteFile(QDir(folderPath).filePath(file));

    if (noteFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&noteFile);
        QString modifiedContent = content;
        modifiedContent.replace("\u200B", "");
        out << modifiedContent;
        noteFile.close();
    }
}

QString Filedb::readFile(const QString &folder, const QString &file)
{
    QString folderPath = QString("%1/%2").arg(basePath, folder);
    QString notesPath = QDir(folderPath).filePath(file);
    QString content;
    QFile fileObj(notesPath);
    if (fileObj.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&fileObj);
        content = in.readAll();
        fileObj.close();
    }
    return content;
}

QString Filedb::getFileTitle(const QString &folder, const QString &file)
{
    const int MAX_TITLE_LEN = 50;
    QString content = readFile(folder, file);
    QTextDocument document;
    document.setHtml(content);
    content = document.toPlainText();

    QStringList lines = content.split("\n");
    for (const QString &line : lines)
    {
        QString stripped = line.trimmed();
        if (!stripped.isEmpty())
        {
            content = stripped;
            break;
        }
    }

    if (content.length() > MAX_TITLE_LEN)
    {
        return content.left(MAX_TITLE_LEN).append("...");
    }

    return content;
}

// Folder operations
QStringList Filedb::listFolders(bool includeTrash)
{
    QString ntermFolder = getOrCreateNtermDir();
    QDir ntermDir(ntermFolder);
    QStringList folders = ntermDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time);
    if (!includeTrash)
    {
        folders.removeAll("Trash");
    }
    return folders;
}

QStringList Filedb::listNotes(const QString &folder)
{
    QString notesDirPath = QString("%1/%2").arg(getOrCreateNtermDir(), folder);
    QDir notesDir(notesDirPath);
    return notesDir.entryList(QStringList() << "*.html", QDir::Files, QDir::Time);
}

QString Filedb::createFolder(const QString &folder)
{
    QString folderName = folder;
    QString folderPath = QDir(basePath).filePath(folderName);
    QDir notesDir(folderPath);

    int postFix = 1;
    while (notesDir.exists())
    {
        folderName = folder + QString::number(postFix);
        folderPath = QDir(basePath).filePath(folderName);
        notesDir.setPath(folderPath);
        postFix++;
    }

    notesDir.mkpath(folderPath);
    return folderName;
}

bool Filedb::renameFolder(const QString &oldName, const QString &newName)
{
    QString oldFolderPath = QDir(basePath).filePath(oldName);
    QString newFolderPath = QDir(basePath).filePath(newName);

    QDir oldDir(oldFolderPath);
    QDir newDir(newFolderPath);

    if (newDir.exists())
    {
        return false;
    }

    return oldDir.rename(oldFolderPath, newFolderPath);
}

void Filedb::deleteFolder(const QString &folder)
{
    QString folderPath = QString("%1/%2").arg(basePath, folder);
    QDir dir(folderPath);
    dir.removeRecursively();
}

void Filedb::deleteAllFilesFromFolder(const QString &folder)
{
    QString folderPath = QDir(basePath).filePath(folder);
    QDir folderDir(folderPath);

    if (folderDir.exists())
    {
        QStringList files = folderDir.entryList(QDir::Files);
        for (const QString &file : files)
        {
            QString filePath = folderDir.filePath(file);
            QFile fileObj(filePath);
            fileObj.remove();
        }
    }
}

bool Filedb::isEmpty(const QString &folder)
{
    return listNotes(folder).empty();
}

// Note operations
QString Filedb::createNewNote(const QString &folder)
{
    QDateTime currDate = QDateTime::currentDateTime();
    qint64 timestamp = currDate.toMSecsSinceEpoch();
    QString filename = QString::number(timestamp) + ".html";

    writeContentToFile(folder, filename, "");
    return filename;
}

void Filedb::moveNoteToTrash(const QString &sourceFolder, const QString &file)
{
    /**
     * Moves a note to the Trash folder, replacing any existing file with the same name
     */
    QString sourcePath = QDir(QDir(basePath).filePath(sourceFolder)).filePath(file);
    QString destPath = QDir(QDir(basePath).filePath("Trash")).filePath(file);

    if (QFile::exists(destPath))
    {
        QFile destFile(destPath);
        destFile.remove();
    }

    QFile::rename(sourcePath, destPath);
}

/**
 * Deletes a note file or moves it to trash
 * If the note is already in Trash, permanently deletes it
 * Otherwise, moves the note to the Trash folder
 */
void Filedb::deleteNoteFile(const QString &folder, const QString &file)
{
    if (folder == "Trash")
    {
        QString filePath = QDir(QDir(basePath).filePath(folder)).filePath(file);
        QFile fileObj(filePath);
        if (fileObj.exists())
        {
            fileObj.remove();
        }
    }
    else
    {
        moveNoteToTrash(folder, file);
    }
}
