#include "myDatabase.h"
#include <QCoreApplication>
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>
#include "QPixmap"

#define DATABASE_NAME "database.db"
#define DATABASE_HOSTNAME "user"

myDatabase& myDatabase::getInstance()
{
    static myDatabase instance;
    return instance;
}

myDatabase::myDatabase(QObject *parent) : QObject(parent)
{
    QString path = qApp->applicationDirPath() + "/" DATABASE_NAME;
    if (!QFile(path).exists()) {
        connectDb(path);
        initDb();
    } else {
        connectDb(path);
    }
}

void myDatabase::initDb()
{
    QSqlQuery query;
    query.prepare("CREATE TABLE \"records\" ("
                    "\"id\"	INTEGER,"
                    "\"name\"	STRING,"
                    "\"path\"	STRING,"
                    "\"hidded\" BOOL,"
                    "PRIMARY KEY(\"id\")"
                    ");");
    if (!query.exec()) {
        qDebug() << "error on create table1 " << query.lastError().text();
    }
    query.prepare(  "CREATE TABLE \"top\" ("
                    "\"timestamp\"	REAL,"
                    "\"tops\"	STRING,"
                    "PRIMARY KEY(\"timestamp\")"
                    ");");

    if (!query.exec()) {
        qDebug() << "error on create table2 " << query.lastError().text();
    }
}

void myDatabase::connectDb(QString path)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(path);
    db.open();
}

void myDatabase::addRecord(GameRecord* gameRecord)
{

    int id = gameRecord->getID();
    QString name = gameRecord->getTitle();
    QPixmap img = gameRecord->getPixmap();
    bool hidded = gameRecord->getIsHide();

    QString path = qApp->applicationDirPath() + "/IMG/" + QString::number(id) + ".jpg";
    img.save(path, "JPG");

    QSqlQuery query;
    query.prepare("INSERT INTO records VALUES ("
                  ":id,"
                  ":name,"
                  ":path,"
                  ":hidded);");
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":path", path);
    query.bindValue(":hidded", hidded);

    if (!query.exec()) {
        qDebug() << "error " << query.lastError().text();
    }
}

void myDatabase::addTopList(QVector<GameRecord*> topList)
{
    QString tops;
    int i = 1;

    for (auto game : topList)
    {
        tops +=  QString::number(i) + ". " + game->getTitle() + "\n\n";
        i++;
    }
    tops = tops.left(tops.size()-2);
    QSqlQuery query;
    query.prepare("INSERT INTO top VALUES ("
                  "(SELECT strftime('%s', datetime('now', 'localtime'))),"
                  ":tops);");
    query.bindValue(":tops", tops);

    if (!query.exec()) {
        qDebug() << "error " << query.lastError().text();
    }
}

QVector<GameRecord*> myDatabase::getAllRecords()
{
    QSqlQuery query("SELECT id, name, path, hidded from records ORDER BY name ASC;");
    query.exec();

    QVector<GameRecord*> records;

    while (query.next()) {
        GameRecord *record = new GameRecord(query.record().value("id").toInt(),
                          query.record().value("name").toString(),
                          QPixmap(query.record().value("path").toString(),"JPG"),
                          query.record().value("hidded").toBool());
        records.push_back(record);
    }

    return records;
}

QVector<TopRecord> myDatabase::getAllTopLists()
{
    QSqlQuery query("SELECT datetime(timestamp, 'unixepoch') as timestamp, tops from top ORDER BY timestamp DESC;");
    query.exec();

    QVector<TopRecord> topList;

    while (query.next()) {
        //  record.timestamp = query.record().value("timestamp").toString();
        TopRecord record;
        record.gameList = query.record().value("tops").toString();
        record.time = query.record().value("timestamp").toString();
        topList.push_back(record);
    }

    return topList;
}

QVector<GameRecord *> myDatabase::getVisibleRecords()
{
    QSqlQuery query("SELECT id, name, path, hidded from records where hidded=0;");
    query.exec();

    QVector<GameRecord*> records;

    while (query.next()) {
        GameRecord *record = new GameRecord(query.record().value("id").toInt(),
                                            query.record().value("name").toString(),
                                            QPixmap(query.record().value("path").toString(),"JPG"),
                                            query.record().value("hidded").toBool());
        records.push_back(record);
    }

    return records;
}

void myDatabase::removeRecord(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM records "
                  "WHERE id =:id;");

    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "error on delete " << query.lastError().text();
    }
}

void myDatabase::removeTopList(TopRecord topRecord)
{
    QSqlQuery query;
    query.prepare("DELETE FROM top "
                  "WHERE timestamp = strftime('%s', :stamp);");

    query.bindValue(":stamp", topRecord.time);

    if (!query.exec()) {
        qDebug() << "error on delete " << query.lastError().text();
    }
}

void myDatabase::changeFlagHide(int id, bool hidded)
{
    QSqlQuery query;
    query.prepare("update records set hidded =:hidded where id =:id;");
    query.bindValue(":id", id);
    query.bindValue(":hidded", hidded);

    if (!query.exec()) {
        qDebug() << "error on delete " << query.lastError().text();
    }
}
