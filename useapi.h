#ifndef USEAPI_H
#define USEAPI_H

#include <QMainWindow>
#include <QObject>
#include "QtNetwork/QNetworkAccessManager"
#include "QtNetwork/QNetworkReply"
#include "QtNetwork/QNetworkRequest"
#include "gamerecord.h"

class UseAPI : public QObject
{
    Q_OBJECT


    QNetworkAccessManager *manager;


public:
    explicit UseAPI(QObject *parent = nullptr);
    QList <GameRecord*>  searchGame(QString titleSearch);
    QList <GameRecord*> getListRequest(QString jsonString, bool &continuescan);

};

#endif // USEAPI_H
