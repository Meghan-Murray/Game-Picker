#include "adddialog.h"
#include "ui_adddialog.h"
#include "useapi.h"
#include "gamerecord.h"
#include <QKeyEvent>
#include <QListWidgetItem>
#include <QPixmap>
#include <QResource>
#include "mydatabase.h"

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
    ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->label->setObjectName("standartText");

    prevItem = nullptr;

    QList <GameRecord*> basesGames = myDatabase::getInstance().getAllRecords();
    for (auto game : basesGames)
    {
        existedID.insert(game->getID());
    }

    backButton = new QPushButton("Back");
    ui->verticalLayout->addWidget(backButton);

    connect(ui->searchButton,SIGNAL(clicked()),this, SLOT(onSearchClicked()));
    connect(backButton,SIGNAL(clicked()),this, SLOT(reject()));

    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)),
            this, SLOT(onListItemClicked(QListWidgetItem*)));
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(onListItemClicked(QListWidgetItem*)));
}

void AddDialog::AddGameWidget(GameRecord* gameRecord)
{

    QListWidgetItem *item = new QListWidgetItem();
    gameWidget *widg = new gameWidget(gameRecord);
    connect(widg, SIGNAL(onAcceptGame()), this, SLOT(fixSelectedGame()));
    item->setSizeHint(widg->sizeHint());
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, widg);

    if (existedID.contains(gameRecord->getID()))
        widg->setDisabled(true);
}


AddDialog::~AddDialog()
{
    delete ui;
}

void AddDialog::keyPressEvent(QKeyEvent * event)
{
    if((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return))
        onSearchClicked();
//    else
//       QDialog::keyPressEvent(event);
}


void AddDialog::onSearchClicked()
{
    ui->listWidget->clear();
    prevItem = nullptr;

    QString string;
    string = ui->lineEdit->text();
    if (string.size() < 1){
        return;
    }

    UseAPI useAPI;
    QList <GameRecord*> gameList = useAPI.searchGame(string);

    for (auto &item : gameList)
    {
        AddGameWidget(item);
    }
    qDebug() << gameList.size();

}

void AddDialog::onListItemClicked(QListWidgetItem *item)
{

    gameWidget* myitem = (gameWidget*) ui->listWidget->itemWidget(item);

    if (!myitem->isEnabled()) return;

    if (myitem->isClick == false)
    {
        myitem->isClick = true;
        myitem->acceptGameButton->show();
    } else {
        prevItem->isClick = false;
        prevItem->acceptGameButton->hide();
    }
    if (prevItem == myitem)
    {
        return;
    }

    if (prevItem) {
        prevItem->isClick = false;
        prevItem->acceptGameButton->hide();
    }
    else {
            prevItem = myitem;
    }
        prevItem = myitem;
}

void AddDialog::fixSelectedGame()
{
    QListWidgetItem *item = ui->listWidget->selectedItems()[0];
    gameWidget* myitem = (gameWidget*) ui->listWidget->itemWidget(item);
    myitem->setDisabled(true);
    existedID.insert(myitem->gameRecord->getID());
}

