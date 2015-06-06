#ifndef IMEXMANAGER_H
#define IMEXMANAGER_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFileDialog>
#include <QPushButton>

#include "Calendar.h"

class ImExManager : public QWidget{
Q_OBJECT
private:
    //QFileDialog

    LoadStrategy* loadStrategy;

    QLabel *loadLabel;
    QLineEdit *loadEdit;
    QString loadPath;
    QPushButton *loadButton;

    QHBoxLayout *HL;
    QVBoxLayout *VL;
public:
    explicit ImExManager(QWidget *parent = 0);
    ~ImExManager();
    void switchStrategy(LoadStrategy* s){
        if (loadStrategy) delete loadStrategy;
        loadStrategy = s;
    }

public slots:
    void loadXML();
    void load(){ loadStrategy->load(); }
};

#endif // IMEXMANAGER_H


