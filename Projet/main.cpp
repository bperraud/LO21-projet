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


#include "Calendar.h"
#include "TacheEditeur.h"

int main(int argc, char *argv[]){

    QApplication app(argc, argv);
    QWidget fenetre;
    fenetre.setFixedSize(1024, 768);

    QMenuBar menubar(&fenetre);
    QMenu menu1("menu 1");
    menubar.addMenu(&menu1);


    TacheManager& TM = TacheManager::getInstance();
    QString chemin = QFileDialog::getOpenFileName();
    //QMessageBox::information(&fenetre, "chargement", chemin);
    TM.load(chemin);
    Tache& T2 = TM.getTache("T2");
    TacheEditeur TE(T2);

    QPushButton buttonChargerTache("Charger tâche");
    buttonChargerTache.setFixedWidth(128);
    //QObject::connect(&buttonChargerTache, SIGNAL(clicked()),

    QTabWidget OngletsManager(&fenetre);
    OngletsManager.setGeometry(32, 32, 800, 600);
    QWidget onglet1, onglet2;
    QLabel hello("Hello world. Bon j'imagine que le premier onglet pourrait être cette fameuse vue hebdomadaire synthétique.\n"
                 "Par rapport aux recherches que j'ai faites, la meilleure piste qui s'offre à nous c'est d'utiliser un tableau\n"
                 "avec soit QTableView ou QTableWidget (à voir la différence entre les deux).");
    QVBoxLayout layout1, layout2;

    layout1.addWidget(&hello);

    layout2.addWidget(&buttonChargerTache);
    layout2.addWidget(&TE);
    onglet1.setLayout(&layout1);
    onglet2.setLayout(&layout2);
    OngletsManager.addTab(&onglet1, "Onglet no 1");
    OngletsManager.addTab(&onglet2, "Onglet Tache Editeur");




    fenetre.show();

    return app.exec();
}
