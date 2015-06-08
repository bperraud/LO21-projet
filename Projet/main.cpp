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

#include "TacheEditeur.h"
#include "ProgCreator.h"
#include "WeekView.h"
#include "TreeView.h"
#include "ImExManager.h"
#include "projetcreator.h"


int main(int argc, char *argv[]){

    QApplication app(argc, argv);
    QWidget* fenetre = new QWidget;
    fenetre->setMinimumSize(QSize(800, 800));

    //try{


    TacheManager& TM = *TacheManager::getInstance();
    ProjetManager& PM = *ProjetManager::getInstance();

    TM.ajouterTacheUnitaire("T1","Etablir une hierarchie coherente",QTime(10, 0),QDate(2015, 2, 28),QDate(2015, 4, 15),true);
    TM.ajouterTacheUnitaire("T2","Repartir les classes entre developpeurs",QTime(1, 30),QDate(2015, 2, 28),QDate(2015, 4, 25),true);

    //QString chemin = QFileDialog::getOpenFileName();
    //QString chemin = "D:/Documents/Dropbox/UTC/HuTech/2014-2015/HU04/UVs/LO21/LO21-projet/Projet/taches.xml";
    /* Utiliser la ligne ci-dessous pour obtenir l'url locale exacte (et ensuite gagner du temps avec la ligne ci-dessus*/
    //QMessageBox::information(&fenetre, "chargement", chemin);

    //TM.load(chemin);

    Tache& T1 = TM.getTache("T1");
    Tache& T2 = TM.getTache("T2");
    TacheUnitaire& T2U = dynamic_cast<TacheUnitaire&>(T2);


    TM.ajouterTacheUnitaire("T3", "Petit test !", QTime(2, 20), QDate(2015, 2, 6), QDate(2015, 6, 4), true);
    ListTaches LT1; LT1 << &T1 << &T2;
    TM.ajouterTacheComposite("T4", "autre test", QDate(2015, 7, 10), QDate(2015, 8, 12), LT1);


    qDebug() << "checkpoint1\n";


    Tache& T4 = TM.getTache("T4");
    TacheComposite& T4C = dynamic_cast<TacheComposite&>(T4);


    T4C.addSousTache(&TM.getTache("T3"));
    T4C.rmSousTache(&T2);

    ListTaches LT; LT << &T2 << &T4;
    TM.ajouterTacheComposite("T5", "autre tacheC", QDate(2016, 2, 15), QDate(2016, 8, 1)).setSousTaches(LT);
    TM.ajouterTacheUnitaire("T6", "tache sans projet", QTime(2, 0), QDate(2015, 4, 3), QDate(2015, 7, 23), true);
    TM.ajouterTacheComposite("T7", "tache C sans projet", QDate(2015, 5, 20), QDate(2015, 7, 28)).addSousTache(&TM.getTache("T6"));


    qDebug() << "checkpoint2\n";

    PM.ajouterProjet("P1", "Projet no1", QDate(2011, 01, 01), QDate(2020, 1, 1));


    PM.getProjet("P1").addTache(&TM.getTache("T5"));
    PM.ajouterProjet("P2", "Projet no2", QDate(2012, 01, 01), QDate(2025, 1, 1));

    qDebug() << "checkpoint3\n";

    TacheUnitaire& T6U = dynamic_cast<TacheUnitaire&>(TM.getTache("T6"));

    ProgManager* ProgM = ProgManager::getInstance();
    ProgM->ajouterProgrammationT(QDate(2015, 5, 26), QTime(12, 0), QTime(13, 30), T2U);
    ProgM->ajouterProgrammationT(QDate(2015, 5, 28), QTime(14, 30), QTime(16, 30), T6U);
    ProgM->ajouterProgrammationA(QDate(2015, 6, 2), QTime(11, 0), QTime(12, 45), "Activité comme ça", "On peut faire plein de choses...", "À l'UTC pardi");
    ProgM->ajouterProgrammationA(QDate(2015, 6, 3), QTime(14, 10), QTime(14, 50), "Une autre activité", "Ce que Dieu veut", "Chez nous");


    // Test du Design pattern Visitor pour connaître dynamiquement le type réel porté par les Tache*
    TacheInformateur informateur;
    for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i)
        (*i).accept(&informateur);




    Tache& T3U = T2;
    PrecedenceManager& PrM = *PrecedenceManager::getInstance();
    PrM.ajouterPrecedence(T1, T2U);
    //PrM.supprimerPrecedence(T1, T2U);



    WeekView* WV = new WeekView;
    TreeView* TV = new TreeView;
    TacheEditeur* TE = new TacheEditeur;
    ProgCreator* PC = new ProgCreator;
    ImExManager* IEM = new ImExManager;
    TacheCreator* TCR = new TacheCreator;
    //ProjetCreator* ProjC = new ProjetCreator;
    //ProjetEditeur* ProjE = new ProjetEditeur;

    QVBoxLayout *layoutWeekView, *layoutTacheEditeur, *layoutTreeView, *layoutProgCreator, *layoutImExManager, *layoutTacheCreator, *layoutProjetCreator, *layoutProjetEditeur;
    layoutWeekView = new QVBoxLayout;
    layoutTacheEditeur = new QVBoxLayout;
    layoutTreeView = new QVBoxLayout;
    layoutProgCreator = new QVBoxLayout;
    layoutImExManager = new QVBoxLayout;
    layoutTacheCreator = new QVBoxLayout;
    //layoutProjetCreator = new QVBoxLayout;
    //layoutProjetEditeur = new QVBoxLayout;

    //QScrollArea* scrollareaTE = new QScrollArea;

    layoutWeekView->addWidget(WV);
    layoutTreeView->addWidget(TV);
    //layoutTacheEditeur->addWidget(&scrollareaTE);
    layoutTacheEditeur->addWidget(TE);
    layoutProgCreator->addWidget(PC);
    layoutImExManager->addWidget(IEM);
    layoutTacheCreator->addWidget(TCR);
    //layoutProjetCreator->addWidget(ProjC);
    //layoutProjetEditeur->addWidget(ProjE);

    //scrollareaTE->setWidget(&TE);

    TabManager* TabM = new TabManager(fenetre);
    QWidget *ongletWeekView, *ongletTacheEditeur, *ongletTreeView, *ongletProgCreator, *ongletImExManager, *ongletTacheCreator, *ongletProjetCreator, *ongletProjetEditeur;
    ongletWeekView = new QWidget;
    ongletTacheEditeur = new QWidget;
    ongletTreeView = new QWidget;
    ongletProgCreator = new QWidget;
    ongletImExManager = new QWidget;
    ongletTacheCreator = new QWidget;
    //ongletProjetCreator = new QWidget;
    //ongletProjetEditeur = new QWidget;

    ongletWeekView->setLayout(layoutWeekView);
    ongletTreeView->setLayout(layoutTreeView);
    ongletTacheEditeur->setLayout(layoutTacheEditeur);
    ongletProgCreator->setLayout(layoutProgCreator);
    ongletImExManager->setLayout(layoutImExManager);
    ongletTacheCreator->setLayout(layoutTacheCreator);
    //ongletProjetCreator->setLayout(layoutProjetCreator);
    //ongletProjetEditeur->setLayout(layoutProjetEditeur);

    TabM->getOnglets().addTab(ongletWeekView, "Vue hebdomadaire");
    TabM->getOnglets().addTab(ongletTreeView, "Vue synthétique des tâches && projets");
    TabM->getOnglets().addTab(ongletTacheEditeur, "Edition de tâches");
    TabM->getOnglets().addTab(ongletTreeView, "Vue synthétique");
    TabM->getOnglets().addTab(ongletTacheEditeur, "Onglet Tache Editeur");
    TabM->getOnglets().addTab(ongletProgCreator, "Programmer un événement");
    TabM->getOnglets().addTab(ongletImExManager, "Imports && exports");
    TabM->getOnglets().addTab(ongletTacheCreator, "Création de tâches");
    //TabM->getOnglets().addTab(ongletProjetCreator, "Création de projets");
    //TabM->getOnglets().addTab(ongletProjetEditeur, "Edition de projets");



    //}catch(CalendarException e){qDebug() << e.getInfo() << "\n";}

    for (TacheManager::tabParentIterator it = TM.tabParentBegin(); it != TM.tabParentEnd(); ++it)
        qDebug() << (*it).key()->getTitre() << "," << (*it).value()->getTitre();

    fenetre->show();

    return app.exec();
}
