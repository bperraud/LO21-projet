#include <QMessageBox>

#include "ProgCreator.h"
#include "TacheManager.h"

ProgCreator::ProgCreator(QWidget *parent) : QWidget(parent){

    setMinimumHeight(600);
    setMaximumWidth(600);

    TacheManager& TM = *TacheManager::getInstance();

    progLabel = new QLabel("Programmation de :", this);

    radioBGroup = new QButtonGroup(this);
    radioBActivite = new QRadioButton("Activité", this);
    radioBActivite->setChecked(true);
    radioBTache = new QRadioButton("Tâche", this);
    radioBGroup->addButton(radioBActivite);
    radioBGroup->addButton(radioBTache);

    dateLabel = new QLabel("Date :", this);
    date = new QDateEdit(this);
    date->setMinimumDate(QDate::currentDate());

    horaireLabel = new QLabel("Début :", this);
    horaire = new QTimeEdit(this);
        horaire->setMinimumTime(QTime(8 ,0));
        horaire->setMaximumTime(QTime(19, 45));
    horaireFinLabel = new QLabel("Fin :", this);
    horaireFin = new QTimeEdit(this);
        horaireFin->setMinimumTime(QTime(8 ,15));
        horaireFin->setMaximumTime(QTime(20, 0));

    tacheLabel = new QLabel("Tâche à programmer :");
    tachesU = new QComboBox(this);
    for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i){
        if ((*i).isTacheUnitaire()){
            QString UneTache = (*i).getTitre();
            tachesU->addItem(UneTache);
        }
    }

    titreLabel = new QLabel("Titre :", this);
    titre = new QLineEdit(this);
    lieuLabel = new QLabel("Lieu :", this);
    lieu = new QLineEdit(this);
    descriptionLabel = new QLabel("Description :", this);
    description = new QTextEdit(this);
    description->setMaximumHeight(100);

    valider = new QPushButton("Valider", this);
    valider->setMaximumWidth(96);

    // Layout

    HL1 = new QHBoxLayout;
        HL1->addWidget(dateLabel);
        HL1->addWidget(date);
        HL1->addStretch();
        HL1->addWidget(progLabel);
        HL1->addWidget(radioBActivite);
        HL1->addWidget(radioBTache);
    HL2 = new QHBoxLayout;
        HL2->addWidget(horaireLabel);
        HL2->addWidget(horaire);
        HL2->addWidget(horaireFinLabel);
        HL2->addWidget(horaireFin);
        HL2->addStretch();
        HL2->addWidget(tacheLabel);
        HL2->addWidget(tachesU);
    HL3 = new QHBoxLayout;
        HL3->addWidget(titreLabel);
        HL3->addWidget(titre);
        HL3->addWidget(lieuLabel);
        HL3->addWidget(lieu);
    HL4 = new QHBoxLayout;
        HL4->addWidget(descriptionLabel);
        HL4->addWidget(description);

    VL = new QVBoxLayout;
        VL->addLayout(HL1);
        VL->addLayout(HL2);
        VL->addLayout(HL3);
        VL->addLayout(HL4);
        VL->addWidget(valider);
    VL->addStretch();


    this->setLayout(VL);

    // Initialisation

    switchProg(radioBActivite);

    // Signaux

    QObject::connect(radioBGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(switchProg(QAbstractButton*)));
    QObject::connect(valider, SIGNAL(clicked()), this, SLOT(sauverProg()));
}

ProgCreator::~ProgCreator(){}

void ProgCreator::switchProg(QAbstractButton* radioB){
    if (radioB == radioBActivite){
        titre->setEnabled(true);
        lieu->setEnabled(true);
        description->setEnabled(true);
        tachesU->setDisabled(true);
    }
    else{
        titre->setEnabled(false);
        lieu->setEnabled(false);
        description->setEnabled(false);
        tachesU->setDisabled(false);
    }
}

void ProgCreator::sauverProg(){
    ProgManager& PM = *ProgManager::getInstance();
    if (radioBActivite->isChecked()){
        PM.ajouterProgrammationA(date->date(), horaire->time(), horaireFin->time(), titre->text(), description->toPlainText(), lieu->text());
        QMessageBox::information(this, "Sauvegarde", "Programmation sauvegardée.");
    }
}
