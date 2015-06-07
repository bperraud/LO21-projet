#include <QMessageBox>

#include "ImExManager.h"

ImExManager::ImExManager(QWidget *parent) : QWidget(parent){

    //setMinimumHeight(800);

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

    valider = new QPushButton("Valider", this);
    valider->setFixedWidth(96);

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
    HL2->addWidget(valider);

    VL = new QVBoxLayout;
    VL->addLayout(HL1);
    VL->addLayout(HL2);
    VL->addStretch();

    this->setLayout(VL);

    // Initialisation

    loadStrategy = 0;
    saveStrategy = 0;
    xmlRadio->setChecked(true);
    loadRadio->setChecked(true);
    format = "Fichiers XML (*.xml)";


    // Signaux

    QObject::connect(formatGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(updateFormat(QAbstractButton*)));
    QObject::connect(findButton, SIGNAL(clicked()), this, SLOT(findFile()));
    QObject::connect(valider, SIGNAL(clicked()), this, SLOT(chooseAction()));

}

ImExManager::~ImExManager(){}

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



void ImExManager::load(){
    if (formatGroup->checkedButton() == xmlRadio) switchStrategy(new LoadXML);
    else if (formatGroup->checkedButton() == txtRadio) switchStrategy(new LoadTXT);
    if (!pathEdit->text().isEmpty()){
        loadStrategy->load(pathEdit->text());
        QMessageBox::information(this, "Chargement", "Chargement réussi !");
    }
    else QMessageBox::information(this, "Chargement", "Chargement impossible : aucun chemin spécifié !");
}
void ImExManager::save(){
    if (formatGroup->checkedButton() == xmlRadio) switchStrategy(new SaveXML);
    else if (formatGroup->checkedButton() == txtRadio) switchStrategy(new SaveTXT);
    if (!pathEdit->text().isEmpty()){
        saveStrategy->save(pathEdit->text());
        QMessageBox::information(this, "Sauvegarde", "Sauvegarde réussie !");
    }
    else QMessageBox::information(this, "Sauvegarde", "Sauvegarde impossible : aucun chemin spécifié !");

}
