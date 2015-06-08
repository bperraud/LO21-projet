#ifndef TACHEMANAGER_H
#define TACHEMANAGER_H

#include "Calendar.h"
#include "ProjetManager.h"

#include <QHash>

class TacheManager : public Singleton<TacheManager>{
private:
    ListTaches taches;
    //QHash<QString, QString> tabParent;
    QHash<const Tache*, const TacheComposite*> tabParent;
    QString file;

    friend ListTachesConst TacheComposite::getSousTaches() const;
    friend void TacheComposite::setSousTaches(const ListTaches& sT);
    friend void TacheComposite::addSousTache(const Tache* t) const;
    friend void TacheComposite::rmSousTache(const Tache* t);
    friend bool ProjetManager::isTacheInProjet(const Tache& t);
    friend void LoadXML::load(const QString& f);

    Tache* trouverTache(const QString& titre) const;
    void ajouterTache(Tache& T);
public:
    ~TacheManager();

    TacheUnitaire& ajouterTacheUnitaire(const QString& t, const QString& desc, const QTime& dur, const QDate& dispo, const QDate& deadline, bool preempt=false);
    TacheComposite& ajouterTacheComposite(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const ListTaches& sT=ListTaches());

    bool isTacheExistante(const QString& titre) const { return trouverTache(titre)!=0; }

    Tache& getTache(const QString& titre);
    const Tache& getTache(const QString& titre) const;

    Tache* getTacheMere(const Tache& t);
    const Tache* getTacheMere(const Tache& t) const;

    void load(const QString& f);
    void save(const QString& f);

    void load1(QXmlStreamReader& xml);
    void load2(QXmlStreamReader& xml);
    void save1(QXmlStreamWriter& xml);

    class iterator{
        ListTaches::iterator current;
        iterator(ListTaches::iterator u):current(u){}
        friend class TacheManager;
    public:
        iterator(){}
        Tache& operator*() const { return **current; }
        bool operator!=(iterator it) const { return current != it.current; }
        iterator& operator++(){ ++current ; return *this; }
    };
    iterator begin(){ return iterator(taches.begin()); }
    iterator end(){ return iterator(taches.end()); }


    class const_iterator{
        ListTaches::const_iterator current;
        const_iterator(ListTaches::const_iterator u):current(u){}
        friend class TacheManager;
    public:
        const_iterator(){}
        const Tache& operator*() const { return **current; }
        bool operator!=(const_iterator it) const { return current != it.current; }
        const_iterator& operator++(){ ++current; return *this; }

    };
    const_iterator begin() const { return const_iterator(taches.begin()); }
    const_iterator end() const { return const_iterator(taches.end()); }

    class tabParentIterator{
        QHash<const Tache*, const TacheComposite*>::iterator current;
        tabParentIterator(QHash<const Tache*, const TacheComposite*>::iterator u):current(u){}
        friend class TacheManager;
    public:
        tabParentIterator(){}
        QHash<const Tache*, const TacheComposite*>::iterator operator*() const { return current; }
        bool operator!=(tabParentIterator it) const { return current != it.current; }
        tabParentIterator& operator++(){ ++current ; return *this; }
    };
    tabParentIterator tabParentBegin(){ return tabParentIterator(tabParent.begin()); }
    tabParentIterator tabParentEnd(){ return tabParentIterator(tabParent.end()); }
};

#endif // TACHEMANAGER_H

