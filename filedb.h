#ifndef FILEDB_H
#define FILEDB_H

#include <QtQml>

class Filedb
{
private:
    QString notesFilePath;

    QString getOrCreateNotesDir();

public:
    Filedb();
    void writeContentToFile(QString fileName, QString content);
    QStringList listAllNotes();
    QString getFileTitle(QString filePath);
    QString createNewNote();
    QString readFile(QString filename);
    void deleteFile(QString filename);
};

#endif // FILEDB_H
