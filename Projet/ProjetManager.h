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

    bool isProjetExistant(const QString& titre) const { return trouverProjet(titre)!=0; }
    bool isTacheInProjet(const Tache& t);

    Projet& getProjet(const QString& titre);
    const Projet& getProjet(const QString& titre) const;

    Projet* getProjet(const Tache& t);
    const Projet* getProjet(const Tache& t)const;

    void load1(QXmlStreamReader& xml);
    void load2(QXmlStreamReader& xml);
    void save(QXmlStreamWriter& xml);

    class iterator{
        ListProjet::iterator current;
        iterator(ListProjet::iterator u):current(u){}
        friend class ProjetManager;
    public:
        iterator(){}
        Projet& operator*() const { return **current; }
        bool operator!=(iterator it) const { return current != it.current; }
        iterator& operator++(){ ++current ; return *this; }
    };

    iterator begin(){ return iterator(projets.begin()); }
    iterator end(){ return iterator(projets.end()); }


    class const_iterator{
        ListProjet::const_iterator current;
        const_iterator(ListProjet::const_iterator u):current(u){}
        friend class ProjetManager;
    public:
        const_iterator(){}
        const Projet& operator*() const { return **current; }
        bool operator!=(const_iterator it) const { return current != it.current; }
        const_iterator& operator++(){ ++current; return *this; }

    };
    const_iterator begin() const { return const_iterator(projets.begin()); }
    const_iterator end() const { return const_iterator(projets.end()); }

    class tabParentIterator{
        QHash<const Tache*, const Projet*>::iterator current;
        tabParentIterator(QHash<const Tache*, const Projet*>::iterator u):current(u){}
        friend class ProjetManager;
    public:
        tabParentIterator(){}
        QHash<const Tache*, const Projet*>::iterator operator*() const { return current; }
        bool operator!=(tabParentIterator it) const { return current != it.current; }
        tabParentIterator& operator++(){ ++current ; return *this; }
    };
    tabParentIterator tabParentBegin(){ return tabParentIterator(tabParent.begin()); }
    tabParentIterator tabParentEnd(){ return tabParentIterator(tabParent.end()); }
};

#endif // PROJETMANAGER_H
