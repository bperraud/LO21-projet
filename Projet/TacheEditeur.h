#ifndef TACHEEDITEUR_h
#define TACHEEDITEUR_h

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QDateEdit>
#include <QLabel>
#include <QCheckBox>

#include "Calendar.h"


class TacheEditeur : public QWidget{
Q_OBJECT

private:
    QVBoxLayout *VC;
    QHBoxLayout *HC1, *HC2, *HC3, *HC4;
    QLabel *titreLabel, *descriptionLabel, *preemptiveLabel, *dispoLabel, *echeanceLabel, *dureeLabel;
    QLineEdit *titre;
    QCheckBox *preemptive;
    QTextEdit *description;
    QSpinBox *dureeH, *dureeM;
    QDateEdit *dispo, *echeance;
    QPushButton *annuler, *sauver;

    Tache& tache;

public slots:
    void activerSauver(){ sauver->setEnabled(true); }
    void sauverTache();

public:
    explicit TacheEditeur(Tache& tacheToEdit, QWidget* parent = 0);

};

#endif
