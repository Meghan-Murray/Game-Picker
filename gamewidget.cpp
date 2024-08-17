#include "gamewidget.h"
#include "QHBoxLayout"
#include "QLabel"
#include "mydatabase.h"

gameWidget::gameWidget(QWidget *parent)
    : QWidget{parent}
{
    QHBoxLayout* hbox = new QHBoxLayout;
    name = new QLabel("name");
    name->setObjectName("smallText");
    img = new QLabel("img");

    hbox->addWidget(name);
    hbox->addStretch();
    hbox->addWidget(img);

    setLayout(hbox);
}

gameWidget::gameWidget(GameRecord* gameRec)
    : QWidget(nullptr)
{
    gameRecord = gameRec;
    QHBoxLayout* hbox = new QHBoxLayout;
    name = new QLabel(gameRecord->getTitle());
    name->setObjectName("smallText");
    img = new QLabel;
    img->setPixmap((gameRecord->getPixmap()).scaled(60, 60, Qt::KeepAspectRatio));
    img->setMinimumSize(60,60);

    deleteGameButton = new QPushButton();
    deleteGameButton->setIcon(QIcon(canPixmap));
    deleteGameButton->setIconSize(QSize(30,30));

    hideGameButton = new QPushButton();

    if (gameRecord->getIsHide() == false)
    {
        img->setDisabled(false);
        hideGameButton->setIcon(QIcon(eyeOpenPixmap));
    }
    else
    {
        img->setDisabled(true);
        hideGameButton->setIcon(QIcon(eyeClosePixmap));
    }

    hideGameButton->setIconSize(QSize(30,30));

    acceptGameButton = new QPushButton();
    acceptGameButton->setIcon(QIcon(acceptPixmap));
    acceptGameButton->setIconSize(QSize(30,30));

    hbox->addWidget(name);
    hbox->addStretch();
    hbox->addWidget(img);
    hbox->addWidget(hideGameButton);
    hbox->addWidget(deleteGameButton);
    hbox->addWidget(acceptGameButton);

    deleteGameButton->hide();
    hideGameButton->hide();
    acceptGameButton->hide();
    isClick = false;

    connect(deleteGameButton,SIGNAL(clicked()),this, SLOT(deleteGame()));
    connect(hideGameButton, SIGNAL(clicked()),this, SLOT(hideGame()));
    connect(acceptGameButton, SIGNAL(clicked()),this, SLOT(acceptButton()));

    setLayout(hbox);
}

void gameWidget::deleteGame()
{
    myDatabase::getInstance().removeRecord(gameRecord->getID());
    emit onDeleteGame();
}

void gameWidget::hideGame()
{
    if (gameRecord->getIsHide() == false)
    {
        gameRecord->setIsHide(true);
        img->setDisabled(true);
        myDatabase::getInstance().changeFlagHide(gameRecord->getID(), true);
        hideGameButton->setIcon(QIcon(eyeClosePixmap));
        hideGameButton->setIconSize(QSize(30,30));
    }
    else
    {
        gameRecord->setIsHide(false);
        img->setDisabled(false);
        myDatabase::getInstance().changeFlagHide(gameRecord->getID(), false);
        hideGameButton->setIcon(QIcon(eyeOpenPixmap));
        hideGameButton->setIconSize(QSize(30,30));
    }
}

void gameWidget::acceptButton()
{
    myDatabase::getInstance().addRecord(gameRecord);
    acceptGameButton->setEnabled(false);
    emit onAcceptGame();
}
