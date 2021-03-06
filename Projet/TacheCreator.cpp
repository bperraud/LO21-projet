#include <QMessageBox>
#include <QFileDialog>

#include "TacheCreator.h"
#include "TacheManager.h"
#include "PrecedenceManager.h"


/* --- [BEGIN] TacheCreator --- */


TacheCreator::TacheCreator(QWidget *parent) : QWidget(parent) {

    typeLabelu = new QLabel("Type : unitaire ");
    unitaire = new QRadioButton(this);
        unitaire->setChecked(true);

    typeLabelc = new QLabel(" composite");
    composite = new QRadioButton(this);
        composite->setChecked(false);

    titreLabel = new QLabel("titre", this);
    titre = new QLineEdit(this);
        titre->setEnabled(false);

    preemptive = new QCheckBox(this);
    preemptive->setEnabled(false);
    preemptiveLabel = new QLabel("préemptive", this);

    descriptionLabel = new QLabel("description", this);
    description = new QTextEdit(this);
        description->setEnabled(false);

    dispoLabel = new QLabel("disponibilité", this);
    dispo = new QDateEdit(QDate::currentDate(), this);
        dispo->setEnabled(false);

    echeanceLabel = new QLabel("échéance", this);
    echeance = new QDateEdit(QDate::currentDate(), this);
        echeance->setEnabled(false);

    dureeLabel = new QLabel("durée", this);
    duree = new QTimeEdit(QTime(1, 0), this);
        duree->setEnabled(false);

    predecesseursLabel = new QLabel("Tâches précédentes :", this);
    predecesseurs = new QListWidget(this);
    TacheManager& TM = *TacheManager::getInstance();
    for (Iterator<TacheManager, Tache> i = TM.begin(); i != TM.end(); ++i){
        QString UneTache = (*i).getTitre();
        QListWidgetItem* item = new QListWidgetItem(UneTache, predecesseurs);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        item->setCheckState(Qt::Unchecked);// AND initialize check state
    };
        predecesseurs->setEnabled(false);


    composantesLabel = new QLabel("Tâche composite de :", this);
    composantes = new QListWidget(this);
    for (Iterator<TacheManager, Tache> i = TM.begin(); i != TM.end(); ++i){
        QString UneTache = (*i).getTitre();
        if (!TM.getTacheMere(*i)){
            QListWidgetItem* item = new QListWidgetItem(UneTache, composantes);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
            item->setCheckState(Qt::Unchecked);// AND initialize check state
        }
    };
        composantes->setEnabled(false);

    creer = new QPushButton("Créer", this);
        creer->setEnabled(false);

    HC1 = new QHBoxLayout;
        HC1->addWidget(typeLabelu);
        HC1->addWidget(unitaire);
        HC1->addWidget(typeLabelc);
        HC1->addWidget(composite);
        HC1->addStretch();
    HC2 = new QHBoxLayout;
        HC2->addWidget(titreLabel);
        HC2->addWidget(titre);
        HC2->addWidget(preemptive);
        HC2->addWidget(preemptiveLabel);
    HC3 = new QHBoxLayout;
        HC3->addWidget(descriptionLabel);
        HC3->addWidget(description);
    HC4 = new QHBoxLayout;
        HC4->addWidget(dispoLabel);
        HC4->addWidget(dispo);
        HC4->addStretch();
        HC4->addWidget(echeanceLabel);
        HC4->addWidget(echeance);
        HC4->addStretch();
        HC4->addWidget(dureeLabel);
        HC4->addWidget(duree);
    HC5 = new QHBoxLayout;
        HC5->addWidget(predecesseursLabel);
        HC5->addWidget(predecesseurs);
    HC6 = new QHBoxLayout;
        HC6->addWidget(composantesLabel);
        HC6->addWidget(composantes);
    HC7 = new QHBoxLayout;
        HC7->addWidget(creer);


    VC = new QVBoxLayout;
        VC->addLayout(HC1);
        VC->addLayout(HC2);
        VC->addLayout(HC3);
        VC->addLayout(HC4);
        VC->addLayout(HC5);
        VC->addLayout(HC6);
        VC->addLayout(HC7);
        VC->addStretch();


    // Init

    veriftype();

    // Signaux

    QObject::connect(unitaire, SIGNAL(toggled(bool)), this, SLOT(veriftype()));
    QObject::connect(composite, SIGNAL(toggled(bool)), this, SLOT(veriftype()));
    QObject::connect(creer, SIGNAL(clicked()), this, SLOT(creerTache()));

    this->setLayout(VC);

}

