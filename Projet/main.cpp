#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>
#include <QFileDialog>
#include <QTabWidget>
#include <QMenuBar>
#include <QMenu>
#include <QScrollArea>

#include "Calendar.h"
#include "TacheManager.h"
#include "TacheEditeur.h"
#include "ProgCreator.h"
#include "WeekView.h"
#include "TreeView.h"


int main(int argc, char *argv[]){

    QApplication app(argc, argv);
    QWidget fenetre;
    //fenetre.setFixedSize(1024, 768);
    fenetre.setMinimumWidth(800);
    fenetre.setMinimumHeight(800);

    //try{



    /*QMenuBar menubar(&fenetre);
    QMenu menu1("menu 1");
    menubar.addMenu(&menu1);*/

    TacheManager& TM = *TacheManager::getInstance();
    ProjetManager& PM = *ProjetManager::getInstance();

    //TM.ajouterTacheUnitaire("T1","Etablir une hierarchie coherente",Duree(10, 0),QDate(2015, 2, 28),QDate(2015, 4, 15),true);
    //TM.ajouterTacheUnitaire("T2","Repartir les classes entre developpeurs",Duree(1, 30),QDate(2015, 2, 28),QDate(2015, 4, 25),true);

    //QString chemin = QFileDialog::getOpenFileName();
    QString chemin = "D:/Documents/Dropbox/UTC/HuTech/2014-2015/HU04/UVs/LO21/LO21-projet/Projet/taches.xml";
    /* Utiliser la ligne ci-dessous pour obtenir l'url locale exacte (et ensuite gagner du temps avec la ligne ci-dessus*/
    //QMessageBox::information(&fenetre, "chargement", chemin);

    TM.load(chemin);

    Tache& T1 = TM.getTache("T1");
    Tache& T2 = TM.getTache("T2");
    TacheUnitaire& T2U = dynamic_cast<TacheUnitaire&>(T2);


    QPushButton buttonChargerTache("Charger tâche");
    buttonChargerTache.setFixedWidth(128);
    //QObject::connect(&buttonChargerTache, SIGNAL(clicked()),

    QTabWidget Onglets(&fenetre);
    //OngletsManager.setGeometry(32, 32, 800, 600);
    Onglets.setMinimumWidth(800);




    QWidget ongletWeekView, onglet2, ongletTreeView, ongletProgCreator;


    //TM.ajouterTacheUnitaire("T3", "Petit test !", Duree(2, 20), QDate(2015, 2, 6), QDate(2015, 6, 4), true);
    //ListTaches LT1; LT1 << &T1 << &T2;
    //TM.ajouterTacheComposite("T4", "autre test", QDate(2015, 7, 10), QDate(2015, 8, 12), LT1);


    //qDebug() << "checkpoint1\n";


    Tache& T4 = TM.getTache("T4");
    TacheComposite& T4C = dynamic_cast<TacheComposite&>(T4);


    //T4C.addSousTache(&TM.getTache("T3"));


    //T4C.rmSousTache(&T2);
    //ListTaches LT; LT << &T2 << &T4;
    //TM.ajouterTacheComposite("T5", "autre tacheC", QDate(2016, 2, 15), QDate(2016, 8, 1)).setSousTaches(LT);

    //TM.ajouterTacheUnitaire("T6", "tache sans projet", Duree(2, 0), QDate(2015, 4, 3), QDate(2015, 7, 23));
    //qDebug() << "checkpoint2\n";

    PM.ajouterProjet("P1", "Projet no1", QDate(2011, 01, 01), QDate(2020, 1, 1));
    PM.getProjet("P1").addTache(&TM.getTache("T5"));
    PM.ajouterProjet("P2", "Projet no2", QDate(2012, 01, 01), QDate(2025, 1, 1));
    //dynamic_cast<TacheComposite&>(TM.ajouterTacheComposite("T5", "autre tacheC", QDate(2016, 2, 15), QDate(2016, 8, 1), ListTaches())).setSousTaches(LT);

    //qDebug() << QString((typeid(T1) == typeid(TacheUnitaire)) ? "tache unitaire" : "tache composite") << "\n";

    TacheUnitaire& T6U = dynamic_cast<TacheUnitaire&>(TM.getTache("T6"));

    ProgManager* ProgM = ProgManager::getInstance();
    ProgM->ajouterProgrammationT(QDate(2015, 5, 26), QTime(12, 0), QTime(13, 30), T2U);
    ProgM->ajouterProgrammationT(QDate(2015, 5, 28), QTime(14, 30), QTime(16, 30), T6U);
    ProgM->ajouterProgrammationA(QDate(2015, 6, 2), QTime(11, 15), QTime(12, 45), "Activité comme ça", "On peut faire plein de choses...", "À l'UTC pardi");



    // Test du Design pattern Visitor pour connaître dynamiquement le type réel porté par les Tache*
    TacheInformateur informateur;
    for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i)
        (*i).accept(&informateur);





    /*ExampleModel em;
    QTableView tv;
    HierarchicalHeaderView* hv=new HierarchicalHeaderView(Qt::Vertical, &tv);
    tv.setVerticalHeader(hv);
    tv.setModel(&em);
    tv.resizeColumnsToContents();
    tv.resizeRowsToContents();*/





    //qDebug() << "checkpoint3\n";

    Tache& T3U = T2;
    PrecedenceManager& PrM = *PrecedenceManager::getInstance();
    PrM.ajouterPrecedence(T1, T2U);
    PrM.supprimerPrecedence(T1, T2U);
    TacheEditeur TE;


    WeekView WV(QDate(2015, 5, 25));
    TreeView TV;
    ProgCreator PC;



    QVBoxLayout layoutWeekView, layout2, layoutTreeView, layoutProgCreator;
    QScrollArea scrollareaTE;

    layoutWeekView.addWidget(&WV);

    layoutTreeView.addWidget(&TV);

    layout2.addWidget(&buttonChargerTache);
    layout2.addWidget(&scrollareaTE);

    layoutProgCreator.addWidget(&PC);

    scrollareaTE.setWidget(&TE);

    ongletWeekView.setLayout(&layoutWeekView);
    ongletTreeView.setLayout(&layoutTreeView);
    onglet2.setLayout(&layout2);
    ongletProgCreator.setLayout(&layoutProgCreator);

    Onglets.addTab(&ongletWeekView, "Vue hebdomadaire");
    Onglets.addTab(&ongletTreeView, "Vue synthétique des tâches && projets");
    Onglets.addTab(&onglet2, "Onglet Tache Editeur");
    Onglets.addTab(&ongletProgCreator, "Programmer un événement");

    //}catch(CalendarException e){qDebug() << e.getInfo() << "\n";}




    fenetre.show();

    return app.exec();
}
