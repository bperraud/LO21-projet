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

#include <QStandardItemModel>
#include <QStandardItem>
#include <QTableView>

#include "Calendar.h"
#include "TacheManager.h"
#include "TacheEditeur.h"
#include "TreeView.h"







int main(int argc, char *argv[]){

    QApplication app(argc, argv);
    QWidget fenetre;
    //fenetre.setFixedSize(1024, 768);
    fenetre.setMinimumWidth(800);
    fenetre.setMinimumHeight(768);

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
    TacheEditeur TE(T2U);



    QPushButton buttonChargerTache("Charger tâche");
    buttonChargerTache.setFixedWidth(128);
    //QObject::connect(&buttonChargerTache, SIGNAL(clicked()),

    QTabWidget OngletsManager(&fenetre);
    //OngletsManager.setGeometry(32, 32, 800, 600);
    OngletsManager.setMinimumWidth(800);




    QWidget onglet1, onglet2, ongletTreeView;
    QLabel hello("Hello world. Bon j'imagine que le premier onglet pourrait être cette fameuse vue hebdomadaire synthétique.\n"
                 "Par rapport aux recherches que j'ai faites, la meilleure piste qui s'offre à nous c'est d'utiliser un tableau\n"
                 "avec soit QTableView ou QTableWidget (à voir la différence entre les deux).\n"
                 "Apparemment, on peut partir sur QTableView qui permet la fusion de cellules, utile dans notre cas\n"
                 "lorsque des événements couvrent plusieures heures. Je pense aussi que c'est plus simple qu'on\n"
                 "restreigne les durées aux demi-heures (ou quarts d'heure).");

    //TM.ajouterTacheUnitaire("T3", "Petit test !", Duree(2, 20), QDate(2015, 2, 6), QDate(2015, 6, 4), true);
    ListTaches LT1; LT1 << &T1 << &T2;
    //TM.ajouterTacheComposite("T4", "autre test", QDate(2015, 7, 10), QDate(2015, 8, 12), LT1);


    //qDebug() << "checkpoint1\n";


    Tache& T4 = TM.getTache("T4");
    TacheComposite& T4C = dynamic_cast<TacheComposite&>(T4);


    T4C.addSousTache(&TM.getTache("T3"));


    T4C.rmSousTache(&T2);
    ListTaches LT; LT << &T2 << &T4;
    TM.ajouterTacheComposite("T5", "autre tacheC", QDate(2016, 2, 15), QDate(2016, 8, 1)).setSousTaches(LT);

    TM.ajouterTacheUnitaire("T6", "tache sans projet", Duree(2, 0), QDate(2015, 4, 3), QDate(2015, 7, 23));

    PM.ajouterProjet("P1", "Projet no1", QDate(2011, 01, 01), QDate(2020, 1, 1));
    PM.getProjet("P1").addTache(&TM.getTache("T5"));
    PM.ajouterProjet("P2", "Projet no2", QDate(2012, 01, 01), QDate(2025, 1, 1));
    //dynamic_cast<TacheComposite&>(TM.ajouterTacheComposite("T5", "autre tacheC", QDate(2016, 2, 15), QDate(2016, 8, 1), ListTaches())).setSousTaches(LT);

    //qDebug() << QString((typeid(T1) == typeid(TacheUnitaire)) ? "tache unitaire" : "tache composite") << "\n";

    ProgTacheManager* PTM = ProgTacheManager::getInstance();
    PTM->ajouterProgrammation(QDate(2016, 2, 15), QTime(2, 0), T2U);


    // Test du Design pattern Visitor pour connaître dynamiquement le type réel porté par les Tache*
    TacheInformateur informateur;
    for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i)
        (*i).accept(&informateur);





    //qDebug() << "checkpoint2\n";

    QTableView* tableView = new QTableView;
    QStringList ListJours, ListHeures;
    ListJours << "Lundi" << "Mardi" << "Mercredi" << "Jeudi" << "Vendredi" << "Samedi" << "Dimanche";
    for (unsigned int i = 0; i<24 ; ++i){
        int heure = (i % 2 == 0) ? i/2 : i/2;
        QString demiheure = (i % 2 == 0) ? "00" : "30";
        ListHeures << QString("%0h%1").arg(heure+8).arg(demiheure);
    }
    QStandardItemModel* WeekModel = new QStandardItemModel(10, 7);
    for (int jour = 0; jour < 7; ++jour){
        WeekModel->setHorizontalHeaderLabels(ListJours);
        for (int heure = 0; heure < 24; ++heure){
            WeekModel->setVerticalHeaderLabels(ListHeures);
            QStandardItem *item = new QStandardItem("blabla");
            WeekModel->setItem(heure, jour, item);
            WeekModel->item(heure, jour)->setFlags(WeekModel->item(heure, jour)->flags() & ~Qt::ItemIsEditable);
        }
    }

    tableView->setSpan(0, 0, 2, 1); //sets the 1st row 1st column cell to span over 2 rows and 1 column

    //qDebug() << "checkpoint3\n";
    tableView->setModel(WeekModel);



    TreeView TV;



    QVBoxLayout layout1, layout2, layoutTreeView;
    QScrollArea scrollareaTE;

    layout1.addWidget(&hello);
    layout1.addWidget(tableView);

    layoutTreeView.addWidget(&TV);

    layout2.addWidget(&buttonChargerTache);
    layout2.addWidget(&scrollareaTE);

    scrollareaTE.setWidget(&TE);

    onglet1.setLayout(&layout1);
    ongletTreeView.setLayout(&layoutTreeView);
    onglet2.setLayout(&layout2);

    OngletsManager.addTab(&onglet1, "Onglet no 1");
    OngletsManager.addTab(&ongletTreeView, "Vue synthétique des tâches & projets");
    OngletsManager.addTab(&onglet2, "Onglet Tache Editeur");

    //}catch(CalendarException e){qDebug() << e.getInfo() << "\n";}




    fenetre.show();

    return app.exec();
}
