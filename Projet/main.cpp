#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>
#include <QFileDialog>
#include <QTabWidget>
#include <QScrollArea>

#include "Calendar.h"
#include "TacheManager.h"
#include "ProgManager.h"
#include "PrecedenceManager.h"

#include "TabManager.h"

#include "TacheCreator.h"
#include "TacheEditeur.h"
#include "ProgCreator.h"
#include "WeekView.h"
#include "TreeView.h"
#include "ImExManager.h"
#include "ProjetCreator.h"
#include "ProjetEditeur.h"
#include "RemoveManager.h"


int main(int argc, char *argv[]){

    QApplication app(argc, argv);
    QWidget* fenetre = new QWidget;
    fenetre->setMinimumSize(QSize(800, 800));

    WeekView* WV = new WeekView;
    TreeView* TV = new TreeView;
    TacheEditeur* TE = new TacheEditeur;
    ProgCreator* PC = new ProgCreator;
    ImExManager* IEM = new ImExManager;
    TacheCreator* TCR = new TacheCreator;
    ProjetCreator* ProjC = new ProjetCreator;
    ProjetEditeur* ProjE = new ProjetEditeur;
    RemoveManager* RM = new RemoveManager;

    QVBoxLayout *layoutWeekView, *layoutTacheEditeur, *layoutTreeView, *layoutProgCreator, *layoutImExManager, *layoutTacheCreator, *layoutProjetCreator, *layoutProjetEditeur, *layoutRemoveManager;
    layoutWeekView = new QVBoxLayout;
    layoutTacheEditeur = new QVBoxLayout;
    layoutTreeView = new QVBoxLayout;
    layoutProgCreator = new QVBoxLayout;
    layoutImExManager = new QVBoxLayout;
    layoutTacheCreator = new QVBoxLayout;
    layoutProjetCreator = new QVBoxLayout;
    layoutProjetEditeur = new QVBoxLayout;
    layoutRemoveManager = new QVBoxLayout;

    layoutWeekView->addWidget(WV);
    layoutTreeView->addWidget(TV);
    layoutTacheEditeur->addWidget(TE);
    layoutProgCreator->addWidget(PC);
    layoutImExManager->addWidget(IEM);
    layoutTacheCreator->addWidget(TCR);
    layoutProjetCreator->addWidget(ProjC);
    layoutProjetEditeur->addWidget(ProjE);
    layoutRemoveManager->addWidget(RM);

    TabManager* TabM = new TabManager(fenetre);
    QWidget *ongletWeekView, *ongletTacheEditeur, *ongletTreeView, *ongletProgCreator, *ongletImExManager, *ongletTacheCreator, *ongletProjetCreator, *ongletProjetEditeur, *ongletRemoveManager;
    ongletWeekView = new QWidget;
    ongletTacheEditeur = new QWidget;
    ongletTreeView = new QWidget;
    ongletProgCreator = new QWidget;
    ongletImExManager = new QWidget;
    ongletTacheCreator = new QWidget;
    ongletProjetCreator = new QWidget;
    ongletProjetEditeur = new QWidget;
    ongletRemoveManager = new QWidget;

    ongletWeekView->setLayout(layoutWeekView);
    ongletTreeView->setLayout(layoutTreeView);
    ongletTacheEditeur->setLayout(layoutTacheEditeur);
    ongletProgCreator->setLayout(layoutProgCreator);
    ongletImExManager->setLayout(layoutImExManager);
    ongletTacheCreator->setLayout(layoutTacheCreator);
    ongletProjetCreator->setLayout(layoutProjetCreator);
    ongletProjetEditeur->setLayout(layoutProjetEditeur);
    ongletRemoveManager->setLayout(layoutRemoveManager);

    TabM->getOnglets().addTab(ongletWeekView, "Vue hebdomadaire");
    TabM->getOnglets().addTab(ongletTreeView, "Vue synthétique des tâches && projets");
    TabM->getOnglets().addTab(ongletTacheCreator, "Création de tâches");
    TabM->getOnglets().addTab(ongletProjetCreator, "Création de projets");
    TabM->getOnglets().addTab(ongletTacheEditeur, "Edition de tâches");
    TabM->getOnglets().addTab(ongletProjetEditeur, "Edition de projets");
    TabM->getOnglets().addTab(ongletProgCreator, "Programmer un événement");
    TabM->getOnglets().addTab(ongletRemoveManager, "Suppression");
    TabM->getOnglets().addTab(ongletImExManager, "Imports && exports");

    fenetre->show();

    return app.exec();
}
