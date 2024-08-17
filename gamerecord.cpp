#include "gamerecord.h"
#include <QNetworkAccessManager>
#include <QCoreApplication>

GameRecord::GameRecord()
{
}

GameRecord::GameRecord(int id, QString title, QString img, bool hidded)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &GameRecord::imgLoaded);
    if (img != "")
    {
        manager->get(QNetworkRequest(QUrl(img)));
        isLoaded = false;
    }
    else
    {
        pixmap = QPixmap(":/new/picture/img/cross.png");
        isLoaded = true;
    }

    this->id = id;
    this->title = title;
    this->img = img;
    this->hidded = hidded;
}

GameRecord::GameRecord(int id, QString title, QPixmap img, bool hidded)
{
    isLoaded = true;

    this->id = id;
    this->title = title;
    this->img = "";
    pixmap = img;
    this->hidded = hidded;
}

GameRecord::GameRecord(GameRecord &another)
{
    id = another.id;
    title=another.title;
    img=another.img;
    pixmap = another.getPixmap();
}

int GameRecord::getID()
{
    return id;
}

QString GameRecord::getTitle()
{
    return title;
}

QString GameRecord::getImg()
{
    return img;
}

QPixmap GameRecord::getPixmap()
{
    while(!isLoaded) {
        QCoreApplication::processEvents();
    }
    return pixmap;
}

bool GameRecord::getIsHide()
{
    return hidded;
}

void GameRecord::setIsHide(bool hide)
{
    hidded = hide;
}

void GameRecord::imgLoaded(QNetworkReply *reply)
{

    QByteArray data = reply->readAll();
    pixmap.loadFromData(data);
    isLoaded = true;
}



