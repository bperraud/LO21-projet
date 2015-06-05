#ifndef PROGMANAGER_H
#define PROGMANAGER_H

#include "Calendar.h"

#include <QHash>

class ProgManager : public Singleton<ProgManager>{
private:
    ListEvent programmations;
    QHash<QString, QTime> tabDuree;

    friend class TacheUnitaire;

    void ajouterProgrammation(Evenement& E);
public:
    void ajouterProgrammationT(const QDate& d, const QTime& h, const QTime& fin, const TacheUnitaire& TU);
    void ajouterProgrammationA(const QDate& d, const QTime& h, const QTime& fin, const QString& t, const QString& desc, const QString& l);

    bool programmationExists(const QDate& d, const QTime& h, const QTime& fin);

    ProgrammationTache* trouverProgrammationT(const TacheUnitaire& TU) const;
    ProgrammationActivite* trouverProgrammationA(const ProgrammationActivite& PA) const;

    void updateDuree(const TacheUnitaire& TU, QTime d);


    class iterator{
        ListEvent::iterator current;
        iterator(ListEvent::iterator u):current(u){}
        friend class ProgManager;
    public:
        iterator(){}
        Evenement& operator*() const { return **current; }
        bool operator!=(iterator it) const { return current != it.current; }
        iterator& operator++(){ ++current ; return *this; }
    };

    iterator begin(){ return iterator(programmations.begin()); }
    iterator end(){ return iterator(programmations.end()); }

    class const_iterator{
        ListEvent::const_iterator current;
        const_iterator(ListEvent::const_iterator u):current(u){}
        friend class ProgManager;
    public:
        const_iterator(){}
        const Evenement& operator*() const { return **current; }
        bool operator!=(const_iterator it) const { return current != it.current; }
        const_iterator& operator++(){ ++current; return *this; }

    };

    const_iterator begin() const { return const_iterator(programmations.begin()); }
    const_iterator end() const { return const_iterator(programmations.end()); }

};

#endif // PROGMANAGER_H
