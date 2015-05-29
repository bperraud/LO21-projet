#ifndef WEEKVIEW_H
#define WEEKVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QStandardItem>
#include <QTableView>
#include <QLabel>
#include <QCalendarWidget>

#include "Calendar.h"
#include "TacheManager.h"

class WeekView : public QWidget{
Q_OBJECT
private:
    QScrollArea *scrollarea;
    QStandardItemModel* modelW;
    QTableView* weekView;
    QStringList ListJours, ListHeures;
    QLabel* choixSemaine;
    QCalendarWidget* calendar;
    QHBoxLayout* layoutSemaine;
    QVBoxLayout* layout;

    QDate date;

public slots:
    void updateWeekView();

public:
    explicit WeekView(const QDate& d = QDate::currentDate(), QWidget *parent = 0);
    ~WeekView();

signals:

public slots:
};

inline int calculeJour(const QDate& d, int rang){ return d.addDays(-d.dayOfWeek()+rang).day(); }

#endif // WEEKVIEW_H
