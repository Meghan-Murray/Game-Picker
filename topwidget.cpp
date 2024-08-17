#include "topwidget.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "QLabel"

topWidget::topWidget(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout* vbox = new QVBoxLayout;
    data = new QLabel("data");
    data->setObjectName("standartText");
    data->setAlignment(Qt::AlignCenter);
    textEdit = new QPlainTextEdit("none");

    vbox->addWidget(data);
    vbox->addStretch();
    vbox->addWidget(textEdit);

    setLayout(vbox);
}

topWidget::topWidget(TopRecord topRecord,  QListWidgetItem* huy)
    : QWidget(nullptr)
{
    top = topRecord;
    huita = huy;
    QVBoxLayout* vbox = new QVBoxLayout;
    data = new QLabel(topRecord.time);
    data->setObjectName("standartText");
    data->setAlignment(Qt::AlignCenter);
    textEdit = new QPlainTextEdit(topRecord.gameList);
    textEdit->setReadOnly(true);

    deleteTopButton = new QPushButton();
    deleteTopButton->setIcon(QIcon(canPixmap));
    deleteTopButton->setIconSize(QSize(30,30));

    vbox->addWidget(data);
    vbox->addWidget(textEdit);
    vbox->addWidget(deleteTopButton);

    connect(deleteTopButton, SIGNAL(clicked()),this, SLOT(deleteTop()));

    setLayout(vbox);
}

void topWidget::deleteTop()
{
    myDatabase::getInstance().removeTopList(top);
    emit onDeleteTop(huita);
}
