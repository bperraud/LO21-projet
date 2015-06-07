#include <QMessageBox>
#include <QFileDialog>

#include "TacheEditeur.h"
#include "TacheManager.h"
#include "PrecedenceManager.h"


/* --- [BEGIN] TacheCreator --- */


TacheCreator::TacheCreator(QWidget *parent) : QWidget(parent) {

    typeLabelu = new QLabel("Type: unitaire ");
    unitaire = new QRadioButton(this);
        unitaire->setChecked(false);

    typeLabelc = new QLabel(" composite");
    composite = new QRadioButton(this);
        composite->setChecked(false);

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
    duree = new QTimeEdit(this);
        duree->setEnabled(false);

    predecesseursLabel = new QLabel("taches précédentes", this);
    predecesseurs = new QListWidget(this);
    TacheManager& TM = *TacheManager::getInstance();
    PrecedenceManager& PrM = *PrecedenceManager::getInstance();
    for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i){
        QString UneTache = (*i).getTitre();
        QListWidgetItem* item = new QListWidgetItem(UneTache, predecesseurs);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        item->setCheckState(Qt::Unchecked);// AND initialize check state
    };
        predecesseurs->setEnabled(false);


    composantesLabel = new QLabel("Tache composite de:", this);
    composantes = new QListWidget(this);
    for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i){
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
        HC4->addWidget(echeanceLabel);
        HC4->addWidget(echeance);
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


    QObject::connect(unitaire, SIGNAL(toggled(bool)), this, SLOT(veriftype()));
    QObject::connect(composite, SIGNAL(toggled(bool)), this, SLOT(veriftype()));
    QObject::connect(creer, SIGNAL(clicked()), this, SLOT(creerTache()));

    this->setLayout(VC);

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

    if (titre->text() == "")
        QMessageBox::warning(this, "Création impossible", "Pas de titre renseigné, impossible de créer la tâche.");
    if (description->toPlainText() == "")
        QMessageBox::warning(this, "Création impossible", "Pas de description renseignée, impossible de créer la tâche.");

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
        if (duree->time().isNull()){
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
        QMessageBox::information(this, "Création", "Tache créée.");
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
        QMessageBox::information(this, "Création", "Tache créée.");

    }
}


/* --- [END] TacheCreator --- */
// ----------------------------------------------------------------------------------------------------
/* --- [BEGIN] TacheEditeur --- */

TacheEditeur::TacheEditeur(QWidget* parent) : QWidget(parent){

    setMaximumSize(QSize(800, 800));

    ChoixTacheLabel = new QLabel("Tâche à éditer :", this);
    ChoixTache = new QComboBox(this);
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
    duree = new QTimeEdit(this);
        duree->setEnabled(false);


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
        HC3->addWidget(duree);
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
        VC->addStretch();

        QObject::connect(titre, SIGNAL(textEdited(const QString&)), this, SLOT(activerSauver()));
        QObject::connect(description, SIGNAL(textChanged()), this, SLOT(activerSauver()));
        QObject::connect(dispo, SIGNAL(dateChanged(const QDate&)), this, SLOT(activerSauver()));
        QObject::connect(echeance, SIGNAL(dateChanged(const QDate&)), this, SLOT(activerSauver()));
        QObject::connect(predecesseurs, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(activerSauver()));
        QObject::connect(preemptive, SIGNAL(stateChanged(int)), this, SLOT(activerSauver()));
        QObject::connect(duree, SIGNAL(timeChanged(QTime)), this, SLOT(activerSauver()));
        QObject::connect(composantes, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(activerSauver()));

    QObject::connect(ChoixTache, SIGNAL(currentTextChanged(QString)), this, SLOT(initialiserEditeur(QString)));

    QObject::connect(annuler, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(sauver, SIGNAL(clicked()), this, SLOT(sauverTache()));

    this->setLayout(VC);
    this->initialiserEditeur(ChoixTache->currentText());


}

void TacheEditeur::initialiserEditeur(QString nomtache){

    TacheManager& TM = *TacheManager::getInstance();

    if (!nomtache.isEmpty()){

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

            duree->setEnabled(true);
            duree->setTime(QTime(tacheU->getDuree().hour(), tacheU->getDuree().minute()));
        }
        else {

            tacheC = &dynamic_cast<TacheComposite&>(tacheToEdit);

            if (preemptive->isEnabled())
                preemptive->setEnabled(false);
            if (duree->isEnabled())
                duree->setEnabled(false);

            composantes->setEnabled(true);
            composantes->clear();
            for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i){
                QString UneTache = (*i).getTitre();
                if (UneTache != tacheC->getTitre()){
                    if (!TM.getTacheMere(*i)){
                        QListWidgetItem* item = new QListWidgetItem(UneTache, composantes);
                        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
                        item->setCheckState(Qt::Unchecked);// AND initialize check state
                    }
                    else if (TM.getTacheMere(*i)->getTitre() == tacheC->getTitre()){
                        QListWidgetItem* item = new QListWidgetItem(UneTache, composantes);
                        item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
                        item->setCheckState(Qt::Checked);// AND initialize check state
                    }
                };
            };
        }
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
        if (duree->time().isNull()){
                QMessageBox::warning(this, "Sauvegarde impossible", "Durée nulle...");
                return;
        }
        if (tacheU->getTitre() != titre->text()) tacheU->setTitre(titre->text());
        preemptive->isChecked() ? tacheU->setPreemptive(true) : tacheU->setPreemptive(false);
        tacheU->setDescription(description->toPlainText());
        tacheU->setDatesDisponibiliteEcheance(dispo->date(), echeance->date());
        tacheU->setDuree(duree->time());
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
        QMessageBox::information(this, "Edition", "Tache éditée.");
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

        for (int i = 0; i < composantes->count(); ++i) {
            QListWidgetItem *composant = composantes->item(i);
            if (composant->checkState() == Qt::Checked) {
               if (!TM.getTacheMere(TM.getTache(composant->text())))
                   tacheC->addSousTache(&(TM.getTache(composant->text())));
            }
            else if (composant->checkState() == Qt::Unchecked) {
                if (TM.getTacheMere(TM.getTache(composant->text()))->getTitre() == tacheC->getTitre())
                    tacheC->rmSousTache(&(TM.getTache(composant->text())));
            }
        }

        QMessageBox::information(this, "Edition", "Tache éditée.");

    }

}

void TacheEditeur::updateT(){
    TacheManager& TM = *TacheManager::getInstance();
    ChoixTache->clear();
    for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i){
        QString UneTache = (*i).getTitre();
        ChoixTache->addItem(UneTache);
    }
}


/* --- [END] TacheEditeur --- */



