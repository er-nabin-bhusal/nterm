#include "Filedb.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QTextDocument>

Filedb::Filedb()
{
    basePath = getOrCreateNotesDir();
}

QString Filedb::getOrCreateNotesDir()
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
QStringList Filedb::listFolders()
{
    QString ntermFolder = getOrCreateNotesDir();
    QDir ntermDir(ntermFolder);
    return ntermDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time);
}

QStringList Filedb::listNotes(const QString &folder)
{
    QString notesDirPath = QString("%1/%2").arg(getOrCreateNotesDir(), folder);
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

void Filedb::deleteFile(const QString &folder, const QString &file)
{
    QString folderPath = QString("%1/%2").arg(basePath, folder);
    QFile fileObj(QDir(folderPath).filePath(file));

    if (fileObj.exists())
    {
        fileObj.remove();
    }
}
