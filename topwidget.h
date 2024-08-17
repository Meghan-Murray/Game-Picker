#ifndef TOPWIDGET_H
#define TOPWIDGET_H

#include <QWidget>
#include <QPixmap>
#include "QPushButton"
#include "QLabel"
#include "mydatabase.h"
#include "QTextEdit"
#include "QPlainTextEdit"
#include "QListWidgetItem"

class topWidget : public QWidget
{
    Q_OBJECT

public:
    explicit topWidget(QWidget *parent = nullptr);
    QLabel* data;
    QPlainTextEdit* textEdit;
    topWidget(TopRecord topRecord, QListWidgetItem* huy);
    QListWidgetItem* huita = nullptr;
    QPushButton* deleteTopButton;

private:
    QPixmap canPixmap = QPixmap(":/new/picture/img/can.png");
    TopRecord top;

private slots:
    void deleteTop();

signals:
    void onDeleteTop(QListWidgetItem*);
};

#endif // TOPWIDGET_H
