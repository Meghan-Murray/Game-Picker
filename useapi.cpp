#include "useapi.h"
#include "QCoreApplication"
#include "gamerecord.h"
#include "qjsondocument.h"
#include "qjsonobject.h"
#include "qjsonarray.h"

QString API_KEY = "moby_2eW7k7k1Z49YEtrYYTac6xWgTI7";

UseAPI::UseAPI(QObject *parent)
    : QObject{parent}
{

    manager = new QNetworkAccessManager(this);
}

QList <GameRecord*> UseAPI::searchGame(QString titleSearch)
{
    //connect(manager, &QNetworkAccessManager::finished, this,
    //        &UseAPI::http_finished);
    QList <GameRecord*> res;
    bool continuescan = true;
    for (int offset=0; continuescan; offset+=100) {
        QNetworkReply *reply = manager->get(QNetworkRequest(QUrl("https://api.mobygames.com/v1/games?format=normal&title="+ titleSearch +
                                                                 "&api_key="+API_KEY+"&offset="+QString::number(offset))));

        while(!reply->isFinished()) {
            QCoreApplication::processEvents();
        }
        QString jsonString = (QString)reply->readAll();
        res.append(getListRequest(jsonString, continuescan));
    }
    std::sort(res.begin(), res.end(),
              [](GameRecord* x, GameRecord* y){
        if (x->getTitle().size() != y->getTitle().size()) {
            return x->getTitle().size() < y->getTitle().size();
        }
        return x->getTitle() < y->getTitle();});
    return res;
}

QList <GameRecord*> UseAPI::getListRequest(QString jsonString, bool &continuescan)
{
    QList <GameRecord*> listFindedGames;
    QJsonDocument jsonResponse = QJsonDocument::fromJson(jsonString.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonArray jsonArray = jsonObject["games"].toArray();
    continuescan = jsonArray.size() == 100;


    foreach (const QJsonValue & value, jsonArray) {
        QJsonObject obj = value.toObject();

        bool isAddon = false;

        QJsonArray genres = obj["genres"].toArray();
        for (const QJsonValue &genre: genres) {
            QJsonObject genre_obj = genre.toObject();
            if (genre_obj["genre_id"].toInt() == 62) {
                isAddon = true;
                break;
            }
        }

        if (isAddon) {
            continue;
        }

        int id = (obj["game_id"].toInt());
        QString title = (obj["title"].toString());
        if (title.size() > 35) title.insert(title.size()/2, "\n");
        QString img = (obj["sample_cover"].toObject()["image"].toString());
        bool hidded = false;

        listFindedGames.append(new GameRecord(id, title, img, hidded));
    }

    return listFindedGames;
}
