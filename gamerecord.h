#ifndef GAMERECORD_H
#define GAMERECORD_H

#include "QObject"
#include "QPixmap"
#include <QNetworkReply>

class GameRecord : public QObject
{
    Q_OBJECT
public:
    GameRecord();
    GameRecord(int id, QString title, QString img, bool hidded);
    GameRecord(int id, QString title, QPixmap img, bool hidded);
    GameRecord(GameRecord&);
    int getID();
    QString getTitle();
    QString getImg();
    QPixmap getPixmap();
    bool getIsHide();
    void setIsHide(bool hide);
    int record = 0;

private:
    bool isLoaded;

    int id;
    QString title;
    QString img;
    QPixmap pixmap;
    bool hidded;

private slots:
    void imgLoaded(QNetworkReply*);

};

#endif // GAMERECORD_H
