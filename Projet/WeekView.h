#ifndef WEEKVIEW_H
#define WEEKVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QStandardItem>
#include <QTableView>
#include <QLabel>
#include <QCalendarWidget>

#include "Calendar.h"

class WeekView : public QWidget{
Q_OBJECT
private:
    QScrollArea *scrollarea;
    QStandardItemModel* modelW;
    QTableView* weekView;
    QStringList ListJours, ListHeures;
    QCalendarWidget* calendar;
    QVBoxLayout* layout;

    QDate date;

public slots:
    void updateWeekView();

public:
    explicit WeekView(QWidget *parent = 0);
    ~WeekView();
};

inline int calculeJour(const QDate& d, int rang){ return d.addDays(-d.dayOfWeek()+rang).day(); }
inline int calculeMois(const QDate& d, int rang){ return d.addDays(-d.dayOfWeek()+rang).month(); }

#endif // WEEKVIEW_H
