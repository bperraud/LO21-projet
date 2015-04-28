#include <QObject>
#include <QMessageBox>
#include <QFileDialog>

#include "TacheEditeur.h"
#include "Calendar.h"

TacheEditeur::TacheEditeur(Tache& tacheToEdit, QWidget* parent) : QWidget(parent), tache(tacheToEdit){

    //this->setWindowTitle(QString("Edition de la tâche ")+tache.getId());
    this->setFixedSize(600, 400);
    //this->setEnabled(false);

    identificateurLabel = new QLabel("identificateur", this);
    identificateur = new QLineEdit(tache.getId(), this);

    preemptive = new QCheckBox(this);
    preemptive->setChecked(tache.isPreemptive());
    preemptiveLabel = new QLabel("preemptive", this);

    titreLabel = new QLabel("titre", this);
    titre = new QTextEdit(tache.getTitre(), this);

    dispoLabel = new QLabel("disponibilité", this);
    dispo = new QDateEdit(tache.getDateDisponibilite(), this);

    echeanceLabel = new QLabel("échéance", this);
    echeance = new QDateEdit(tache.getDateEcheance(), this);

    dureeLabel = new QLabel("durée", this);
    dureeH = new QSpinBox(this);
    dureeH->setMinimum(0);
    dureeH->setSuffix(" heure(s)");
    dureeH->setValue(tache.getDuree().getHeure());
    dureeM = new QSpinBox(this);
    dureeM->setRange(0, 59);
    dureeM->setSuffix(" minute(s)");
    dureeM->setValue(tache.getDuree().getMinute());

    annuler = new QPushButton("Annuler", this);
    sauver = new QPushButton("Sauver", this);
    sauver->setEnabled(false);

    // Gestion des Layouts

    HC1 = new QHBoxLayout;
        HC1->addWidget(identificateurLabel);
        HC1->addWidget(identificateur);
        HC1->addWidget(preemptive);
        HC1->addWidget(preemptiveLabel);
    HC2 = new QHBoxLayout;
        HC2->addWidget(titreLabel);
        HC2->addWidget(titre);
    HC3 = new QHBoxLayout;
        HC3->addWidget(dispoLabel);
        HC3->addWidget(dispo);
        HC3->addWidget(echeanceLabel);
        HC3->addWidget(echeance);
        HC3->addWidget(dureeLabel);
        HC3->addWidget(dureeH);
        HC3->addWidget(dureeM);
    HC4 = new QHBoxLayout;
        HC4->addWidget(annuler);
        HC4->addWidget(sauver);

    VC = new QVBoxLayout;
        VC->addLayout(HC1);
        VC->addLayout(HC2);
        VC->addLayout(HC3);
        VC->addLayout(HC4);

    this->setLayout(VC);

    // Gestion des signaux

        // Activation du bouton Sauver
        QObject::connect(identificateur, SIGNAL(textEdited(const QString&)), this, SLOT(activerSauver()));
        QObject::connect(preemptive, SIGNAL(stateChanged(int)), this, SLOT(activerSauver()));
        QObject::connect(titre, SIGNAL(textChanged()), this, SLOT(activerSauver()));
        QObject::connect(dispo, SIGNAL(dateChanged(const QDate&)), this, SLOT(activerSauver()));
        QObject::connect(echeance, SIGNAL(dateChanged(const QDate&)), this, SLOT(activerSauver()));
        QObject::connect(dureeH, SIGNAL(valueChanged(int)), this, SLOT(activerSauver()));
        QObject::connect(dureeM, SIGNAL(valueChanged(int)), this, SLOT(activerSauver()));

    QObject::connect(annuler, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(sauver, SIGNAL(clicked()), this, SLOT(sauverTache()));
}


void TacheEditeur::sauverTache(){
    TacheManager& TM = TacheManager::getInstance();
    // Si l'id de la tâche en cours d'édition existe déjà et que ce n'est pas celui de la tâche chargée
    if (TM.isTacheExistante(identificateur->text())
        && &(TM.getTache(identificateur->text())) != &tache){
        QMessageBox::warning(this, "Sauvegarde impossible", "Identificateur tâche déjà existant...");
        return;
    }
    // Si les dates de dispo et d'échéance sont incohérentes
    if (dispo->date() >= echeance->date()){
            QMessageBox::warning(this, "Sauvegarde impossible", "Incohérence de dates...");
            return;
    }
    if (tache.getId() != identificateur->text()) tache.setId(identificateur->text());
    preemptive->isChecked() ? tache.setPreemptive(true) : tache.setPreemptive(false);
    tache.setTitre(titre->toPlainText());
    tache.setDatesDisponibiliteEcheance(dispo->date(), echeance->date());
    tache.setDuree(Duree(dureeH->value(), dureeM->value()));
    QString cheminSauver = QFileDialog::getSaveFileName();
    TM.save(cheminSauver);
    QMessageBox::information(this, "Sauvegarde", "Tache sauvegardée.");
}
