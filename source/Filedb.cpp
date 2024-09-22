#include <QtQml>
#include <QCoreApplication>
#include <QTextDocument>
#include "source/Filedb.h"

Filedb::Filedb() {
    basePath = getOrCreateNotesDir();
}

QString Filedb::getOrCreateNotesDir() {
    QString homeDirPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString notesDirPath = QDir(homeDirPath).filePath("nterm");

    QDir notesDir(notesDirPath);
    if (!notesDir.exists()) {
        notesDir.mkpath(notesDirPath);
    }

    return notesDirPath;
}

void Filedb::writeContentToFile(QString folder, QString file, QString content) {
    QString folderPath = QString("%1/%2").arg(basePath, folder);

    QFile noteFile(QDir(folderPath).filePath(file));

    if (noteFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&noteFile);
        content.replace("\u200B", "");
        out << content;
        noteFile.close();
    }
}

QStringList Filedb::listNotes(QString folder) {
    QString notesDirPath = QString("%1/%2").arg(getOrCreateNotesDir(), folder);
    QDir notesDir(notesDirPath);
    QStringList noteFiles = notesDir.entryList(QStringList() << "*.html", QDir::Files, QDir::Time);
    return noteFiles;
}

QStringList Filedb::listFolders() {
    const int MAX_FOLDER_NAME_LEN = 10;
    QString ntermFolder = getOrCreateNotesDir();
    QDir ntermDir(ntermFolder);
    QStringList folders = ntermDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Time);
    return folders;
}

QString Filedb::readFile(QString folder, QString file) {
    QString folderPath = QString("%1/%2").arg(basePath, folder);
    QString notesPath = QDir(folderPath).filePath(file);
    QString content;
    QFile fileObj(notesPath);
    if (fileObj.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&fileObj);
        content = in.readAll();
        fileObj.close();
    }
    return content;
}

QString Filedb::getFileTitle(QString folder, QString file) {
    // Returns the title of minimum length of 50 characters
    const int MAX_TITLE_LEN = 50;

    // QString
    QString content = readFile(folder, file);
    QTextDocument document;
    document.setHtml(content);
    content = document.toPlainText();

    QStringList lines = content.split("\n");

    for(const QString &line: lines) {
        QString stripped = line.trimmed();
        if (!stripped.isEmpty()) {
            content = stripped;
            break;
        }
    }

    if (content.length() > MAX_TITLE_LEN) {
        return content.left(MAX_TITLE_LEN).append("...");
    }

    return content;
}

QString Filedb::createNewNote(QString folder) {
    QDateTime currDate = QDateTime::currentDateTime();
    qint64 timestamp = currDate.toMSecsSinceEpoch();
    QString filename = QString::number(timestamp);
    filename.append(".html");

    writeContentToFile(folder, filename, "");
    return filename;
}

QString Filedb::createFolder(QString folder) {
    QString folderName = folder;
    QString folderPath = QDir(basePath).filePath(folderName);
    QDir notesDir(folderPath);

    int postFix = 1;
    while (notesDir.exists()) {
        folderName = folder + QString::number(postFix);
        folderPath = QDir(basePath).filePath(folderName);
        notesDir.setPath(folderPath);
        postFix++;
    }

    notesDir.mkpath(folderPath);
    return folderName;
}

bool Filedb::renameFolder(QString oldName, QString newName) {
    QString oldFolderPath = QDir(basePath).filePath(oldName);
    QString newFolderPath = QDir(basePath).filePath(newName);

    QDir oldDir(oldFolderPath);
    QDir newDir(newFolderPath);

    if (newDir.exists()) {
        return false;
    }

    oldDir.rename(oldFolderPath, newFolderPath);
    return true;
}


void Filedb::deleteFile(QString folder, QString file) {
    QString folderPath = QString("%1/%2").arg(basePath, folder);
    QFile fileObj(QDir(folderPath).filePath(file));

    if (fileObj.exists()) {
        fileObj.remove();
    }
}

bool Filedb::isEmpty(QString folder) {
    QStringList notes = listNotes(folder);
    return notes.empty();
}

void Filedb::deleteFolder(QString folder) {
    QString folderPath = QString("%1/%2").arg(basePath, folder);
    QDir dir(folderPath);
    dir.removeRecursively();
}
