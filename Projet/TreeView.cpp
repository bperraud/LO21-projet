#include "TreeView.h"
#include "TacheManager.h"
#include "ProgManager.h"

TreeView::TreeView(QWidget *parent) : QWidget(parent){

    setMinimumHeight(800);

    QFont f; f.setBold(true);

    // treeViewProjets
    treeViewP = new QTreeView(this);
    modelP = new QStandardItemModel(this);
    treePLabels << "Projets et tâches associées";

    // treeViewTaches
    treeViewT = new QTreeView(this);
    modelT = new QStandardItemModel(this);
    treeTLabels << "Tâches hors projet";

    // treeViewActivitees
    treeViewA = new QTreeView(this);
    treeViewA->setSortingEnabled(true);
    modelA = new QStandardItemModel(this);
    treeALabels << "Type" << "Titre" << "Description" << "Lieu" << "Date" << "Début" << "Fin" << "Préemptive";

    progLabel = new QLabel("Liste des programmations :", this);
    progLabel->setFont(f);

    // Initialisation

    updateTrees();

    treeViewP->setModel(modelP);
    treeViewT->setModel(modelT);
    treeViewA->setModel(modelA);

    // Informations

    infoTacheLabel = new QLabel("Informations pour la sélection :", this);
    infoTacheLabel->setFont(f);
    infoTacheLabel->setStyleSheet("margin-top:6px;margin-bottom:6px;");

    infoTache = new QLabel("Sélection vide", this);

    // Layout & Scroll

    scrollareaP = new QScrollArea;
    scrollareaT = new QScrollArea;
    scrollareaA = new QScrollArea;
    scrollareaS = new QScrollArea;

    scrollareaP->setWidget(treeViewP);
    scrollareaP->setWidgetResizable(true);
    scrollareaP->setFixedHeight(160);

    scrollareaT->setWidget(treeViewT);
    scrollareaT->setWidgetResizable(true);
    scrollareaT->setFixedHeight(160);

    scrollareaA->setWidget(treeViewA);
    scrollareaA->setWidgetResizable(true);
    scrollareaA->setFixedHeight(160);

    scrollareaS->setWidget(infoTache);
    scrollareaS->setWidgetResizable(true);
    scrollareaS->setFixedHeight(160);
    scrollareaS->setStyleSheet("padding:6px;");

    layout = new QVBoxLayout;


    layout->addWidget(scrollareaP);
    layout->addWidget(scrollareaT);
    layout->addWidget(progLabel);
    layout->addWidget(scrollareaA);
    layout->addWidget(infoTacheLabel);
    layout->addWidget(scrollareaS);
    layout->addStretch();



    this->setLayout(layout);

    // Signaux

    QObject::connect(treeViewP, SIGNAL(clicked(const QModelIndex&)), this, SLOT(afficherInfos(const QModelIndex&)));
    QObject::connect(treeViewT, SIGNAL(clicked(const QModelIndex&)), this, SLOT(afficherInfos(const QModelIndex&)));
    QObject::connect(treeViewA, SIGNAL(clicked(const QModelIndex&)), this, SLOT(afficherInfos(const QModelIndex&)));
}

TreeView::~TreeView(){}

void TreeView::ajouterProjetTree(QStandardItem* root, const Projet& projet){
    QStandardItem* newItem =  new QStandardItem(projet.getTitre());
    root->appendRow(newItem);
    newItem->setEditable(false);
    newItem->setData(QVariant::fromValue(projet.getTitre()));
}

void TreeView::ajouterTacheTree(QStandardItem* pere, const Tache& tache){
    QStandardItem* newItem = new QStandardItem(tache.getTitre());
    pere->appendRow(newItem);
    newItem->setEditable(false);
    if(!tache.isTacheUnitaire()){
        const TacheComposite& TC = dynamic_cast<const TacheComposite&>(tache);
        if (!TC.getSousTaches().isEmpty()){
            for (int i = 0; i < TC.getSousTaches().size(); ++i)
                ajouterTacheTree(newItem, *TC.getSousTaches()[i]);
        }
    }
}

