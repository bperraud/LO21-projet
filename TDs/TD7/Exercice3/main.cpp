#include <QApplication>
#include <QFileDialog>
#include <QComboBox>

#include <QDebug>

#include "Calendar.h"
#include "TacheEditeur.h"

int main(int argc, char *argv[]){

    QApplication app(argc, argv);

    TacheManager& TM = TacheManager::getInstance();
    QString chemin = QFileDialog::getOpenFileName();
    TM.load(chemin);
    Tache& T2 = TM.getTache("T2");
    TacheEditeur fenetre(T2);
    fenetre.show();

    return app.exec();
}
