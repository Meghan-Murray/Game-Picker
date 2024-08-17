#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "adddialog.h"
#include "mydatabase.h"
#include "gamelist.h"
#include "topDialog.h"
#include <algorithm>
#include <random>
#include "topwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myDatabase::getInstance();

    //делиться списком игр через менюшку
    //ручное добавление игр
    // добавить проверку на множественное отправление поисковых запросов
    // анимация загрузки в поисковике
    // поиск по собственным играм

    QString smallText = "QLabel#smallText {font: 12pt '" + font + "';}";
    QString standartText = "QLabel#standartText {font: 14pt '" + font + "';}";
    QString buttonText = "QPushButton {font: 14px '" + font + "';}";
    QString planeText ="QPlainTextEdit {font: 11pt '" + font + "';}";
    QString lineText = "QLineEdit {font: 12pt '" + font + "';}";

    styleS = buttonText+standartText+planeText+smallText+lineText+"";
    setStyleSheet(styleS);


    ui->imgGame1->setIcon(QIcon(noneImg));
    ui->imgGame2->setIcon(QIcon(noneImg));

    ui->imgGame1->installEventFilter(this);
    ui->imgGame2->installEventFilter(this);

    ui->nameGame1->setText("none Game");
    ui->nameGame1->setObjectName("standartText");
    ui->nameGame2->setText("none Game");
    ui->nameGame2->setObjectName("standartText");
    ui->VSLable->setObjectName("standartText");

  //  ui->bothButton->setStyleSheet(buttonText);
 //   ui->bothButton->setObjectName("hui");

    connect(ui->TopButton, &QPushButton::clicked, this, &MainWindow::onTopListButtonClicked);
    connect(ui->SearchGameButton, &QPushButton::clicked, this, &MainWindow::onSearchGameButtonClicked);
    connect(ui->ListGameButton, &QPushButton::clicked, this, &MainWindow::onListGameButtonClicked);
    connect(ui->RefreshButton, &QPushButton::clicked, this, &MainWindow::onRefreshChoice);

    connect(ui->imgGame1, &QPushButton::clicked, this, &MainWindow::clickOnGame);
    connect(ui->imgGame2, &QPushButton::clicked, this, &MainWindow::clickOnGame);
    connect(ui->bothButton, &QPushButton::clicked, this, &MainWindow::skipGameButton);

    startChoice();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onSearchGameButtonClicked()
{
    AddDialog addDialog;
    addDialog.setStyleSheet(styleS);
    addDialog.setModal(true);
    addDialog.setWindowFlags(Qt::CustomizeWindowHint);
    addDialog.exec();
}

void MainWindow::onListGameButtonClicked()
{
    gameList listWidget;
    listWidget.setStyleSheet(styleS);
    listWidget.setModal(true);
    listWidget.setWindowFlags(Qt::CustomizeWindowHint);
    listWidget.exec();
}

void MainWindow::onRefreshChoice()
{
    savedList.clear();
    allGamesList.clear();
    bufferGameList.clear();
    startChoice();
}

void MainWindow::onTopListButtonClicked()
{
    TopDialog topDialog;
    topDialog.setStyleSheet(styleS);
    topDialog.setModal(true);
    topDialog.setWindowFlags(Qt::CustomizeWindowHint);
    topDialog.exec();
}

void MainWindow::skipGameButton()
{
    if (!canChoice) return;
    allGamesList[0]->record += 1;
    allGamesList[1]->record += 1;

    bufferGameList.append(allGamesList[0]);
    bufferGameList.append(allGamesList[1]);
    allGamesList.remove(0, 2);

    if (firstGame == nullptr) firstGame = bufferGameList[0];

    if (allGamesList.size() > 0)
    {
        pairwiseComparison();
    }
    else
    {
        continueChoice();
    }
}

