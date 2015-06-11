#ifndef TACHEMANAGER_H
#define TACHEMANAGER_H

#include "Calendar.h"
#include "ProjetManager.h"
#include "ProgManager.h"

#include <QHash>

class TacheManager : public Singleton<TacheManager>{
private:
    ListTaches taches;
    QHash<const Tache*, const TacheComposite*> tabParent;

    friend ListTachesConst TacheComposite::getSousTaches() const;
    friend void TacheComposite::setSousTaches(const ListTaches& sT);
    friend void TacheComposite::addSousTache(const Tache* t) const;
    friend void TacheComposite::rmSousTache(const Tache* t);
    friend bool ProjetManager::isTacheInProjet(const Tache& t);
    friend Projet* ProjetManager::getProjet(const Tache& t);
    friend void LoadXML::load(const QString& f);
    friend void LoadXML::load(const QString& f, QDate jour);
    friend void LoadXML::load(const QString& f, const Projet* projet);
    friend bool ProgManager::isPrecedenceProgramme(const QDate& d, const QTime& h, const Tache& pred) const;

    Tache* trouverTache(const QString& titre) const;
    void ajouterTache(Tache& T);
public:
    ~TacheManager();

    TacheUnitaire& ajouterTacheUnitaire(const QString& t, const QString& desc, const QTime& dur, const QDate& dispo, const QDate& deadline, bool preempt=false);
    TacheComposite& ajouterTacheComposite(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const ListTaches& sT=ListTaches());

    void deleteTache(const QString& str);

    bool isTacheExistante(const QString& titre) const { return trouverTache(titre)!=0; }

    Tache& getTache(const QString& titre);
    const Tache& getTache(const QString& titre) const;

    Tache* getTacheMere(const Tache& t);
    const Tache* getTacheMere(const Tache& t) const;

    void loadTaches(QXmlStreamReader& xml);
    void loadHierarchie(QXmlStreamReader& xml);
    void save(QXmlStreamWriter& xml) const;

    bool isEmpty() const { return taches.isEmpty(); }

    Iterator<TacheManager, Tache> begin(){ return Iterator<TacheManager, Tache>(taches.begin()); }
    Iterator<TacheManager, Tache> end(){ return Iterator<TacheManager, Tache>(taches.end()); }

    const_Iterator<TacheManager, Tache> begin() const { return const_Iterator<TacheManager, Tache>(taches.begin()); }
    const_Iterator<TacheManager, Tache> end() const { return const_Iterator<TacheManager, Tache>(taches.end()); }

    HashIterator<TacheManager, Tache, TacheComposite> tabParentBegin(){ return HashIterator<TacheManager, Tache, TacheComposite>(tabParent.begin()); }
    HashIterator<TacheManager, Tache, TacheComposite> tabParentEnd(){ return HashIterator<TacheManager, Tache, TacheComposite>(tabParent.end()); }

    const_HashIterator<TacheManager, Tache, TacheComposite> tabParentBegin() const { return const_HashIterator<TacheManager, Tache, TacheComposite>(tabParent.begin()); }
    const_HashIterator<TacheManager, Tache, TacheComposite> tabParentEnd() const { return const_HashIterator<TacheManager, Tache, TacheComposite>(tabParent.end()); }
};

#endif // TACHEMANAGER_H

