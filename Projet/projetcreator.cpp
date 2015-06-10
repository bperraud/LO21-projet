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
    for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i){
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
    for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i){
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

    QMessageBox::information(this, "Création", "Tache créée.");

}

// --- [END] ProjetCreator ---
// --------------------------------------------------------------------------------------------------------------------------------------
// --- [BEGIN] ProjetEditeur ---

ProjetEditeur::ProjetEditeur(QWidget* parent) : QWidget(parent){

    setMaximumSize(QSize(800, 800));

    ChoixProjetLabel = new QLabel("Projet à éditer :", this);
    ChoixProjet = new QComboBox(this);
        ProjetManager& ProjM = *ProjetManager::getInstance();
        for (ProjetManager::iterator i = ProjM.begin(); i != ProjM.end(); ++i){
            QString UnProjet = (*i).getTitre();
            ChoixProjet->addItem(UnProjet);
        }

    titreLabel = new QLabel("titre", this);
    titre = new QLineEdit(this);
        titre->setEnabled(false);

    descriptionLabel = new QLabel("description", this);
    description = new QTextEdit(this);
        description->setEnabled(false);

    dispoLabel = new QLabel("disponibilité", this);
    dispo = new QDateEdit(QDate::currentDate(), this);
        dispo->setEnabled(false);

    echeanceLabel = new QLabel("échéance", this);
    echeance = new QDateEdit(QDate::currentDate(), this);
        echeance->setEnabled(false);

    tachesLabel = new QLabel("Tâches:", this);
    taches = new QListWidget(this);
        taches->setEnabled(false);

    editer = new QPushButton("Editer", this);
        editer->setEnabled(false);


    HC0 = new QHBoxLayout;
        HC0->addWidget(ChoixProjetLabel);
        HC0->addWidget(ChoixProjet);
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
        HC5->addWidget(editer);

    VC = new QVBoxLayout;
        VC->addLayout(HC0);
        VC->addLayout(HC1);
        VC->addLayout(HC2);
        VC->addLayout(HC3);
        VC->addLayout(HC4);
        VC->addLayout(HC5);
        VC->addStretch();

        QObject::connect(titre, SIGNAL(textEdited(const QString&)), this, SLOT(activerEditer()));
        QObject::connect(description, SIGNAL(textChanged()), this, SLOT(activerEditer()));
        QObject::connect(dispo, SIGNAL(dateChanged(const QDate&)), this, SLOT(activerEditer()));
        QObject::connect(echeance, SIGNAL(dateChanged(const QDate&)), this, SLOT(activerEditer()));
        QObject::connect(taches, SIGNAL(itemChanged(QListWidgetItem*)), this, SLOT(activerEditer()));

    QObject::connect(ChoixProjet, SIGNAL(currentTextChanged(QString)), this, SLOT(initialiserEditeur(QString)));

    QObject::connect(editer, SIGNAL(clicked()), this, SLOT(editerProjet()));

    this->setLayout(VC);
    this->initialiserEditeur(ChoixProjet->currentText());


}

void ProjetEditeur::initialiserEditeur(QString nomprojet){

    ProjetManager& ProjM = *ProjetManager::getInstance();

    if (!nomprojet.isEmpty()){

        Projet& projetToEdit = ProjM.getProjet(nomprojet);
        projet = &projetToEdit;

        titre->setEnabled(true);
        titre->setText(projetToEdit.getTitre());

        description->setEnabled(true);
        description->setText(projetToEdit.getDescription());

        dispo->setEnabled(true);
        dispo->setDate(projetToEdit.getDateDisponibilite());

        echeance->setEnabled(true);
        echeance->setDate(projetToEdit.getDateEcheance());

        taches->setEnabled(true);
        taches->clear();
        TacheManager& TM = *TacheManager::getInstance();
        for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i){
            QString UneTache = (*i).getTitre();
            if (ProjM.isTacheInProjet(*i)){
                if (ProjM.getProjet(*i)->getTitre() == projetToEdit.getTitre()){
                    QListWidgetItem* item = new QListWidgetItem(UneTache, taches);
                    item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
                    item->setCheckState(Qt::Checked);// AND initialize check state
                }
            }
            else {
                QListWidgetItem* item = new QListWidgetItem(UneTache, taches);
                item->setFlags(item->flags() | Qt::ItemIsUserCheckable); // set checkable flag
                item->setCheckState(Qt::Unchecked);// AND initialize check state
            }

        }
    }

    editer->setEnabled(false);

}


void ProjetEditeur::editerProjet(){

    ProjetManager& ProjM = *ProjetManager::getInstance();
    // Si le titre de la tâche en cours d'édition existe déjà et que ce n'est pas celui de la tâche chargée
    if (ProjM.isProjetExistant(titre->text())
        && &(ProjM.getProjet(titre->text())) != projet){
        QMessageBox::warning(this, "Edition impossible", "titre projet déjà existant...");
        return;
    }
    // Si les dates de dispo et d'échéance sont incohérentes
    if (dispo->date() >= echeance->date()){
        QMessageBox::warning(this, "Edition impossible", "Incohérence de dates...");
        return;
    }

    if (projet->getTitre() != titre->text()) projet->setTitre(titre->text());
    projet->setDescription(description->toPlainText());
    projet->setDatesDisponibiliteEcheance(dispo->date(), echeance->date());

    TacheManager& TM = *TacheManager::getInstance();
    for (int i = 0; i < taches->count(); ++i) {
        QListWidgetItem *tache = taches->item(i);
        if (tache->checkState() == Qt::Checked) {
            if (!ProjM.isTacheInProjet(TM.getTache(tache->text())))
                   projet->addTache(&(TM.getTache(tache->text())));
            }
            else if (tache->checkState() == Qt::Unchecked) {
                if (ProjM.getProjet(TM.getTache(tache->text())) == projet)
                    projet->rmTache(&(TM.getTache(tache->text())));
            }
        }

    QMessageBox::information(this, "Edition", "Tache éditée.");

}

void ProjetEditeur::updateProjEdit(){
    ProjetManager& ProjM = *ProjetManager::getInstance();
    ChoixProjet->clear();
    for (ProjetManager::iterator i = ProjM.begin(); i != ProjM.end(); ++i){
        QString UnProjet = (*i).getTitre();
        ChoixProjet->addItem(UnProjet);
    }
}
