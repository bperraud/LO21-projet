#include <QHeaderView>
#include "WeekView.h"

WeekView::WeekView(QWidget *parent) : QWidget(parent){

    scrollarea = new QScrollArea;
    layout = new QVBoxLayout;

    weekView = new QTableView(this);
    ListJours << "Lundi" << "Mardi" << "Mercredi" << "Jeudi" << "Vendredi" << "Samedi" << "Dimanche";
    for (unsigned int i = 0; i < 48 ; ++i){
        int heure = i/4;
        if (!(i % 4)) ListHeures << QString("%0h00").arg(heure+8);
        else ListHeures << QString();
    }

    modelW = new QStandardItemModel(this);



    for (int jour = 0; jour < 7; ++jour){
        modelW->setHorizontalHeaderLabels(ListJours);
        for (int heure = 0; heure < 24; ++heure){
            modelW->setVerticalHeaderLabels(ListHeures);
            QStandardItem *item = new QStandardItem("blabla");
            modelW->setItem(heure, jour, item);
            modelW->item(heure, jour)->setEditable(false);
        }
    }

    QHeaderView* vHeader = weekView->verticalHeader();
    vHeader->setSectionResizeMode(QHeaderView::Fixed);
    vHeader->setDefaultSectionSize(20);

    weekView->setSpan(0, 0, 2, 1); //sets the 1st row 1st column cell to span over 2 rows and 1 column

    weekView->setModel(modelW);

    // Layout

    scrollarea->setWidget(weekView);
    scrollarea->setWidgetResizable(true);
    scrollarea->setFixedHeight(600);


    layout->addWidget(scrollarea);


    this->setLayout(layout);

}

WeekView::~WeekView(){}










