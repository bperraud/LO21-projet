#include <QHeaderView>
#include <QFont>
#include <QBrush>

#include "WeekView.h"
#include "ProgManager.h"

WeekView::WeekView(QWidget *parent) : QWidget(parent){

    calendar = new QCalendarWidget(this);
    calendar->setHorizontalHeaderFormat(QCalendarWidget::LongDayNames);
    calendar->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    calendar->setGridVisible(true);

    weekView = new QTableView(this);

    for (unsigned int i = 0; i < 48 ; ++i){
        int heure = i/4;
        if (!(i % 4)) ListHeures << QString("%0h00").arg(heure+8);
        else ListHeures << QString();
    }

    modelW = new QStandardItemModel(this);
    modelW->setHorizontalHeaderLabels(ListJours);
    modelW->setVerticalHeaderLabels(ListHeures);
    for (int jour = 0; jour < 7; ++jour){
        for (int quartHeure = 0; quartHeure < 48; ++quartHeure){
            QStandardItem *item = new QStandardItem();
            modelW->setItem(quartHeure, jour, item);
            modelW->item(quartHeure, jour)->setEditable(false);
            item->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
            item->setFont(QFont("Helvetica", 9, QFont::Bold));
        }
    }

    QHeaderView* vHeader = weekView->verticalHeader();
    vHeader->setSectionResizeMode(QHeaderView::Fixed);
    vHeader->setDefaultSectionSize(20);

    weekView->setModel(modelW);

    // Initialisation

    updateWeekView();

    // Layout & Scroll

    layout = new QVBoxLayout;

    calendar->setFixedHeight(200);

    scrollarea = new QScrollArea;
    scrollarea->setWidget(weekView);
    scrollarea->setWidgetResizable(true);
    scrollarea->setFixedHeight(500);

    layout->addWidget(calendar);
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
    ListJours << QString("Lundi %0/%1").arg(calculeJour(date, 1)).arg(calculeMois(date, 1))
              << QString("Mardi %0/%1").arg(calculeJour(date, 2)).arg(calculeMois(date, 2))
              << QString("Mercredi %0/%1").arg(calculeJour(date, 3)).arg(calculeMois(date, 3))
              << QString("Jeudi %0/%1").arg(calculeJour(date, 4)).arg(calculeMois(date, 4))
              << QString("Vendredi %0/%1").arg(calculeJour(date, 5)).arg(calculeMois(date, 5))
              << QString("Samedi %0/%1").arg(calculeJour(date, 6)).arg(calculeMois(date, 6))
              << QString("Dimanche %0/%1").arg(calculeJour(date, 7)).arg(calculeMois(date, 7));
    modelW->setHorizontalHeaderLabels(ListJours);

    // Clean de la view
    for (int jour = 0; jour < 7; ++jour){
        for (int quartHeure = 0; quartHeure < 48; ++quartHeure){
            modelW->item(quartHeure, jour)->setText("");
            modelW->item(quartHeure, jour)->setBackground(QBrush(QColor(Qt::white)));
        }
    }
    weekView->clearSpans();

    // Traitement des programmations
    ProgManager& ProgM = *ProgManager::getInstance();
    for (ProgManager::iterator i = ProgM.begin(); i != ProgM.end(); ++i){
        if ((*i).getDate() >= date.addDays(-date.dayOfWeek()+1) && (*i).getDate() <= date.addDays(-date.dayOfWeek()+7)){
            QStandardItem* item;
            int dureeEnMin = ((*i).getHoraireFin().minute() + (*i).getHoraireFin().hour()*60
                              - (*i).getHoraire().minute() - (*i).getHoraire().hour()*60);
            int nbQuartsH = dureeEnMin/15;
            if (!dureeEnMin%15) nbQuartsH++;
            int trancheQH = (*i).getHoraire ().minute()/15;
            if ((*i).getHoraire ().minute()%15) nbQuartsH++;
            item = modelW->item(((*i).getHoraire().hour()-8)*4+trancheQH, (*i).getDate().dayOfWeek()-1);
            item->setText((*i).getTitre());
            if (item->row() + nbQuartsH >= 48) throw CalendarException("Erreur, WeekView, durée de la prog dépasse 20h...\n");
            if (nbQuartsH > 1) weekView->setSpan(item->row(), item->column(), nbQuartsH, 1);
            if ((*i).isProgTache()) item->setBackground(QBrush(QColor(190, 200, 255)));
            else item->setBackground(QBrush(QColor(255, 200, 190)));
        }
    }
}
