#include "TabManager.h"

#include "TacheEditeur.h"
#include "ProgCreator.h"
#include "WeekView.h"
#include "TreeView.h"
#include "ImExManager.h"
#include "ProjetCreator.h"
#include "RemoveManager.h"

TabManager::TabManager(QWidget *parent) : QWidget(parent){

    Onglets = new QTabWidget(this);
    Onglets->setMinimumSize(QSize(800, 800));

    QObject::connect(Onglets, SIGNAL(currentChanged(int)), this, SLOT(switchTab(int)));

}

TabManager::~TabManager(){}

void TabManager::switchTab(int index){
    switch (index){
    case 0:
        qobject_cast<WeekView*>(Onglets->currentWidget()->layout()->itemAt(0)->widget())->updateWeekView();
        break;
    case 1:
        qobject_cast<TreeView*>(Onglets->currentWidget()->layout()->itemAt(0)->widget())->updateTrees();
        break;
    case 2:
        qobject_cast<TacheCreator*>(Onglets->currentWidget()->layout()->itemAt(0)->widget())->updateTC();
        break;
    case 3:
        qobject_cast<ProjetCreator*>(Onglets->currentWidget()->layout()->itemAt(0)->widget())->updatePC();
        break;
    case 4:
        qobject_cast<TacheEditeur*>(Onglets->currentWidget()->layout()->itemAt(0)->widget())->updateT();
        break;
    case 5:
        qobject_cast<ProjetEditeur*>(Onglets->currentWidget()->layout()->itemAt(0)->widget())->updateProjEdit();
        break;
    case 6:
        qobject_cast<ProgCreator*>(Onglets->currentWidget()->layout()->itemAt(0)->widget())->updateTachesU();
        break;
    case 7:
        qobject_cast<RemoveManager*>(Onglets->currentWidget()->layout()->itemAt(0)->widget())->updateRm();
        break;
    case 8:
        qobject_cast<ImExManager*>(Onglets->currentWidget()->layout()->itemAt(0)->widget())->updateProj();
        break;
    }
}
