#include <QObject>
#include <QMessageBox>
#include <QFileDialog>

#include "Calendar.h"
#include "TacheManager.h"
#include "TacheEditeur.h"


TacheEditeur::TacheEditeur(QWidget* parent) : QWidget(parent){

    //this->setWindowTitle(QString("Edition de la tâche ")+tache.getId());
    //this->setFixedSize(600, 300);
    //this->setEnabled(false);

    ChoixTacheLabel = new QLabel("Tache à éditer:");
    ChoixTache = new QComboBox;
        TacheManager& TM = *TacheManager::getInstance();
        for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i){
            QString UneTache = (*i).getTitre();
            ChoixTache->addItem(UneTache);
        }

    titreLabel = new QLabel("titre", this);
    titre = new QLineEdit(this);
        titre->setEnabled(false);

    preemptive = new QCheckBox(this);
    preemptive->setEnabled(false);
    preemptiveLabel = new QLabel("preemptive", this);

    descriptionLabel = new QLabel("description", this);
    description = new QTextEdit(this);
        description->setEnabled(false);

    dispoLabel = new QLabel("disponibilité", this);
    dispo = new QDateEdit(this);
        dispo->setEnabled(false);

    echeanceLabel = new QLabel("échéance", this);
    echeance = new QDateEdit(this);
        echeance->setEnabled(false);

    dureeLabel = new QLabel("durée", this);
        dureeH = new QSpinBox(this);
            dureeH->setMinimum(0);
            dureeH->setSuffix(" heure(s)");
            dureeH->setEnabled(false);
        dureeM = new QSpinBox(this);
            dureeM->setRange(0, 59);
            dureeM->setSuffix(" minute(s)");
            dureeM->setEnabled(false);

    predecesseursLabel = new QLabel("taches précédentes", this);
    predecesseurs = new QListWidget(this);
        predecesseurs->setEnabled(false);

    composantesLabel = new QLabel("Tache composite de:", this);
    composantes = new QListWidget(this);
        composantes->setEnabled(false);

    annuler = new QPushButton("Annuler", this);
    sauver = new QPushButton("Sauver", this);
        sauver->setEnabled(false);


    HC0 = new QHBoxLayout;
        HC0->addWidget(ChoixTacheLabel);
        HC0->addWidget(ChoixTache);
    HC1 = new QHBoxLayout;
        HC1->addWidget(titreLabel);
        HC1->addWidget(titre);
        HC1->addWidget(preemptive);
        HC1->addWidget(preemptiveLabel);
    HC2 = new QHBoxLayout;
        HC2->addWidget(descriptionLabel);
        HC2->addWidget(description);
    HC3 = new QHBoxLayout;
        HC3->addWidget(dispoLabel);
        HC3->addWidget(dispo);
        HC3->addWidget(echeanceLabel);
        HC3->addWidget(echeance);
        HC3->addWidget(dureeLabel);
        HC3->addWidget(dureeH);
        HC3->addWidget(dureeM);
    HC4 = new QHBoxLayout;
        HC4->addWidget(predecesseursLabel);
        HC4->addWidget(predecesseurs);
    HC5 = new QHBoxLayout;
        HC5->addWidget(composantesLabel);
        HC5->addWidget(composantes);
    HC6 = new QHBoxLayout;
        HC6->addWidget(annuler);
        HC6->addWidget(sauver);

    VC = new QVBoxLayout;
        VC->addLayout(HC0);
        VC->addLayout(HC1);
        VC->addLayout(HC2);
        VC->addLayout(HC3);
        VC->addLayout(HC4);
        VC->addLayout(HC5);
        VC->addLayout(HC6);

        QObject::connect(titre, SIGNAL(textEdited(const QString&)), this, SLOT(activerSauver()));
        QObject::connect(description, SIGNAL(textChanged()), this, SLOT(activerSauver()));
        QObject::connect(dispo, SIGNAL(dateChanged(const QDate&)), this, SLOT(activerSauver()));
        QObject::connect(echeance, SIGNAL(dateChanged(const QDate&)), this, SLOT(activerSauver()));
        QObject::connect(predecesseurs, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(activerSauver()));
        QObject::connect(preemptive, SIGNAL(stateChanged(int)), this, SLOT(activerSauver()));
        QObject::connect(dureeH, SIGNAL(valueChanged(int)), this, SLOT(activerSauver()));
        QObject::connect(dureeM, SIGNAL(valueChanged(int)), this, SLOT(activerSauver()));
        QObject::connect(composantes, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(activerSauver()));

    QObject::connect(ChoixTache, SIGNAL(currentTextChanged(QString)), this, SLOT(initialiserEditeur(QString)));

    QObject::connect(annuler, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(sauver, SIGNAL(clicked()), this, SLOT(sauverTache()));

    this->setLayout(VC);
    this->initialiserEditeur(ChoixTache->currentText());


}

