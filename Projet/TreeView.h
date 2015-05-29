#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QVariant>
#include <QTreeView>

#include "Calendar.h"
#include "TacheManager.h"

class TreeView : public QWidget{
Q_OBJECT

private:
    QScrollArea *scrollareaP, *scrollareaT;
    QTreeView *treeViewP, *treeViewT;
    QStandardItemModel *modelP, *modelT;
    QList<QString> treePLabels, treeTLabels;
    QStandardItem *rootNodeP, *rootNodeT;
    QLabel *infoTacheLabel, *infoTache;

    QVBoxLayout* layout;

public:
    explicit TreeView(QWidget *parent = 0);
    ~TreeView();
    void ajouterProjetTree(QStandardItem* root, Projet& projet);
    void ajouterTacheTree(QStandardItem* pere, Tache& tache);

public slots:
    void afficherInfos(const QModelIndex& index);
};

#endif // TREEVIEW_H
