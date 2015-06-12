#include <QMessageBox>

#include "ImExManager.h"
#include "ProjetManager.h"

ImExManager::ImExManager(QWidget *parent) : QWidget(parent){

    formatLabel = new QLabel("Format :", this);
    formatGroup = new QButtonGroup(this);
    xmlRadio = new QRadioButton("XML", this);
    txtRadio = new QRadioButton("Texte", this);
    formatGroup->addButton(xmlRadio);
    formatGroup->addButton(txtRadio);

    actionLabel = new QLabel("Action :", this);
    actionGroup = new QButtonGroup(this);
    loadRadio = new QRadioButton("Charger", this);
    saveRadio = new QRadioButton("Sauver", this);
    actionGroup->addButton(loadRadio);
    actionGroup->addButton(saveRadio);

    pathLabel = new QLabel("Chemin du fichier :", this);
    findButton = new QPushButton("Sélectionner le chemin", this);
    pathEdit = new QLineEdit(this);

    validerAll = new QPushButton("Valider", this);
    validerAll->setFixedWidth(96);

    weekLabel = new QLabel("Programmations d'une semaine (choisir une date) :", this);
    jour = new QDateEdit(QDate::currentDate(), this);

    projLabel = new QLabel("Programmations d'un projet :", this);
    projets = new QComboBox(this);

    validerWeek = new QPushButton("Valider", this);
    validerWeek->setFixedWidth(96);

    validerProj = new QPushButton("Valider", this);
    validerProj->setFixedWidth(96);

    // Layout

    HL1 = new QHBoxLayout;
    HL1->addWidget(formatLabel);
    HL1->addWidget(xmlRadio);
    HL1->addWidget(txtRadio);
    HL1->addStretch();
    HL1->addWidget(actionLabel);
    HL1->addWidget(loadRadio);
    HL1->addWidget(saveRadio);

    HL2 = new QHBoxLayout;
    HL2->addWidget(pathLabel);
    HL2->addWidget(findButton);
    HL2->addWidget(pathEdit);
    HL2->addWidget(validerAll);

    HL3 = new QHBoxLayout;
    HL3->addWidget(weekLabel);
    HL3->addWidget(jour);
    HL3->addStretch();
    HL3->addWidget(validerWeek);

    HL4 = new QHBoxLayout;
    HL4->addWidget(projLabel);
    HL4->addWidget(projets);
    HL4->addStretch();
    HL4->addWidget(validerProj);

    VL = new QVBoxLayout;
    VL->addLayout(HL1);
    VL->addLayout(HL2);
    VL->addLayout(HL3);
    VL->addLayout(HL4);
    VL->addStretch();

    this->setLayout(VL);

    // Initialisation

    loadStrategy = 0;
    saveStrategy = 0;
    xmlRadio->setChecked(true);
    loadRadio->setChecked(true);
    format = "Fichiers XML (*.xml)";

    updateProj();

    // Signaux

    QObject::connect(formatGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(updateFormat(QAbstractButton*)));
    QObject::connect(findButton, SIGNAL(clicked()), this, SLOT(findFile()));
    QObject::connect(validerAll, SIGNAL(clicked()), this, SLOT(loadSaveAll()));
    QObject::connect(validerWeek, SIGNAL(clicked()), this, SLOT(loadSaveWeek()));
    QObject::connect(validerProj, SIGNAL(clicked()), this, SLOT(loadSaveProj()));

}

ImExManager::~ImExManager(){}

void ImExManager::updateProj(){
    ProjetManager& PM = *ProjetManager::getInstance();
    projets->clear();
    for (Iterator<ProjetManager, Projet> it = PM.begin(); it != PM.end(); ++it)
        projets->addItem((*it).getTitre());
    if (!projets->count()) validerProj->setDisabled(true);
    else validerProj->setEnabled(true);
}

void ImExManager::updateFormat(QAbstractButton* radioB){
    if (radioB == xmlRadio) format = "Fichiers XML (*.xml)";
    else if (radioB == txtRadio) format = "Fichiers texte (*.txt)";
}

void ImExManager::findFile(){
    QString newPath;
    if (actionGroup->checkedButton() == loadRadio)
        newPath = QFileDialog::getOpenFileName(this, "Charger un fichier", QString(), format);
    else newPath = QFileDialog::getSaveFileName(this, "Sauver un fichier", QString(), format);
    if (!newPath.isNull()){
        path = newPath;
        pathEdit->setText(path);
    }
}

void ImExManager::load(QString type){
    if (formatGroup->checkedButton() == xmlRadio) switchStrategy(new LoadXML);
    else if (formatGroup->checkedButton() == txtRadio){
        switchStrategy(new LoadTXT);
        QMessageBox::warning(this, "Chargement", "Annulation, fonction non encore implémentée dans cette version !");
        return;
    }
    if (!pathEdit->text().isEmpty()){
        if (type == "all") loadStrategy->load(pathEdit->text());
        else if (type == "week") loadStrategy->load(pathEdit->text(), jour->date().addDays(-jour->date().dayOfWeek()+1));
        else if (type == "proj") loadStrategy->load(pathEdit->text(), &ProjetManager::getInstance()->getProjet(projets->currentText()));
        else throw CalendarException("erreur chargement, type inconnu");
        QMessageBox::information(this, "Chargement", "Chargement réussi !");
        updateProj();
    }
    else QMessageBox::warning(this, "Chargement", "Chargement impossible : aucun chemin spécifié !");
}
void ImExManager::save(QString type){
    if (formatGroup->checkedButton() == xmlRadio) switchStrategy(new SaveXML);
    else if (formatGroup->checkedButton() == txtRadio){
        switchStrategy(new SaveTXT);
        QMessageBox::warning(this, "Chargement", "Annulation, fonction non encore implémentée dans cette version !");
        return;
    }
    if (!pathEdit->text().isEmpty()){
        if (type == "all") saveStrategy->save(pathEdit->text());
        else if (type == "week") saveStrategy->save(pathEdit->text(), jour->date());
        else if (type == "proj") saveStrategy->save(pathEdit->text(), QDate(), &ProjetManager::getInstance()->getProjet(projets->currentText()));
        else throw CalendarException("erreur sauvegarde, type inconnu");
        QMessageBox::information(this, "Sauvegarde", "Sauvegarde réussie !");
    }
    else QMessageBox::warning(this, "Sauvegarde", "Sauvegarde impossible : aucun chemin spécifié !");

}
