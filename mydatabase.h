#ifndef MYDATABASE_H
#define MYDATABASE_H
#include "QObject"
#include "QSqlDatabase"
#include "gamerecord.h"

struct TopRecord
{
    QString time;
    QString gameList;
};

class myDatabase : public QObject
{
    Q_OBJECT
public:

    static myDatabase& getInstance();
    void addRecord(GameRecord* gameRecord);
    void addTopList(QVector <GameRecord*> gameRecord);
    QVector<GameRecord*> getAllRecords();
    QVector<TopRecord> getAllTopLists();

    QVector<GameRecord*> getVisibleRecords();
    void removeRecord(int id);
    void removeTopList(TopRecord topRecord);
    void changeFlagHide(int id, bool hidded);


private:
    myDatabase(const myDatabase&) = delete;
    myDatabase& operator=(const myDatabase&) = delete;
    myDatabase(QObject *parent = nullptr);
    void initDb();
    void connectDb(QString path);
    QSqlDatabase db;
};

#endif // MYDATABASE_H
