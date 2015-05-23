#ifndef TACHEMANAGER_H
#define TACHEMANAGER_H

#include "Calendar.h"

class TacheManager : public Singleton<TacheManager>{
private:
    ListTaches taches;
    QString file;

    struct HierarchyTachesC{
        QString mere;
        QString fille;
        HierarchyTachesC(QString m, QString f):mere(m),fille(f){}
    };
    QList<HierarchyTachesC*> hierarchie;
    friend void TacheComposite::setSousTaches(const ListTaches& sT);
    friend void TacheComposite::addSousTache(const Tache* t);
    friend void TacheComposite::rmSousTache(const Tache* t);

    Tache* trouverTache(const QString& titre) const;
    void ajouterTache(Tache& T);
public:
    ~TacheManager();

    TacheUnitaire& ajouterTacheUnitaire(const QString& t, const QString& desc, const Duree& dur, const QDate& dispo, const QDate& deadline, bool preempt=false);
    TacheComposite& ajouterTacheComposite(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const ListTaches& sT=ListTaches());

    bool isTacheExistante(const QString& titre) const { return trouverTache(titre)!=0; }

    Tache& getTache(const QString& titre);
    const Tache& getTache(const QString& titre) const;

    void load(const QString& f);
    void save(const QString& f);


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




    /*
    class DisponibiliteFilterIterator {
        friend class TacheManager;
        Tache** currentTache;
        unsigned int nbRemain;
        QDate dispo;
        DisponibiliteFilterIterator(Tache** u, unsigned nb, const QDate& d):currentTache(u),nbRemain(nb),dispo(d){
            while(nbRemain>0 && dispo<(*currentTache)->getDateDisponibilite()){
                nbRemain--; currentTache++;
            }
        }
    public:
        //DisponibiliteFilterIterator():nbRemain(0),currentTache(0){}
        bool isDone() const { return nbRemain==0; }
        void next() {
            if (isDone())
                throw CalendarException("error, next on an iterator which is done");
            do {
                nbRemain--; currentTache++;
            }while(nbRemain>0 && dispo<(*currentTache)->getDateDisponibilite());
        }
        Tache& current() const {
            if (isDone())
                throw CalendarException("error, indirection on an iterator which is done");
            return **currentTache;
        }
    };
    DisponibiliteFilterIterator getDisponibiliteFilterIterator(const QDate& d) {
        return DisponibiliteFilterIterator(taches,nb,d);
    }*/
};

#endif // TACHEMANAGER_H

