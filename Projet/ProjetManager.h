#ifndef PROJETMANAGER_H
#define PROJETMANAGER_H

#include "Calendar.h"

#include <QHash>

class ProjetManager : public Singleton<ProjetManager>{
private:
    ListProjet projets;
    QHash<const Tache*, const Projet*> tabParent;

    friend ListTachesConst Projet::getTaches() const;
    friend void Projet::setTaches(const ListTaches& T);
    friend void Projet::addTache(const Tache* t);
    friend void Projet::rmTache(const Tache* t);
    Projet* trouverProjet(const QString& titre) const;
public:
    ~ProjetManager();

    Projet& ajouterProjet(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const ListTaches& Taches=ListTaches());
    void deleteProjet(const QString& str);

    bool isProjetExistant(const QString& titre) const { return trouverProjet(titre)!=0; }
    bool isTacheInProjet(const Tache& t);

    Projet& getProjet(const QString& titre);
    const Projet& getProjet(const QString& titre) const;

    Projet* getProjet(const Tache& t);
    const Projet* getProjet(const Tache& t)const;

    void loadProjets(QXmlStreamReader& xml);
    void loadHierarchie(QXmlStreamReader& xml);
    void save(QXmlStreamWriter& xml) const;

    bool isEmpty() const { return projets.isEmpty(); }

    Iterator<ProjetManager, Projet> begin(){ return Iterator<ProjetManager, Projet>(projets.begin()); }
    Iterator<ProjetManager, Projet> end(){ return Iterator<ProjetManager, Projet>(projets.end()); }

    const_Iterator<ProjetManager, Projet> begin() const { return const_Iterator<ProjetManager, Projet>(projets.begin()); }
    const_Iterator<ProjetManager, Projet> end() const { return const_Iterator<ProjetManager, Projet>(projets.end()); }

    HashIterator<ProjetManager, Tache, Projet> tabParentBegin(){ return HashIterator<ProjetManager, Tache, Projet>(tabParent.begin()); }
    HashIterator<ProjetManager, Tache, Projet> tabParentEnd(){ return HashIterator<ProjetManager, Tache, Projet>(tabParent.end()); }

    const_HashIterator<ProjetManager, Tache, Projet> tabParentBegin() const { return const_HashIterator<ProjetManager, Tache, Projet>(tabParent.begin()); }
    const_HashIterator<ProjetManager, Tache, Projet> tabParentEnd() const { return const_HashIterator<ProjetManager, Tache, Projet>(tabParent.end()); }
};

#endif // PROJETMANAGER_H
