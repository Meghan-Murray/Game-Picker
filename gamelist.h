#ifndef GAMELIST_H
#define GAMELIST_H

#include <QDialog>
#include "QPushButton"
#include "gamerecord.h"
#include "QListWidgetItem"
#include "gamewidget.h"

namespace Ui {
class gameList;
}

class gameList : public QDialog
{
    Q_OBJECT

public:
    explicit gameList(QWidget *parent = nullptr);
    ~gameList();

private:
    void AddGameWidget(GameRecord* gameRecord);
    Ui::gameList *ui;
    QPushButton* backButton;
    gameWidget* prevItem;

private slots:
    void onListItemClicked(QListWidgetItem *item);
    void deleteSelectedGame();
};

#endif // GAMELIST_H
