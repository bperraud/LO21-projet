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

#include "Calendar.h"

class ImExManager : public QWidget{
Q_OBJECT
private:
    //QFileDialog

    LoadStrategy* loadStrategy;
    SaveStrategy* saveStrategy;

    QButtonGroup *formatGroup, *actionGroup;
    QRadioButton *xmlRadio, *txtRadio, *loadRadio, *saveRadio;

    QLabel *formatLabel, *actionLabel, *pathLabel;
    QLineEdit *pathEdit;
    QString path, format;
    QPushButton *findButton, *valider;

    QHBoxLayout *HL1, *HL2;
    QVBoxLayout *VL;

    void switchStrategy(LoadStrategy* s){
        if (loadStrategy) delete loadStrategy;
        loadStrategy = s;
    }
    void switchStrategy(SaveStrategy* s){
        if (saveStrategy) delete saveStrategy;
        saveStrategy = s;
    }
public:
    explicit ImExManager(QWidget *parent = 0);
    ~ImExManager();

public slots:
    void updateFormat(QAbstractButton* radioB);
    void findFile();
    void chooseAction(){ actionGroup->checkedButton() == loadRadio ? load() : save(); }
    void load();
    void save();
};

#endif // IMEXMANAGER_H


