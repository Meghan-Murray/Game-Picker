#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "GameRecord.h"
#include <QMainWindow>
#include <QEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QVector <GameRecord*> allGamesList;
    QVector <GameRecord*> bufferGameList;
    QVector <GameRecord*> savedList;
    void startChoice();
    void continueChoice();
    void pairwiseComparison();

    GameRecord* firstGame;

    QPixmap noneImg = QPixmap(":/new/picture/img/noneImg.png");

    QString font = "Segoe Print";;
    QString styleS;
    bool canChoice;

    bool eventFilter(QObject *obj, QEvent *event);

private slots:

    void onSearchGameButtonClicked();
    void onListGameButtonClicked();
    void onRefreshChoice();
    void onTopListButtonClicked();

    void skipGameButton();
    void clickOnGame();

signals:
    void clickedGame(GameRecord*);

};
#endif // MAINWINDOW_H
