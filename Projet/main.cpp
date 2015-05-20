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
#include <QAbstractScrollArea>

#include <QStandardItemModel>
#include <QStandardItem>
#include <QVariant>
#include <QTreeView>
#include <QTableView>

#include "Calendar.h"
#include "TacheManager.h"
#include "TacheEditeur.h"


int main(int argc, char *argv[]){

    QApplication app(argc, argv);
    QWidget fenetre;
    //fenetre.setFixedSize(1024, 768);
    fenetre.setMinimumWidth(800);
    fenetre.setMinimumHeight(768);

    /*QMenuBar menubar(&fenetre);
    QMenu menu1("menu 1");
    menubar.addMenu(&menu1);*/



    TacheManager& TM = *TacheManager::getInstance();
    //QString chemin = QFileDialog::getOpenFileName();
    QString chemin = "D:/Documents/Dropbox/UTC/HuTech/2014-2015/HU04/UVs/LO21/LO21-projet/Projet/taches.xml";
    /* Utiliser la ligne ci-dessus pour obtenir l'url locale exacte (et ensuite gagner du temps avec la ligne ci-dessus*/
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


    QWidget onglet1, onglet2;
    QLabel hello("Hello world. Bon j'imagine que le premier onglet pourrait être cette fameuse vue hebdomadaire synthétique.\n"
                 "Par rapport aux recherches que j'ai faites, la meilleure piste qui s'offre à nous c'est d'utiliser un tableau\n"
                 "avec soit QTableView ou QTableWidget (à voir la différence entre les deux).\n"
                 "Apparemment, on peut partir sur QTableView qui permet la fusion de cellules, utile dans notre cas\n"
                 "lorsque des événements couvrent plusieures heures. Je pense aussi que c'est plus simple qu'on\n"
                 "restreigne les durées aux demi-heures (ou quarts d'heure).");

    //TM.ajouterTacheUnitaire("T3", "Petit test !", Duree(2, 20), QDate(2015, 2, 6), QDate(2015, 6, 4), true);
    //ListTaches LT; LT << &T1 << &T2;
    //TM.ajouterTacheComposite("T4", "autre test", QDate(2015, 7, 10), QDate(2015, 8, 12), LT);

    Tache& T4 = TM.getTache("T4");
    TacheComposite& T4C = dynamic_cast<TacheComposite&>(T4);
    T4C.addSousTache(&TM.getTache("T3"));
    T4C.rmSousTache(&T2);
    ListTaches LT; LT << &T2 << &T4;
    TM.ajouterTacheComposite("T5", "autre tacheC", QDate(2016, 2, 15), QDate(2016, 8, 1)).setSousTaches(LT);

    //dynamic_cast<TacheComposite&>(TM.ajouterTacheComposite("T5", "autre tacheC", QDate(2016, 2, 15), QDate(2016, 8, 1), ListTaches())).setSousTaches(LT);

    //qDebug() << QString((typeid(T1) == typeid(TacheUnitaire)) ? "tache unitaire" : "tache composite") << "\n";

    ProgTacheManager* PTM = ProgTacheManager::getInstance();
    PTM->ajouterProgrammation(QDate(2016, 2, 15), QTime(2, 0), T2U);

    QLabel labelLDT;
    QString listeDeTaches = "Liste des tâches :\n\n";
    for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i)
        (*i).ajouterInfos(listeDeTaches);
    labelLDT.setText(listeDeTaches);

    // Test du Design pattern Visitor pour connaître dynamiquement le type réel porté par les Tache*
    TacheInformateur informateur;
    for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i)
        (*i).accept(&informateur);

    QTreeView* treeView = new QTreeView;
    treeView->setHeaderHidden(true);
    QStandardItemModel* TacheModel = new QStandardItemModel;
    //TacheModel->setHorizontalHeaderLabels (QStringList(QString("toto")));
    QStandardItem *rootNode = TacheModel->invisibleRootItem();

    //defining a couple of items
    QStandardItem *americaItem = new QStandardItem(QString("America"));
    QStandardItem *mexicoItem =  new QStandardItem("Canada");
    QStandardItem *usaItem =     new QStandardItem("USA");
    QStandardItem *bostonItem =  new QStandardItem("Boston");
    QStandardItem *europeItem =  new QStandardItem("Europe");
    QStandardItem *italyItem =   new QStandardItem("Italy");
    QStandardItem *romeItem =    new QStandardItem("Rome");
    QStandardItem *veronaItem =  new QStandardItem("Verona");

    QList<QStandardItem*> ItemList;
    for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i){
        QStandardItem* tache = new QStandardItem((*i).getTitre());
        //tache->setData(QVariant::fromValue(5));
        ItemList.append(tache);
    }

    // Pour empêcher l'édition des lignes
    for (int i = 0; i < ItemList.size(); ++i)
        ItemList[i]->setFlags(ItemList[i]->flags() & ~Qt::ItemIsEditable);

    //building up the hierarchy

    /*void ajouterFils(QStandardItem* pere){

    }*/

    rootNode->appendRows(ItemList);

    rootNode->    appendRow(americaItem);
    rootNode->    appendRow(europeItem);
    americaItem-> appendRow(mexicoItem);
    americaItem-> appendRow(usaItem);
    usaItem->     appendRow(bostonItem);
    europeItem->  appendRow(italyItem);
    italyItem->   appendRow(romeItem);
    italyItem->   appendRow(veronaItem);

    //register the model
    treeView->setModel(TacheModel);
    //treeView->expandAll();


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


    tableView->setModel(WeekModel);


    QVBoxLayout layout1, layout2;

    layout1.addWidget(&hello);
    layout1.addWidget(treeView);
    layout1.addWidget(tableView);
    layout2.addWidget(&buttonChargerTache);
    layout2.addWidget(&labelLDT);
    layout2.addWidget(&TE);
    onglet1.setLayout(&layout1);
    onglet2.setLayout(&layout2);
    OngletsManager.addTab(&onglet1, "Onglet no 1");
    OngletsManager.addTab(&onglet2, "Onglet Tache Editeur");



    fenetre.show();

    return app.exec();
}
