#include <QMessageBox>
#include <QFileDialog>

#include "ProjetCreator.h"
#include "TacheManager.h"
#include "PrecedenceManager.h"

ProjetCreator::ProjetCreator(QWidget *parent) : QWidget(parent) {

    titreLabel = new QLabel("titre", this);
    titre = new QLineEdit(this);

    descriptionLabel = new QLabel("description", this);
    description = new QTextEdit(this);

    dispoLabel = new QLabel("disponibilité", this);
    dispo = new QDateEdit(QDate::currentDate(), this);

    echeanceLabel = new QLabel("échéance", this);
    echeance = new QDateEdit(QDate::currentDate(), this);

    tachesLabel = new QLabel("Taches:", this);
    taches = new QListWidget(this);
    TacheManager& TM = *TacheManager::getInstance();
    ProjetManager& ProjM = *ProjetManager::getInstance();
    for (Iterator<TacheManager, Tache> i = TM.begin(); i != TM.end(); ++i){
        QString UneTache = (*i).getTitre();
        if (!ProjM.isTacheInProjet(*i)){
            QListWidgetItem* item = new QListWidgetItem(UneTache, taches);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
            item->setCheckState(Qt::Unchecked);// AND initialize check state
        }
    };

    creer = new QPushButton("Créer", this);

    HC1 = new QHBoxLayout;
        HC1->addWidget(titreLabel);
        HC1->addWidget(titre);
    HC2 = new QHBoxLayout;
        HC2->addWidget(descriptionLabel);
        HC2->addWidget(description);
    HC3 = new QHBoxLayout;
        HC3->addWidget(dispoLabel);
        HC3->addWidget(dispo);
        HC3->addWidget(echeanceLabel);
        HC3->addWidget(echeance);
    HC4 = new QHBoxLayout;
        HC4->addWidget(tachesLabel);
        HC4->addWidget(taches);
    HC5 = new QHBoxLayout;
        HC5->addWidget(creer);


    VC = new QVBoxLayout;
        VC->addLayout(HC1);
        VC->addLayout(HC2);
        VC->addLayout(HC3);
        VC->addLayout(HC4);
        VC->addLayout(HC5);
        VC->addStretch();

    QObject::connect(creer, SIGNAL(clicked()), this, SLOT(creerProjet()));

    this->setLayout(VC);

}

void ProjetCreator::updatePC() {
    TacheManager& TM = *TacheManager::getInstance();
    ProjetManager& ProjM = *ProjetManager::getInstance();
    taches->clear();
    for (Iterator<TacheManager, Tache> i = TM.begin(); i != TM.end(); ++i){
        QString UneTache = (*i).getTitre();
        if (!ProjM.isTacheInProjet(*i)){
            QListWidgetItem* item = new QListWidgetItem(UneTache, taches);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
            item->setCheckState(Qt::Unchecked);// AND initialize check state
        }
    }
}

void ProjetCreator::creerProjet(){

    if (titre->text() == "")
        QMessageBox::warning(this, "Création impossible", "Pas de titre renseigné, impossible de créer le projet.");
    if (description->toPlainText() == "")
        QMessageBox::warning(this, "Création impossible", "Pas de description renseignée, impossible de créer le projet.");

    ProjetManager& ProjM = *ProjetManager::getInstance();
    // Si le titre de la tâche en cours de création existe déjà
    if (ProjM.isProjetExistant(titre->text())){
        QMessageBox::warning(this, "Création impossible", "titre projet déjà existant...");
        return;
    }
    // Si les dates de dispo et d'échéance sont incohérentes
    if (dispo->date() >= echeance->date()){
            QMessageBox::warning(this, "Sauvegarde impossible", "Incohérence de dates...");
            return;
    }

    ListTaches T;
    TacheManager& TM = *TacheManager::getInstance();
    for (int i = 0; i < taches->count(); ++i) {
        QListWidgetItem *tache = taches->item(i);
        if (tache->checkState() == Qt::Checked)
            T << &(TM.getTache(tache->text()));
        };
    ProjM.ajouterProjet(titre->text(), description->toPlainText(), dispo->date(), echeance->date(), T);

    QMessageBox::information(this, "Création", "Projet créé.");

}

// --- [END] ProjetCreator ---
