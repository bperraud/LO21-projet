#include "ImExManager.h"

ImExManager::ImExManager(QWidget *parent) : QWidget(parent){

    //setMinimumHeight(800);

    loadLabel = new QLabel("Charger un fichier xml :", this);
    loadButton = new QPushButton("Sélectionner le fichier", this);
    loadEdit = new QLineEdit(loadPath, this);

    // Layout

    HL = new QHBoxLayout;
    HL->addWidget(loadLabel);
    HL->addWidget(loadButton);
    HL->addWidget(loadEdit);
    HL->addStretch();
    VL = new QVBoxLayout;
    VL->addLayout(HL);
    VL->addStretch();


    this->setLayout(VL);

    // Signaux

    QObject::connect(loadButton, SIGNAL(clicked()), this, SLOT(loadXML()));

}

ImExManager::~ImExManager(){}

void ImExManager::loadXML(){
    loadPath = QFileDialog::getOpenFileName(this, "Charger un fichier xml", "/", "Fichiers XML (*.xml)");
    loadEdit->setText(loadPath);
}

