#ifndef PROGCREATOR_H
#define PROGCREATOR_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QButtonGroup>
#include <QRadioButton>
#include <QComboBox>
#include <QPushButton>

#include "Calendar.h"

class ProgCreator : public QWidget{
Q_OBJECT

private:
    QLabel *progLabel, *titreLabel, *lieuLabel, *descriptionLabel, *dateLabel, *horaireLabel, *horaireFinLabel, *tacheLabel;
    QDateEdit *date;
    QTimeEdit *horaire, *horaireFin;
    QButtonGroup *radioBGroup;
    QRadioButton *radioBActivite, *radioBTache;
    // Programmation d'une activité :
    QLineEdit *titre, *lieu;
    QTextEdit *description;
    // Programmation d'une tâche unitaire :
    QComboBox *tachesU;
    TacheUnitaire *tache;

    QPushButton *valider;

    QVBoxLayout *VL;
    QHBoxLayout *HL1, *HL2, *HL3, *HL4;

public:
    explicit ProgCreator(QWidget *parent = 0);
    ~ProgCreator();

public slots:
    void switchProg(QAbstractButton* radioB);
    void sauverProg();
    void setTache(QString titre);
    void updateHoraire(QTime h){ if (h < horaire->time()) horaire->setTime(h); }
    void updateHoraireFin(QTime h);
    void updateTachesU();
};



#endif // PROGCREATOR_H
