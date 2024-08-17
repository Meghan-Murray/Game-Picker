#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include "gamerecord.h"
#include <QPixmap>
#include "QPushButton"
#include "QLabel"

class gameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit gameWidget(QWidget *parent = nullptr);
    gameWidget(GameRecord* gameRec);
    GameRecord* gameRecord;
    bool isClick;

    QPushButton* deleteGameButton;
    QPixmap canPixmap = QPixmap(":/new/picture/img/can.png");

    QPushButton* hideGameButton;
    QPixmap eyeOpenPixmap = QPixmap(":/new/picture/img/eyeOpen.png");
    QPixmap eyeClosePixmap = QPixmap(":/new/picture/img/eyeClose.png");

    QPushButton* acceptGameButton;
    QPixmap acceptPixmap = QPixmap(":/new/picture/img/check.png");

private:
    QLabel* name;
    QLabel* img;

signals:
    void onDeleteGame();
    void onAcceptGame();

private slots:
    void deleteGame();
    void hideGame();
    void acceptButton();
};

#endif // GAMEWIDGET_H
