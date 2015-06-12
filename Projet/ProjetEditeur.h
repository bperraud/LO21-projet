#ifndef PROJETEDITEUR
#define PROJETEDITEUR

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

class ProjetEditeur : public QWidget{
Q_OBJECT

private:
    QVBoxLayout *VC;
    QHBoxLayout *HC0, *HC1, *HC2, *HC3, *HC4, *HC5;
    QLabel *ChoixProjetLabel, *titreLabel, *descriptionLabel, *dispoLabel, *echeanceLabel, *tachesLabel;
    QLineEdit *titre;
    QTextEdit *description;
    QDateEdit *dispo, *echeance;
    QListWidget *taches;
    QComboBox *ChoixProjet;
    QPushButton *editer;

    Projet *projet;

public slots:
    void activerEditer(){ editer->setEnabled(true); }
    void initialiserEditeur(QString);
    void updateProjEdit();
    void editerProjet();

public:
    explicit ProjetEditeur(QWidget* parent = 0);

};


#endif // PROJETEDITEUR

