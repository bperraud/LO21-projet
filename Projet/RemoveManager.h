#ifndef REMOVEMANAGER
#define REMOVEMANAGER

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

class RemoveManager : public QWidget{
Q_OBJECT

private:
    QVBoxLayout *VC;
    QHBoxLayout *HC1, *HC2;
    QLabel *ChoixProjetLabel, *ChoixTacheLabel;
    QComboBox *ChoixProjet, *ChoixTache;
    QPushButton *supprimerP, *supprimerT;

public slots:
    void supprimerProjet();
    void supprimerTache();
    void updateRm();

public:
    explicit RemoveManager(QWidget* parent = 0);

};



#endif // REMOVEMANAGER
