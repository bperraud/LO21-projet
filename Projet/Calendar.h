#ifndef CALENDAR_H
#define CALENDAR_H

#include "CalendarException.h"
#include "Template.h"
#include "Strategies.h"

#include <QString>
#include <QDate>

#include <QDebug>
#include <QTextCodec>
#include <QtXml>

class Tache{
protected:
    QString titre;
    QString description;
    QDate disponibilite;
    QDate echeance;

private:
    Tache(const Tache& t);
    Tache& operator=(const Tache&);

public:
    Tache(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline):
            titre(t), description(desc), disponibilite(dispo), echeance(deadline){}
    virtual ~Tache(){}

    QString getTitre() const { return titre; }
    void setTitre(const QString& str);
    QString getDescription() const { return description; }
    void setDescription(const QString& str) { description=str; }
    QDate getDateDisponibilite() const { return disponibilite; }
    QDate getDateEcheance() const { return echeance; }
    void setDatesDisponibiliteEcheance(const QDate& disp, const QDate& e){
        if (e<disp) throw CalendarException("erreur tâche : date échéance < date disponibilité");
        disponibilite=disp; echeance=e;
    }

    virtual void ajouterInfos(QString& infos) const;

    void save(QXmlStreamWriter& stream) const;

    virtual void saveTache(QXmlStreamWriter& stream) const =0;
    virtual bool isTacheUnitaire() const =0;
};

typedef QList<Tache*> ListTaches;
typedef QList<const Tache*> ListTachesConst;

class TacheUnitaire : public Tache{
private:
    QTime duree;
    bool preemptive;
    TacheUnitaire(const QString& t, const QString& desc, const QTime& dur, const QDate& dispo, const QDate& deadline, bool preempt=false):
            Tache(t, desc, dispo, deadline), duree(dur), preemptive(preempt){
        if (!preempt && dur.hour()>12) throw CalendarException("erreur tâche unitaire non preemptive et durée > 12h");
    }
    friend class TacheManager;
public:
    QTime getDuree() const { return duree; }
    QTime getDureeRestante() const;
    void setDuree(const QTime& d) { duree=d; }
    bool isPreemptive() const { return preemptive; }
    void setPreemptive(bool b = true) { preemptive=b; }

    void ajouterInfos(QString& infos) const;

    void saveTache(QXmlStreamWriter& stream) const;
    bool isTacheUnitaire() const { return true; }
};

class TacheComposite : public Tache{
private:
    TacheComposite(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline):
            Tache(t, desc, dispo, deadline){}
    friend class TacheManager;
public:
    ListTachesConst getSousTaches() const;
    void setSousTaches(const ListTaches& sT);
    void addSousTache(const Tache* t) const;
    void rmSousTache(const Tache* t);

    void ajouterInfos(QString& infos) const;

    void saveTache(QXmlStreamWriter&) const {}
    bool isTacheUnitaire() const { return false; }
};


class Precedence{
private:
    const Tache* pred;
    const Tache* succ;
    Precedence(const Tache& i, const Tache& j): pred(&i), succ(&j){}
    friend class PrecedenceManager;
public:
    const Tache& getPredecesseur() const { return *pred;}
    const Tache& getSuccesseur() const { return *succ;}

    void save(QXmlStreamWriter& stream) const;
};


class Projet{
private:
    QString titre;
    QString description;
    QDate disponibilite;
    QDate echeance;
    Projet(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline):
            titre(t), description(desc), disponibilite(dispo), echeance(deadline){}
    friend class ProjetManager;
public:
    QString getTitre() const { return titre; }
    void setTitre(const QString& str);
    QString getDescription() const { return description; }
    void setDescription(const QString& str) { description = str; }
    QDate getDateDisponibilite() const { return disponibilite; }
    QDate getDateEcheance() const { return echeance; }
    void setDatesDisponibiliteEcheance(const QDate& disp, const QDate& e);

    ListTachesConst getTaches() const;

    void setTaches(const ListTaches& T);
    void addTache(const Tache* t);
    void rmTache(const Tache* t);

    void ajouterInfos(QString& infos) const;

    void save(QXmlStreamWriter& stream) const;
};

typedef QList<Projet*> ListProjet;


class Evenement{
private:
    QDate date;
    QTime horaire;
    QTime horaireFin;

public:
    Evenement(const QDate& d, const QTime& h, const QTime& fin): date(d), horaire(h), horaireFin(fin){}
    virtual ~Evenement(){}

    QDate getDate() const { return date; }
    QTime getHoraire() const { return horaire; }
    QTime getHoraireFin() const { return horaireFin; }

    void save(QXmlStreamWriter& stream) const;

    virtual void saveEvt(QXmlStreamWriter& stream) const =0;

    virtual bool isProgTache() const =0;
    virtual QString getTitre() const =0;
    virtual QString getDescription() const =0;
};

typedef QList<Evenement*> ListEvent;
typedef QList<const Evenement*> ListEventConst;

class ProgrammationActivite : public Evenement{
private:
    QString titre;
    QString description;
    QString lieu;

    ProgrammationActivite(const QDate& d, const QTime& h, const QTime& fin, const QString& t, const QString& desc, const QString& l)
        :Evenement(d, h, fin), titre(t), description(desc), lieu(l){}
    friend class ProgManager;
public:

    QString getLieu() const { return lieu; }

    void saveEvt(QXmlStreamWriter& stream) const;
    bool isProgTache() const { return false; }
    QString getTitre() const { return titre; }
    QString getDescription() const { return description; }
};

class ProgrammationTache : public Evenement{
private:
    const TacheUnitaire* tache;
    ProgrammationTache(const QDate& d, const QTime& h, const QTime& fin, const TacheUnitaire& t): Evenement(d, h, fin), tache(&t){}
    friend class ProgManager;
public:
    const TacheUnitaire& getTache() const { return *tache; }
    const Projet& getProjet() const;

    void saveEvt(QXmlStreamWriter& stream) const;
    virtual bool isProgTache() const { return true; }
    QString getTitre() const { return tache->getTitre(); }
    QString getDescription() const { return tache->getDescription(); }
};

#endif // CALENDAR_H
