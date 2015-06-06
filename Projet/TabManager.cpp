#include <typeinfo>

#include "TabManager.h"

#include "TacheEditeur.h"
#include "ProgCreator.h"
#include "WeekView.h"
#include "TreeView.h"

TabManager::TabManager(QWidget *parent) : QWidget(parent){

    Onglets = new QTabWidget(this);
    Onglets->setMinimumSize(QSize(800, 800));

    QObject::connect(Onglets, SIGNAL(currentChanged(int)), this, SLOT(switchTab(int)));

}

TabManager::~TabManager(){}

void TabManager::switchTab(int index){
    qDebug() << "index : " << QString::number(index) << "\n";
    switch (index){
    case 0:
        /*qDebug() << QString::number(reinterpret_cast<int>(Onglets->currentWidget()));
        qDebug() << QString((typeid(*Onglets->currentWidget()).name()));
        qDebug() << QString((typeid(WeekView).name()));
        qDebug() << QString((typeid(QWidget).name()));
        qDebug() << QString::number(reinterpret_cast<int>(Onglets->currentWidget()->layout()));
        qDebug() << QString::number(Onglets->currentWidget()->layout()->count());
        qDebug() << QString::number(reinterpret_cast<int>(Onglets->currentWidget()->layout()->itemAt(0)->widget()));
        qDebug() << QString((typeid(*Onglets->currentWidget()->layout()->itemAt(0)->widget()).name()));
        qDebug() << QString::number(reinterpret_cast<int>(qobject_cast<WeekView*>(Onglets->currentWidget()->layout()->itemAt(0)->widget())));*/
        qobject_cast<WeekView*>(Onglets->currentWidget()->layout()->itemAt(0)->widget())->updateWeekView();
        break;
    case 1:
        qobject_cast<TreeView*>(Onglets->currentWidget()->layout()->itemAt(0)->widget())->updateTrees();
        break;
    case 2:
        qobject_cast<TacheEditeur*>(Onglets->currentWidget()->layout()->itemAt(0)->widget())->updateT();
        break;
    case 3:
        qobject_cast<ProgCreator*>(Onglets->currentWidget()->layout()->itemAt(0)->widget())->updateTachesU();
        break;
    }
}
