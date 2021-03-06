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
#include <QTimeEdit>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
#include <QRadioButton>

#include "Calendar.h"


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
    QTimeEdit *duree;
    QDateEdit *dispo, *echeance;
    QListWidget *predecesseurs, *composantes;
    QPushButton *sauver;

    TacheUnitaire* tacheU;
    TacheComposite* tacheC;

public slots:
    void activerSauver(){ sauver->setEnabled(true); }
    void sauverTache();
    void initialiserEditeur(QString);
    void updateT();

public:
    explicit TacheEditeur(QWidget* parent = 0);
};

#endif
