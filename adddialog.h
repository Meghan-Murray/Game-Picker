#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include "QListWidgetItem"
#include "gamerecord.h"
#include "gameWidget.h"
#include <QSet>

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent = nullptr);
    void AddGameWidget(GameRecord* gameRecord);
    ~AddDialog();

private slots:

    void onSearchClicked();
    void onListItemClicked(QListWidgetItem* item);
    void fixSelectedGame();

private:
    Ui::AddDialog *ui;
    QPushButton* backButton;
    void keyPressEvent(QKeyEvent *);
    gameWidget* prevItem = nullptr;
    QSet <int> existedID;
};

#endif // ADDDIALOG_H
