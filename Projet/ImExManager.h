#ifndef IMEXMANAGER_H
#define IMEXMANAGER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QPushButton>
#include <QButtonGroup>
#include <QRadioButton>
#include <QComboBox>
#include <QDateEdit>

#include "Calendar.h"

class ImExManager : public QWidget{
Q_OBJECT
private:
    //QFileDialog

    LoadStrategy* loadStrategy;
    SaveStrategy* saveStrategy;

    QButtonGroup *formatGroup, *actionGroup;
    QRadioButton *xmlRadio, *txtRadio, *loadRadio, *saveRadio;

    QLabel *formatLabel, *actionLabel, *pathLabel, *weekLabel, *projLabel;
    QLineEdit *pathEdit;
    QString path, format;
    QPushButton *findButton, *validerAll, *validerWeek, *validerProj;

    QDateEdit *jour;
    QComboBox *projets;

    QHBoxLayout *HL1, *HL2, *HL3, *HL4;
    QVBoxLayout *VL;

    void switchStrategy(LoadStrategy* s){
        if (loadStrategy) delete loadStrategy;
        loadStrategy = s;
    }
    void switchStrategy(SaveStrategy* s){
        if (saveStrategy) delete saveStrategy;
        saveStrategy = s;
    }
    void chooseAction(QString type){ actionGroup->checkedButton() == loadRadio ? load(type) : save(type); }
    void load(QString type);
    void save(QString type);

public:
    explicit ImExManager(QWidget *parent = 0);
    ~ImExManager();

public slots:
    void updateProj();
    void updateFormat(QAbstractButton* radioB);
    void findFile();
    void loadSaveAll(){ chooseAction("all"); }
    void loadSaveWeek(){ chooseAction("week"); }
    void loadSaveProj(){ chooseAction("proj"); }
};

#endif // IMEXMANAGER_H


