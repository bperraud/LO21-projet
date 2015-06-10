#include <QMessageBox>
#include <QFileDialog>

#include "RemoveManager.h"
#include "TacheManager.h"
#include "PrecedenceManager.h"

RemoveManager::RemoveManager(QWidget *parent) : QWidget(parent) {

    ChoixProjetLabel = new QLabel("Projet à supprimer :", this);
    ChoixProjet = new QComboBox(this);
        ProjetManager& ProjM = *ProjetManager::getInstance();
        for (ProjetManager::iterator i = ProjM.begin(); i != ProjM.end(); ++i){
            QString UnProjet = (*i).getTitre();
            ChoixProjet->addItem(UnProjet);
        }

    supprimerP = new QPushButton("Supprimer", this);
    supprimerP->setFixedWidth(96);

    ChoixTacheLabel = new QLabel("Tâche à Supprimer :", this);
    ChoixTache = new QComboBox(this);
         TacheManager& TM = *TacheManager::getInstance();
         for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i){
            QString UneTache = (*i).getTitre();
            ChoixTache->addItem(UneTache);
         }

    supprimerT = new QPushButton("Supprimer", this);
    supprimerT->setFixedWidth(96);

    HC1 = new QHBoxLayout;
        HC1->addWidget(ChoixProjetLabel);
        HC1->addWidget(ChoixProjet);
        HC1->addWidget(supprimerP);
    HC2 = new QHBoxLayout;
        HC2->addWidget(ChoixTacheLabel);
        HC2->addWidget(ChoixTache);
        HC2->addWidget(supprimerT);
    VC = new QVBoxLayout;
        VC->addLayout(HC1);
        VC->addLayout(HC2);
        VC->addStretch();

    QObject::connect(supprimerP, SIGNAL(clicked(bool)), this, SLOT(supprimerProjet()));
    QObject::connect(supprimerT, SIGNAL(clicked(bool)), this, SLOT(supprimerTache()));

    this->setLayout(VC);

}

void RemoveManager::updateRm() {

    ChoixProjet->clear();
    ProjetManager& ProjM = *ProjetManager::getInstance();
    for (ProjetManager::iterator i = ProjM.begin(); i != ProjM.end(); ++i){
        QString UnProjet = (*i).getTitre();
        ChoixProjet->addItem(UnProjet);
    }

    ChoixTache->clear();
    TacheManager& TM = *TacheManager::getInstance();
    for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i){
       QString UneTache = (*i).getTitre();
       ChoixTache->addItem(UneTache);
    }

}

void RemoveManager::supprimerProjet() {

    ProjetManager& ProjM = *ProjetManager::getInstance();
    if (!ProjM.isProjetExistant(ChoixProjet->currentText()))
        QMessageBox::warning(this, "Suppression impossible", "Pas de projet portant ce titre");
    else {
        ProjM.deleteProjet(ChoixProjet->currentText());
        updateRm();
        QMessageBox::information(this, "Suppression projet", "Projet supprimé avec succès");
    }
}

void RemoveManager::supprimerTache() {

    TacheManager& TM = *TacheManager::getInstance();
    if (!TM.isTacheExistante(ChoixTache->currentText()))
        QMessageBox::warning(this, "Suppression impossible", "Pas de tâche portant ce titre");
    else {
        TM.deleteTache(ChoixTache->currentText());
        updateRm();
        QMessageBox::information(this, "Suppression tâche", "Tâche supprimée avec succès");
    }

}
