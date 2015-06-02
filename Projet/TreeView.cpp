#include "TreeView.h"
#include "TacheManager.h"

TreeView::TreeView(QWidget *parent) : QWidget(parent){

    setMinimumHeight(600);

    TacheManager& TM = *TacheManager::getInstance();
    ProjetManager& PM = *ProjetManager::getInstance();

    // treeViewProjets
    treeViewP = new QTreeView(this);
    modelP = new QStandardItemModel(this);
    treePLabels << "Projets et tâches associées";
    modelP->setHorizontalHeaderLabels (QStringList(treePLabels));
    rootNodeP = modelP->invisibleRootItem();

    // treeViewTaches
    treeViewT = new QTreeView(this);
    modelT = new QStandardItemModel(this);
    treeTLabels << "Tâches hors projet";
    modelT->setHorizontalHeaderLabels (QStringList(treeTLabels));
    rootNodeT = modelT->invisibleRootItem();

    // Construction de l'arbre
    for (ProjetManager::iterator i = PM.begin(); i != PM.end(); ++i)
        ajouterProjetTree(rootNodeP, *i);
    for (TacheManager::iterator i = TM.begin(); i != TM.end(); ++i){
        if (!TM.getTacheMere(*i)){
            Projet* P = PM.getProjet(*i);
            if (P) ajouterTacheTree(modelP->findItems(P->getTitre()).first(), *i);
            else ajouterTacheTree(rootNodeT, *i);
        }
    }

    treeViewP->setModel(modelP);
    treeViewP->expandAll();
    treeViewT->setModel(modelT);
    treeViewT->expandAll();

    // Informations

    infoTacheLabel = new QLabel("Informations pour la sélection :", this);
    QFont f;
    f.setFamily("Times");
    f.setBold(true);
    infoTacheLabel->setFont(f);
    infoTacheLabel->setIndent(24);

    infoTache = new QLabel("Sélection vide", this);

    // Layout & Scroll

    scrollareaP = new QScrollArea;
    scrollareaT = new QScrollArea;

    scrollareaP->setWidget(treeViewP);
    scrollareaP->setWidgetResizable(true);
    scrollareaP->setFixedHeight(200);

    scrollareaT->setWidget(treeViewT);
    scrollareaT->setWidgetResizable(true);
    scrollareaT->setFixedHeight(200);

    layout = new QVBoxLayout;

    layout->addWidget(scrollareaP);
    layout->addWidget(scrollareaT);
    layout->addWidget(infoTacheLabel);
    layout->addWidget(infoTache);
    layout->addStretch();

    this->setLayout(layout);

    // Signaux

    QObject::connect(treeViewP, SIGNAL(clicked(const QModelIndex&)), this, SLOT(afficherInfos(const QModelIndex&)));
    QObject::connect(treeViewT, SIGNAL(clicked(const QModelIndex&)), this, SLOT(afficherInfos(const QModelIndex&)));
}

TreeView::~TreeView(){}

void TreeView::ajouterProjetTree(QStandardItem* root, Projet& projet){
    QStandardItem* newItem =  new QStandardItem(projet.getTitre());
    root->appendRow(newItem);
    newItem->setEditable(false);
    newItem->setData(QVariant::fromValue(projet.getTitre()));
}

void TreeView::ajouterTacheTree(QStandardItem* pere, Tache& tache){
    QStandardItem* newItem =  new QStandardItem(tache.getTitre());
    pere->appendRow(newItem);
    newItem->setEditable(false);
    if(!tache.isTacheUnitaire()){
        TacheComposite& TC = dynamic_cast<TacheComposite&>(tache);
        if (!TC.getSousTaches().isEmpty()){
            for (int i = 0; i < TC.getSousTaches().size(); ++i)
                ajouterTacheTree(newItem, *TC.getSousTaches()[i]);
        }
    }
}

void TreeView::afficherInfos(const QModelIndex& index){
    QString titre = index.data().toString();
    QString infos;
    if (index.parent().isValid() || index.model() == modelT){
        infos.append("Tâche ");
        const Tache& tache = TacheManager::getInstance()->getTache(titre);
        if (tache.isTacheUnitaire()) infos.append("unitaire :\n");
        else infos.append("composite :\n");
        tache.ajouterInfos(infos);
    }
    else{
        infos.append("Projet :\n");
        const Projet& projet = ProjetManager::getInstance()->getProjet(titre);
        projet.ajouterInfos(infos);
    }
    infoTache->setText(infos);
}

