#include <QtQml>
#include <QCoreApplication>
#include <QTextDocument>
#include "filedb.h"

Filedb::Filedb() {
    notesFilePath = getOrCreateNotesDir();
}

QString Filedb::getOrCreateNotesDir() {
    QString homeDirPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString notesDirPath = QDir(homeDirPath).filePath("nterm/notes");

    QDir notesDir(notesDirPath);
    if (!notesDir.exists()) {
        notesDir.mkpath(notesDirPath);
    }

    return notesDirPath;
}

void Filedb::writeContentToFile(QString fileName, QString content) {
    QFile file(QDir(this->notesFilePath).filePath(fileName));

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << content;
        file.close();
    }
}

QStringList Filedb::listAllNotes() {
    QString notesDirPath = getOrCreateNotesDir();
    QDir notesDir(notesDirPath);
    QStringList noteFiles = notesDir.entryList(QStringList() << "*.txt", QDir::Files, QDir::Time);
    return noteFiles;
}

QString Filedb::readFile(QString filename) {
    QString notesPath = QDir(this->notesFilePath).filePath(filename);
    QString content;
    QFile file(notesPath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        content = in.readAll();
        file.close();
    }
    return content;
}

QString Filedb::getFileTitle(QString fileName) {
    // Returns the title of minimum length of 50 characters
    const int MAX_TITLE_LEN = 50;

    QString content = readFile(fileName);
    QTextDocument document;
    document.setHtml(content);
    content = document.toPlainText().replace("\n", " ");
    if (content.length() > MAX_TITLE_LEN) {
        return content.left(MAX_TITLE_LEN).append("...");
    }
    return content;
}

QString Filedb::createNewNote() {
    QDateTime currDate = QDateTime::currentDateTime();
    qint64 timestamp = currDate.toMSecsSinceEpoch();
    QString filename = QString::number(timestamp);
    filename.append(".txt");

    writeContentToFile(filename, "");
    return filename;
}

void Filedb::deleteFile(QString filename) {
    QFile file(QDir(this->notesFilePath).filePath(filename));

    if (file.exists()) {
        file.remove();
    }
}
