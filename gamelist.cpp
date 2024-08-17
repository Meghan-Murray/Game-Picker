#include "gamelist.h"
#include "ui_gamelist.h"
#include "mydatabase.h"
#include "gamewidget.h"

gameList::gameList(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::gameList)
{
    ui->setupUi(this);
    ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->label->setObjectName("standartText");

    backButton = new QPushButton("Back");
    ui->verticalLayout->addWidget(backButton);
    prevItem = nullptr;

    connect(ui->listWidget, SIGNAL(itemPressed(QListWidgetItem*)),
            this, SLOT(onListItemClicked(QListWidgetItem*)));
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(onListItemClicked(QListWidgetItem*)));

    connect(backButton,SIGNAL(clicked()),this, SLOT(reject()));

    QVector <GameRecord*> gameList = myDatabase::getInstance().getAllRecords();

    for (auto &item : gameList)
    {
        AddGameWidget(item);
    }
}

void gameList::AddGameWidget(GameRecord* gameRecord)
{
    QListWidgetItem *item = new QListWidgetItem();
    gameWidget *widg = new gameWidget(gameRecord);
    widg->setStyleSheet(ui->listWidget->styleSheet());
    connect(widg, SIGNAL(onDeleteGame()), this, SLOT(deleteSelectedGame()));
    item->setSizeHint(widg->sizeHint());
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, widg);
}

void gameList::onListItemClicked(QListWidgetItem *item)
{
    gameWidget* myitem = (gameWidget*) ui->listWidget->itemWidget(item);

    if (myitem->isClick == false)
    {
        myitem->isClick = true;
        myitem->deleteGameButton->show();
        myitem->hideGameButton->show();

    } else {
        prevItem->isClick = false;
        prevItem->deleteGameButton->hide();
        prevItem->hideGameButton->hide();
    }
    if (prevItem == myitem) return;

    if (prevItem) {
        prevItem->isClick = false;
        prevItem->deleteGameButton->hide();
        prevItem->hideGameButton->hide();
    }
    prevItem = myitem;
}

void gameList::deleteSelectedGame()
{
    QListWidgetItem *item = ui->listWidget->selectedItems()[0];
    ui->listWidget->removeItemWidget(item);
    delete item;
    prevItem = nullptr;
}

gameList::~gameList()
{
    delete ui;
}
