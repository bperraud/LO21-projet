#include <QMessageBox>

#include "ProgCreator.h"
#include "TacheManager.h"
#include "ProgManager.h"

ProgCreator::ProgCreator(QWidget *parent) : QWidget(parent){

    setMinimumHeight(800);
    setMaximumWidth(800);

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

    tacheLabel = new QLabel("Tâche à programmer :", this);
    tachesU = new QComboBox(this);

    titreLabel = new QLabel("Titre * :", this);
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

    updateTachesU();
    switchProg(radioBActivite);

    // Signaux

    QObject::connect(radioBGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(switchProg(QAbstractButton*)));
    QObject::connect(valider, SIGNAL(clicked()), this, SLOT(sauverProg()));
    QObject::connect(tachesU, SIGNAL(activated(QString)), this, SLOT(setTache(QString)));
    QObject::connect(horaireFin, SIGNAL(userTimeChanged(QTime)), this, SLOT(updateHoraire(QTime)));
    QObject::connect(horaire, SIGNAL(userTimeChanged(QTime)), this, SLOT(updateHoraireFin(QTime)));
}

ProgCreator::~ProgCreator(){}

void ProgCreator::switchProg(QAbstractButton* radioB){
    if (radioB == radioBActivite){
        tache = 0;
        titre->setEnabled(true);
        lieu->setEnabled(true);
        description->setEnabled(true);
        tachesU->setDisabled(true);
        horaireFin->setDisabled(false);
        horaireFin->setMaximumTime(QTime(20, 0));
    }
    else{
        setTache(tachesU->currentText());
        titre->setEnabled(false);
        lieu->setEnabled(false);
        description->setEnabled(false);
        tachesU->setDisabled(false);
    }
}

void ProgCreator::updateHoraireFin(QTime h){
    if (radioBTache->isChecked()){
        if (!tache->isPreemptive()){
            horaireFin->setMaximumTime(QTime(20, 0));
            int dureeSec = QTime(0, 0).secsTo(tache->getDuree());
            QTime newHF = h.addSecs(dureeSec);
            if (newHF < QTime(20, 0)) horaireFin->setTime(newHF);
            else{
                horaireFin->setTime(QTime(20, 0));
                QTime newH = horaireFin->time().addSecs(-dureeSec);
                horaire->setTime(newH);
            }
        }
        else{
            int dureeRestSec = QTime(0, 0).secsTo(tache->getDureeRestante());
            QTime newHFmax = (h.addSecs(dureeRestSec) > QTime(20, 0)) ? QTime(20, 0) : h.addSecs(dureeRestSec);
            horaireFin->setMaximumTime(newHFmax);
        }
    }
    if (horaireFin->time() < horaire->time())
        horaireFin->setTime(horaire->time());
}

void ProgCreator::setTache(QString titre){
    TacheManager& TM = *TacheManager::getInstance();
    if (!titre.isEmpty() && radioBGroup->checkedButton() == radioBTache){
        tache = &dynamic_cast<TacheUnitaire&>((TM.getTache(titre)));
        horaireFin->setDisabled(tache->isPreemptive() ? false : true);
        updateHoraireFin(horaire->time());
    }
}

void ProgCreator::updateTachesU(){
    TacheManager& TM = *TacheManager::getInstance();
    ProjetManager& PM = *ProjetManager::getInstance();
    ProgManager& ProgM = *ProgManager::getInstance();
    tachesU->clear();
    for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i){
        if ((*i).isTacheUnitaire() && PM.isTacheInProjet(*i)){
            TacheUnitaire& TU = dynamic_cast<TacheUnitaire&>(*i);
            QString titre = TU.getTitre();
            if (TU.isPreemptive() && TU.getDureeRestante() > QTime(0, 0)){
                tachesU->addItem(titre);
            }
            else if (!TU.isPreemptive() && !ProgM.trouverProgrammationT(TU))
                tachesU->addItem(titre);
        }
    }
    if (!tachesU->count()){
        radioBActivite->setChecked(true);
        radioBTache->setEnabled(false);
        switchProg(radioBActivite);
    }
    else{
        radioBTache->setEnabled(true);
        setTache(tachesU->currentText());
    }
}

void ProgCreator::sauverProg(){
    ProgManager& PM = *ProgManager::getInstance();
    if (PM.programmationExists(date->date(), horaire->time(), horaireFin->time())){
        QMessageBox::information(this, "Sauvegarde", "Échec de la sauvegarde : l'horaire est déjà pris !");
        return;
    }
    if (horaire->time() >= horaireFin->time()){
        QMessageBox::information(this, "Sauvegarde", "Échec de la sauvegarde : horaire incohérent !");
        return;
    }
    if (radioBActivite->isChecked()){
        if (titre->text().isEmpty()){
            QMessageBox::information(this, "Sauvegarde", "Échec de la sauvegarde : veuillez renseigner un titre !");
            return;
        }
        PM.ajouterProgrammationA(date->date(), horaire->time(), horaireFin->time(), titre->text(), description->toPlainText(), lieu->text());
        QMessageBox::information(this, "Sauvegarde", "Activité programmée.");
    }
    else{
        PM.ajouterProgrammationT(date->date(), horaire->time(),horaireFin->time(), *tache);
        if (tache->isPreemptive()){
            QTime d = horaireFin->time();
            int dureeSec = QTime(0, 0).secsTo(horaire->time());
            d = d.addSecs(-dureeSec);
            PM.updateDuree(*tache, d);
        }
        updateTachesU();
        QMessageBox::information(this, "Sauvegarde", "Tâche programmée.");
    }
}