void MainWindow::clickOnGame()
{
    if (!canChoice) return;

    if(sender() == ui->imgGame1) {
        emit clickedGame(allGamesList[0]);
        allGamesList[0]->record += 1;
        bufferGameList.append(allGamesList[0]);
    }
    else{
        emit clickedGame(allGamesList[1]);
        allGamesList[1]->record += 1;
        bufferGameList.append(allGamesList[1]);
    }

    if (firstGame == nullptr) firstGame = bufferGameList[0];

    allGamesList.remove(0, 2);

    if (allGamesList.size() > 0)
    {
        pairwiseComparison();
    }
    else
    {
        if (bufferGameList.size() < 2)
        {
            std::sort(savedList.begin(), savedList.end(), [](GameRecord *x,  GameRecord *y) {return x->record > y->record;});
           // for (auto &x: savedList) {
           //     qDebug() << x->getTitle() << " " << x->record;
           // }
            while(savedList.size()>6) savedList.removeLast();

            myDatabase::getInstance().addTopList(savedList);
            topWidget *aboba = new topWidget(myDatabase::getInstance().getAllTopLists()[0], nullptr);
            aboba->setStyleSheet(styleS);
            aboba->setWindowModality(Qt::WindowModality::ApplicationModal);
            aboba->deleteTopButton->hide();
            aboba->show();
            onRefreshChoice();
            return;
        }
        continueChoice();
    }
}

void MainWindow::startChoice()
{
    auto rd = std::random_device {};
    auto rng = std::default_random_engine { rd() };
    allGamesList = myDatabase::getInstance().getVisibleRecords();
    std::shuffle(allGamesList.begin(), allGamesList.end(), rng);

    savedList = allGamesList;

    firstGame = nullptr;
    pairwiseComparison();
}

void MainWindow::continueChoice()
{
    auto rd = std::random_device {};
    auto rng = std::default_random_engine { rd() };
    allGamesList = bufferGameList;
    bufferGameList.clear();
    std::shuffle(allGamesList.begin(), allGamesList.end(), rng);

    firstGame = nullptr;
    pairwiseComparison();
}

void MainWindow::pairwiseComparison()
{
    if (allGamesList.empty())
    {
        ui->imgGame1->setIcon(QIcon(noneImg));
        ui->imgGame2->setIcon(QIcon(noneImg));
        ui->nameGame1->setText("none Game");
        ui->nameGame2->setText("none Game");
        canChoice = false;
        //в прогресс бар написать, что не с чем сравнивать. Добавьте больше игр
        return;
    }

    if (allGamesList.size() == 1 && firstGame == nullptr)
    {
        ui->imgGame1->setIcon(QIcon(allGamesList[0]->getPixmap()));
        ui->imgGame2->setIcon(QIcon(noneImg));
        ui->nameGame1->setText(allGamesList[0]->getTitle());
        ui->nameGame2->setText("none Game");
        canChoice = false;
        //в прогресс бар написать, что не с чем сравнивать. Добавьте больше игр
        return;
    }

    if (allGamesList.size() == 1)
    {
        ui->imgGame1->setIcon(QIcon(allGamesList[0]->getPixmap()));
        ui->imgGame2->setIcon(QIcon(firstGame->getPixmap()));
        ui->nameGame1->setText(allGamesList[0]->getTitle());
        ui->nameGame2->setText(firstGame->getTitle());
        canChoice = true;

        if (bufferGameList.contains(firstGame)) bufferGameList.removeFirst();
        allGamesList.append(firstGame);
        return;
    }

        ui->imgGame1->setIcon(QIcon(allGamesList[0]->getPixmap()));
        ui->imgGame2->setIcon(QIcon(allGamesList[1]->getPixmap()));

        ui->nameGame1->setText(allGamesList[0]->getTitle());
        ui->nameGame2->setText(allGamesList[1]->getTitle());
        canChoice = true;
}

bool  MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (((watched == ui->imgGame1) || (watched == ui->imgGame2)) && ((event->type() == QEvent::HoverEnter)))
    {
        return true;
    }

    return false;
}



