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
        //qobject_cast<WeekView*>(Onglets->currentWidget())->updateWeekView();
        break;
    case 1:
        //qobject_cast<TreeView*>(Onglets->currentWidget())->updateTrees();
        break;
    }
}
