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
    void writeContentToFile(QString folder, QString file, QString content);
    QStringList listNotes(QString folder);
    QStringList listFolders();
    QString getFileTitle(QString folder, QString file);
    void createFolder(QString folder);
    QString createNewNote(QString folder);
    QString readFile(QString folder, QString file);
    void deleteFile(QString folder, QString file);
};

#endif // FILEDB_H
