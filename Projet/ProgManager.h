#ifndef PROGMANAGER_H
#define PROGMANAGER_H

#include "Calendar.h"

#include <QHash>

class ProgManager : public Singleton<ProgManager>{
private:
    ListEvent programmations;
    QHash<const TacheUnitaire*, QTime> tabDuree;

    friend class TacheUnitaire;

    void ajouterProgrammation(Evenement& E);
public:
    ~ProgManager();

    void ajouterProgrammationT(const QDate& d, const QTime& h, const QTime& fin, const TacheUnitaire& TU);
    void ajouterProgrammationA(const QDate& d, const QTime& h, const QTime& fin, const QString& t, const QString& desc, const QString& l);

    void supprimerProgrammationsT(const TacheUnitaire& TU);

    bool programmationExists(const QDate& d, const QTime& h, const QTime& fin) const;
    bool progTacheExists(const TacheUnitaire& TU) const;

    QList<ProgrammationTache*> getProgsT(const TacheUnitaire& TU) const;

    ProgrammationActivite* getProgA(const QString titre);
    const ProgrammationActivite* getProgA(const QString titre) const;

    bool isPrecedenceProgramme(const QDate& d, const QTime& h, const Tache& pred) const;

    void updateDuree(const TacheUnitaire& TU, QTime d);

    ListEventConst getProgWeek(const QDate& jour) const;
    ListEventConst getProgProj(const Projet& projet) const;

    void loadEvts(QXmlStreamReader& xml, QDate jour = QDate(), const Projet* projet = 0);
    void loadDurees(QXmlStreamReader& xml);
    void save(QXmlStreamWriter& xml, QDate jour = QDate(), const Projet* projet = 0) const;

    bool isEmpty() const { return programmations.isEmpty(); }

    Iterator<ProgManager, Evenement> begin(){ return Iterator<ProgManager, Evenement>(programmations.begin()); }
    Iterator<ProgManager, Evenement> end(){ return Iterator<ProgManager, Evenement>(programmations.end()); }

    const_Iterator<ProgManager, Evenement> begin() const { return const_Iterator<ProgManager, Evenement>(programmations.begin()); }
    const_Iterator<ProgManager, Evenement> end() const { return const_Iterator<ProgManager, Evenement>(programmations.end()); }

    HashIterator<ProgManager, TacheUnitaire, QTime> tabDureeBegin(){ return HashIterator<ProgManager, TacheUnitaire, QTime>(tabDuree.begin()); }
    HashIterator<ProgManager, TacheUnitaire, QTime> tabDureeEnd(){ return HashIterator<ProgManager, TacheUnitaire, QTime>(tabDuree.end()); }

    const_HashIterator<ProgManager, TacheUnitaire, QTime> tabDureeBegin() const { return const_HashIterator<ProgManager, TacheUnitaire, QTime>(tabDuree.begin()); }
    const_HashIterator<ProgManager, TacheUnitaire, QTime> tabDureeEnd() const { return const_HashIterator<ProgManager, TacheUnitaire, QTime>(tabDuree.end()); }
};

#endif // PROGMANAGER_H
