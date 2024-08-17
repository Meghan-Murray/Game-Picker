#ifndef TOPDIALOG_H
#define TOPDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QPushButton>
#include "mydatabase.h"
#include "QListWidgetItem"

namespace Ui {
class TopDialog;
}

class TopDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TopDialog(QWidget *parent = nullptr);
    ~TopDialog();

private:
    Ui::TopDialog *ui;
    QPushButton* backButton;
    void addTopsWidget(TopRecord topRecord);

private slots:
    void deleteSelectedTop(QListWidgetItem*);
};

#endif // TOPDIALOG_H