void TreeView::ajouterActiviteTree(QStandardItem* root, const Evenement& evt){
    QList<QStandardItem*> row;
    QStandardItem* item;
    if (!evt.isProgTache()){
        const ProgrammationActivite& progA = dynamic_cast<const ProgrammationActivite&>(evt);
            item = new QStandardItem("Activité");
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progA.getTitre());
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progA.getDescription());
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progA.getLieu());
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progA.getDate().toString("dd/MM/yyyy"));
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progA.getHoraire().toString("hh'h'mm"));
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progA.getHoraireFin().toString("hh'h'mm"));
            item->setEditable(false);
            row.append(item);
    }
    else{
        const ProgrammationTache& progT = dynamic_cast<const ProgrammationTache&>(evt);
            item = new QStandardItem("Tâche");
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progT.getTitre());
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progT.getDescription());
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem();
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progT.getDate().toString("dd/MM/yyyy"));
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progT.getHoraire().toString("hh'h'mm"));
            item->setEditable(false);
            row.append(item);
            item = new QStandardItem(progT.getHoraireFin().toString("hh'h'mm"));
            item->setEditable(false);
            row.append(item);
            if (progT.getTache().isPreemptive())
                item = new QStandardItem("Oui");
            else item = new QStandardItem("Non");
            item->setEditable(false);
            row.append(item);
    }
    root->appendRow(row);
}

void TreeView::afficherInfos(const QModelIndex& index){
    QString infos;
    if ((index.model() == modelP && index.parent().isValid()) || index.model() == modelT){
        QString titre = index.data().toString();
        infos.append("Tâche ");
        const Tache& tache = TacheManager::getInstance()->getTache(titre);
        if (tache.isTacheUnitaire()) infos.append("unitaire :\n\n");
        else infos.append("composite :\n\n");
        tache.ajouterInfos(infos);
    }
    else if (index.model() == modelP){
        QString titre = index.data().toString();
        infos.append("Projet :\n\n");
        const Projet& projet = ProjetManager::getInstance()->getProjet(titre);
        projet.ajouterInfos(infos);
    }
    else{
        infos.append("Cf. ci-dessus pour les détails de la programmation sélectionnée.");
    }
    infoTache->setText(infos);
}

void TreeView::updateTrees(){
    TacheManager& TM = *TacheManager::getInstance();
    ProjetManager& PM = *ProjetManager::getInstance();
    ProgManager& ProgM = *ProgManager::getInstance();

    // Cleaning
    modelP->clear();
    modelT->clear();
    modelA->clear();
    treeViewP->clearSelection();
    treeViewT->clearSelection();
    treeViewA->clearSelection();

    // Re-init
    modelP->setHorizontalHeaderLabels (QStringList(treePLabels));
    rootNodeP = modelP->invisibleRootItem();
    modelT->setHorizontalHeaderLabels (QStringList(treeTLabels));
    rootNodeT = modelT->invisibleRootItem();
    modelA->setHorizontalHeaderLabels (QStringList(treeALabels));
    rootNodeA = modelA->invisibleRootItem();

    // Construction de treeViewP et treeViewT
    for (Iterator<ProjetManager, Projet> i = PM.begin(); i != PM.end(); ++i){
        ajouterProjetTree(rootNodeP, *i);
    }

    for (Iterator<TacheManager, Tache> i = TM.begin(); i != TM.end(); ++i){
        if (!TM.getTacheMere(*i)){
            Projet* P = PM.getProjet(*i);
            if (P) ajouterTacheTree(modelP->findItems(P->getTitre()).first(), *i);
            else ajouterTacheTree(rootNodeT, *i);
        }
    }

    // Construction de treeViewA
    for (Iterator<ProgManager, Evenement> i = ProgM.begin(); i != ProgM.end(); ++i)
        ajouterActiviteTree(rootNodeA, *i);

    treeViewP->expandAll();
    treeViewT->expandAll();
}
