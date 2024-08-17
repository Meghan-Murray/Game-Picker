#include "topdialog.h"
#include "ui_topdialog.h"
#include "mydatabase.h"
#include "topwidget.h"

TopDialog::TopDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TopDialog)
{
    ui->setupUi(this);
    ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->listWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->label->setObjectName("standartText");

    backButton = new QPushButton("Back");
    ui->verticalLayout->addWidget(backButton);
    connect(backButton,SIGNAL(clicked()),this, SLOT(reject()));

    QVector <TopRecord> gameList = myDatabase::getInstance().getAllTopLists();

    for (auto &item : gameList)
    {
        addTopsWidget(item);
    }
}

void TopDialog::addTopsWidget(TopRecord topRecord)
{
    QListWidgetItem *item = new QListWidgetItem();
    topWidget *widg = new topWidget(topRecord, item);
    widg->setStyleSheet(ui->listWidget->styleSheet());
    connect(widg, SIGNAL(onDeleteTop(QListWidgetItem*)), this, SLOT(deleteSelectedTop(QListWidgetItem*)));
    item->setSizeHint(widg->sizeHint());
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, widg);
}

void TopDialog::deleteSelectedTop(QListWidgetItem* thisHuinya)
{
    QListWidgetItem *item = thisHuinya;
    ui->listWidget->removeItemWidget(item);
    delete item;
}

TopDialog::~TopDialog()
{
    delete ui;
}

