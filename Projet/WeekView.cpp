#include <QHeaderView>
#include "WeekView.h"

WeekView::WeekView(const QDate& d, QWidget *parent) : QWidget(parent), date(d){

    scrollarea = new QScrollArea;

    //choixSemaine = new QLabel("Semaine du : ", this);
    calendar = new QCalendarWidget(this);
    calendar->setHorizontalHeaderFormat(QCalendarWidget::LongDayNames);
    calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    calendar->setGridVisible(true);

    weekView = new QTableView(this);
    ListJours << QString("Lundi %0").arg(calculeJour(date, 1))
              << QString("Mardi %0").arg(calculeJour(date, 2))
              << QString("Mercredi %0").arg(calculeJour(date, 3))
              << QString("Jeudi %0").arg(calculeJour(date, 4))
              << QString("Vendredi %0").arg(calculeJour(date, 5))
              << QString("Samedi %0").arg(calculeJour(date, 6))
              << QString("Dimanche %0").arg(calculeJour(date, 7));
    for (unsigned int i = 0; i < 48 ; ++i){
        int heure = i/4;
        if (!(i % 4)) ListHeures << QString("%0h00").arg(heure+8);
        else ListHeures << QString();
    }

    modelW = new QStandardItemModel(this);
    modelW->setHorizontalHeaderLabels(ListJours);
    modelW->setVerticalHeaderLabels(ListHeures);
    for (int jour = 0; jour < 7; ++jour){

        for (int heure = 0; heure < 24; ++heure){

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

    layout = new QVBoxLayout;
    layoutSemaine = new QHBoxLayout;

    scrollarea->setWidget(weekView);
    scrollarea->setWidgetResizable(true);
    scrollarea->setFixedHeight(600);

    //layoutSemaine->addWidget(choixSemaine);
    layoutSemaine->addWidget(calendar);

    layout->addLayout(layoutSemaine);
    layout->addWidget(scrollarea);

    this->setLayout(layout);

    // Gestion des signaux

    QObject::connect(calendar, SIGNAL(selectionChanged()), this, SLOT(updateWeekView()));

}

WeekView::~WeekView(){}

void WeekView::updateWeekView(){

    // Traitement des headers
    date = calendar->selectedDate();
    ListJours.clear();
    ListJours << QString("Lundi %0").arg(calculeJour(date, 1))
              << QString("Mardi %0").arg(calculeJour(date, 2))
              << QString("Mercredi %0").arg(calculeJour(date, 3))
              << QString("Jeudi %0").arg(calculeJour(date, 4))
              << QString("Vendredi %0").arg(calculeJour(date, 5))
              << QString("Samedi %0").arg(calculeJour(date, 6))
              << QString("Dimanche %0").arg(calculeJour(date, 7));
    modelW->setHorizontalHeaderLabels(ListJours);

    // Traitement des programmations

    ProgTacheManager& PTM = *ProgTacheManager::getInstance();
    for (ProgTacheManager::iterator i = PTM.begin(); i != PTM.end(); ++i)
        if ((*i).getDate() >= date.addDays(-date.dayOfWeek()+1) && (*i).getDate() <= date.addDays(-date.dayOfWeek()+7)){
            qDebug() << "coco\n";

        }

}