void TacheEditeur::initialiserEditeur(QString nomtache) {

    TacheManager& TM = *TacheManager::getInstance();
    Tache& tacheToEdit = TM.getTache(nomtache);

    titre->setEnabled(true);
    titre->setText(tacheToEdit.getTitre());

    description->setEnabled(true);
    description->setText(tacheToEdit.getDescription());

    dispo->setEnabled(true);
    dispo->setDate(tacheToEdit.getDateDisponibilite());

    echeance->setEnabled(true);
    echeance->setDate(tacheToEdit.getDateEcheance());


    predecesseurs->setEnabled(true);
    predecesseurs->clear();
    PrecedenceManager& PrM = *PrecedenceManager::getInstance();
    for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i){
        QString UneTache = (*i).getTitre();
        if (UneTache != tacheToEdit.getTitre()) {
            QListWidgetItem* item = new QListWidgetItem(UneTache, predecesseurs);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
            if (PrM.isPrecedence(*i, tacheToEdit))
                item->setCheckState(Qt::Checked);
            else item->setCheckState(Qt::Unchecked);// AND initialize check state
        };
    };

    if (tacheToEdit.isTacheUnitaire()){

        tacheU = &dynamic_cast<TacheUnitaire&>(tacheToEdit);

        if (composantes->isEnabled())
            composantes->setEnabled(false);

        preemptive->setEnabled(true);
        preemptive->setChecked(tacheU->isPreemptive());

        dureeH->setEnabled(true);
            dureeH->setValue(tacheU->getDuree().getHeure());
        dureeM->setEnabled(true);
            dureeM->setValue(tacheU->getDuree().getMinute());
    }
    else {

        tacheC = &dynamic_cast<TacheComposite&>(tacheToEdit);

        if (preemptive->isEnabled())
            preemptive->setEnabled(false);
        if (dureeH->isEnabled())
            dureeH->setEnabled(false);
        if (dureeM->isEnabled())
            dureeM->setEnabled(false);

        composantes->setEnabled(true);
        composantes->clear();
        for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i){
            QString UneTache = (*i).getTitre();
            if (UneTache != tacheC->getTitre()){
                QListWidgetItem* item = new QListWidgetItem(UneTache, composantes);
                item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
                if (TM.getTacheMere(*i))
                    if (TM.getTacheMere(*i)->getTitre() == tacheC->getTitre())
                        item->setCheckState(Qt::Checked);
                    else item->setCheckState(Qt::Unchecked);// AND initialize check state
                else item->setCheckState(Qt::Unchecked);
            };
        };


    }

    sauver->setEnabled(false);

}

void TacheEditeur::sauverTache(){

    if ((preemptive->isEnabled()) && (!composantes->isEnabled())) {

        TacheManager& TM = *TacheManager::getInstance();
        // Si le titre de la tâche en cours d'édition existe déjà et que ce n'est pas celui de la tâche chargée
        if (TM.isTacheExistante(titre->text())
            && &(TM.getTache(titre->text())) != tacheU){
            QMessageBox::warning(this, "Sauvegarde impossible", "titre tâche déjà existant...");
            return;
        }
        // Si les dates de dispo et d'échéance sont incohérentes
        if (dispo->date() >= echeance->date()){
                QMessageBox::warning(this, "Sauvegarde impossible", "Incohérence de dates...");
                return;
        }
        // Si la durée est nulle
        if (!(dureeH->value() || dureeM->value())){
                QMessageBox::warning(this, "Sauvegarde impossible", "Durée nulle...");
                return;
        }
        if (tacheU->getTitre() != titre->text()) tacheU->setTitre(titre->text());
        preemptive->isChecked() ? tacheU->setPreemptive(true) : tacheU->setPreemptive(false);
        tacheU->setDescription(description->toPlainText());
        tacheU->setDatesDisponibiliteEcheance(dispo->date(), echeance->date());
        tacheU->setDuree(Duree(dureeH->value(), dureeM->value()));
        PrecedenceManager& PrM = *PrecedenceManager::getInstance();
        for (int i = 0; i < predecesseurs->count(); ++i) {
            QListWidgetItem *precedent = predecesseurs->item(i);
            if (precedent->checkState() == Qt::Checked) {
                if (!PrM.isPrecedence(TM.getTache(precedent->text()), *tacheU))
                    PrM.ajouterPrecedence(TM.getTache(precedent->text()), *tacheU);
            }
            else if (precedent->checkState() == Qt::Unchecked) {
                if (PrM.isPrecedence(TM.getTache(precedent->text()), *tacheU))
                    PrM.supprimerPrecedence(TM.getTache(precedent->text()), *tacheU);
            }
        }
        QString cheminSauver = QFileDialog::getSaveFileName();
        TM.save(cheminSauver);
        QMessageBox::information(this, "Sauvegarde", "Tache sauvegardée.");
    }

    else {

        TacheManager& TM = *TacheManager::getInstance();
        // Si le titre de la tâche en cours d'édition existe déjà et que ce n'est pas celui de la tâche chargée
        if (TM.isTacheExistante(titre->text())
            && &(TM.getTache(titre->text())) != tacheC){
            QMessageBox::warning(this, "Sauvegarde impossible", "titre tâche déjà existant...");
            return;
        }
        // Si les dates de dispo et d'échéance sont incohérentes
        if (dispo->date() >= echeance->date()){
                QMessageBox::warning(this, "Sauvegarde impossible", "Incohérence de dates...");
                return;
        }
        if (tacheC->getTitre() != titre->text()) tacheC->setTitre(titre->text());
        tacheC->setDescription(description->toPlainText());
        tacheC->setDatesDisponibiliteEcheance(dispo->date(), echeance->date());
        PrecedenceManager& PrM = *PrecedenceManager::getInstance();
        for (int i = 0; i < predecesseurs->count(); ++i) {
            QListWidgetItem *precedent = predecesseurs->item(i);
            if (precedent->checkState() == Qt::Checked) {
               if (!PrM.isPrecedence(TM.getTache(precedent->text()), *tacheC))
                   PrM.ajouterPrecedence(TM.getTache(precedent->text()), *tacheC);
            }
            else if (precedent->checkState() == Qt::Unchecked) {
                if (PrM.isPrecedence(TM.getTache(precedent->text()), *tacheC))
                    PrM.supprimerPrecedence(TM.getTache(precedent->text()), *tacheC);
            }
        }
        QString cheminSauver = QFileDialog::getSaveFileName();
        TM.save(cheminSauver);
        QMessageBox::information(this, "Sauvegarde", "Tache sauvegardée.");

    }

}