void TacheCreator::updateTC() {
    TacheManager& TM = *TacheManager::getInstance();
    predecesseurs->clear();
    for (Iterator<TacheManager, Tache> i = TM.begin(); i != TM.end(); ++i){
        QString UneTache = (*i).getTitre();
        QListWidgetItem* item = new QListWidgetItem(UneTache, predecesseurs);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        item->setCheckState(Qt::Unchecked);// AND initialize check state
    };

    ProjetManager& ProjM = *ProjetManager::getInstance();
    composantes->clear();
    for (Iterator<TacheManager, Tache> i = TM.begin(); i != TM.end(); ++i){
        QString UneTache = (*i).getTitre();
        if ((!TM.getTacheMere(*i)) && (!ProjM.isTacheInProjet(*i))){
            QListWidgetItem* item = new QListWidgetItem(UneTache, composantes);
            item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
            item->setCheckState(Qt::Unchecked);// AND initialize check state
        }
    };
}

void TacheCreator::veriftype(){

    if (unitaire->isChecked())
        this->TacheUni();
    else if (composite->isChecked())
        this->TacheCompo();

}

void TacheCreator::TacheUni(){

    if (!titre->isEnabled())
        titre->setEnabled(true);

    if (!preemptive->isEnabled())
        preemptive->setEnabled(true);

    if (!description->isEnabled())
        description->setEnabled(true);

    if (!dispo->isEnabled())
        dispo->setEnabled(true);

    if (!echeance->isEnabled())
        echeance->setEnabled(true);

    if (!duree->isEnabled())
        duree->setEnabled(true);

    if (!predecesseurs->isEnabled())
        predecesseurs->setEnabled(true);

    if (!creer->isEnabled())
        creer->setEnabled(true);

    if (composantes->isEnabled())
        composantes->setEnabled(false);
}

void TacheCreator::TacheCompo(){

    if (!titre->isEnabled())
        titre->setEnabled(true);

    if (preemptive->isEnabled())
        preemptive->setEnabled(false);

    if (!description->isEnabled())
        description->setEnabled(true);

    if (!dispo->isEnabled())
        dispo->setEnabled(true);

    if (!echeance->isEnabled())
        echeance->setEnabled(true);

    if (duree->isEnabled())
        duree->setEnabled(false);

    if (!predecesseurs->isEnabled())
        predecesseurs->setEnabled(true);

    if (!creer->isEnabled())
        creer->setEnabled(true);

    if (!composantes->isEnabled())
        composantes->setEnabled(true);
}

void TacheCreator::creerTache(){

    if (titre->text() == ""){
        QMessageBox::warning(this, "Création impossible", "Pas de titre renseigné, impossible de créer la tâche.");
        return;
    }
    if (description->toPlainText() == ""){
        QMessageBox::warning(this, "Création impossible", "Pas de description renseignée, impossible de créer la tâche.");
        return;
    }

    TacheManager& TM = *TacheManager::getInstance();
    // Si le titre de la tâche en cours de création existe déjà
    if (TM.isTacheExistante(titre->text())){
        QMessageBox::warning(this, "Création impossible", "titre tâche déjà existant...");
        return;
    }
    // Si les dates de dispo et d'échéance sont incohérentes
    if (dispo->date() >= echeance->date()){
        QMessageBox::warning(this, "Sauvegarde impossible", "Incohérence de dates...");
        return;
    }

    //Cas de la Tache Unitaire
    if ((preemptive->isEnabled()) && (!composantes->isEnabled())) {

        // Si la durée est nulle
        if (duree->time() == QTime(0, 0)){
            QMessageBox::warning(this, "Sauvegarde impossible", "Durée nulle...");
            return;
        }
        if (preemptive->isChecked())
            TM.ajouterTacheUnitaire(titre->text(),description->toPlainText(),QTime(duree->time()),dispo->date(),echeance->date(),true);
        else
            TM.ajouterTacheUnitaire(titre->text(),description->toPlainText(),QTime(duree->time()),dispo->date(),echeance->date(),false);

        PrecedenceManager& PrM = *PrecedenceManager::getInstance();
        for (int i = 0; i < predecesseurs->count(); ++i) {
            QListWidgetItem *precedent = predecesseurs->item(i);
            if (precedent->checkState() == Qt::Checked) {
                    PrM.ajouterPrecedence(TM.getTache(precedent->text()), TM.getTache(titre->text()));
            }
        }
        QMessageBox::information(this, "Création", "Tâche créée.");
    }

    //Cas de la Tache Composite
    else {

        ListTaches sT;
        for (int i = 0; i < composantes->count(); ++i) {
            QListWidgetItem *composant = composantes->item(i);
            if (composant->checkState() == Qt::Checked)
                sT << &(TM.getTache(composant->text()));
        };
        TM.ajouterTacheComposite(titre->text(), description->toPlainText(), dispo->date(), echeance->date(), sT);
        PrecedenceManager& PrM = *PrecedenceManager::getInstance();
        for (int i = 0; i < predecesseurs->count(); ++i) {
            QListWidgetItem *precedent = predecesseurs->item(i);
            if (precedent->checkState() == Qt::Checked) {
                    PrM.ajouterPrecedence(TM.getTache(precedent->text()), TM.getTache(titre->text()));
            }
        }
        QMessageBox::information(this, "Création", "Tâche créée.");

    }
}


/* --- [END] TacheCreator --- */
