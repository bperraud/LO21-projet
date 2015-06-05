#ifndef TABMANAGER_H
#define TABMANAGER_H

#include <QWidget>
#include <QTabWidget>

#include "Calendar.h"

class TabManager : public QWidget{
Q_OBJECT

private:
    QTabWidget* Onglets;
public:
    explicit TabManager(QWidget *parent = 0);
    ~TabManager();
    QTabWidget& getOnglets() const {return *Onglets;}

public slots:
    void switchTab(int index);
};

#endif // TABMANAGER_H
