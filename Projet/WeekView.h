#ifndef WEEKVIEW_H
#define WEEKVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QStandardItem>
#include <QTableView>

#include "Calendar.h"
#include "TacheManager.h"

class WeekView : public QWidget{
Q_OBJECT
private:
    QScrollArea *scrollarea;
    QStandardItemModel* modelW;
    QTableView* weekView;
    QStringList ListJours, ListHeures;

    QVBoxLayout* layout;



public:
    explicit WeekView(QWidget *parent = 0);
    ~WeekView();

signals:

public slots:
};

#endif // WEEKVIEW_H
