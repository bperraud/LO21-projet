#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QString>
#include <QFileDialog>
#include <QDebug>

#include "Calendar.h"

int main(int argc, char *argv[]){

    QApplication app(argc, argv);

    QWidget fenetre;
    fenetre.setFixedSize(200, 200);

    QLineEdit identificateur;
    identificateur.setFixedWidth(180);

    QTextEdit titre;
    titre.setFixedSize(180, 110);

    QPushButton button("Sauver");
    button.setFixedWidth(80);

    QVBoxLayout layout;

    layout.addWidget(&identificateur);
    layout.addWidget(&titre);
    layout.addWidget(&button);
    fenetre.setLayout(&layout);

    try{
        TacheManager& TM = TacheManager::getInstance();
        QString chemin = QFileDialog::getOpenFileName();
        TM.load(chemin);
        Tache& T1 = TM.getTache("T1");
        identificateur.setText(T1.getId());
        titre.setText(T1.getTitre());
    }
    catch(CalendarException& e){
        qDebug() << e.getInfo();
    }
    catch(...){
        qDebug() << "Erreur inconnue";
    }

    fenetre.show();

    return app.exec();
}
