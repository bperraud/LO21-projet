#ifndef TACHEEDITEUR_h
#define TACHEEDITEUR_h

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QDateEdit>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QRadioButton>

#include "Calendar.h"


class TacheCreator : public QWidget{
Q_OBJECT

private:
    QVBoxLayout *VC;
    QHBoxLayout *HC1, *HC2, *HC3, *HC4, *HC5, *HC6, *HC7;
    QLabel *typeLabelu, *typeLabelc, *titreLabel, *descriptionLabel, *preemptiveLabel, *dispoLabel, *echeanceLabel, *dureeLabel, *predecesseursLabel, *composantesLabel;
    QRadioButton *unitaire, *composite;
    QLineEdit *titre;
    QCheckBox *preemptive;
    QTextEdit *description;
    QSpinBox *dureeH, *dureeM;
    QDateEdit *dispo, *echeance;
    QListWidget *predecesseurs, *composantes;
    QPushButton *creer;

public slots:
    void creerTache();
    void TacheUni();
    void TacheCompo();
    void veriftype();

public:
    explicit TacheCreator(QWidget* parent = 0);

};

class TacheEditeur : public QWidget{
Q_OBJECT

private:
    QVBoxLayout *VC;
    QHBoxLayout *HC0, *HC1, *HC2, *HC3, *HC4, *HC5, *HC6;
    QLabel *ChoixTacheLabel, *titreLabel, *descriptionLabel, *preemptiveLabel, *dispoLabel, *echeanceLabel, *dureeLabel, *predecesseursLabel, *composantesLabel;
    QComboBox *ChoixTache;
    QLineEdit *titre;
    QCheckBox *preemptive;
    QTextEdit *description;
    QSpinBox *dureeH, *dureeM;
    QDateEdit *dispo, *echeance;
    QListWidget *predecesseurs, *composantes;
    QPushButton *annuler, *sauver;

    TacheUnitaire* tacheU;
    TacheComposite* tacheC;

public slots:
    void activerSauver(){ sauver->setEnabled(true); }
    //void sauverTacheU();
    //void sauverTacheC();
    void sauverTache();
    void initialiserEditeur(QString);

public:
    //explicit TacheEditeur(TacheUnitaire& tacheToEdit, QWidget* parent = 0);
    //explicit TacheEditeur(TacheComposite& tachetoEdit, QWidget* parent = 0);
    //explicit TacheEditeur(Tache& tacheToEdit, QWidget* parent = 0);
    explicit TacheEditeur(QWidget* parent = 0);

};

#endif
