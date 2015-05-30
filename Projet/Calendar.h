#ifndef CALENDAR_H
#define CALENDAR_H

#include "CalendarException.h"
#include "Duree.h"
#include "Singleton.h"

#include <typeinfo>

#include <QString>
#include <QDate>


#include <QDebug>
#include <QTextCodec>
#include <QtXml>


class TacheVisitor;


/* ----- [BEGIN] Design Pattern Composite ----- */

class Tache{ // Abstraite
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
    virtual void saveTache(QXmlStreamWriter& stream) const=0;
    virtual bool isTacheUnitaire() const =0;

    virtual void accept(TacheVisitor* v) =0;
};

QTextStream& operator<<(QTextStream& f, const Tache& t);

typedef QList<Tache*> ListTaches;

class TacheUnitaire : public Tache{
private:
    Duree duree;
    bool preemptive;
    TacheUnitaire(const QString& t, const QString& desc, const Duree& dur, const QDate& dispo, const QDate& deadline, bool preempt=false):
            Tache(t, desc, dispo, deadline), duree(dur), preemptive(preempt){
        if (!preempt && dur.getDureeEnHeures()>12) throw CalendarException("erreur tâche unitaire non preemptive et durée > 12h");
    }
    friend class TacheManager;
public:
    Duree getDuree() const { return duree; }
    void setDuree(const Duree& d) { duree=d; }
    bool isPreemptive() const { return preemptive; }
    void setPreemptive(bool b = true) { preemptive=b; }

    void ajouterInfos(QString& infos) const;

    void saveTache(QXmlStreamWriter& stream) const;
    bool isTacheUnitaire() const { return true; }
    void accept(TacheVisitor* v);
};


class TacheComposite : public Tache{
private:
    ListTaches sousTaches;
    TacheComposite(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const ListTaches& sT=ListTaches()):
            Tache(t, desc, dispo, deadline), sousTaches(sT){}
    friend class TacheManager;
public:
    const ListTaches& getSousTaches() const { return sousTaches; }
    void setSousTaches(const ListTaches& sT);
    void addSousTache(const Tache* t);
    void rmSousTache(const Tache* t);

    void ajouterInfos(QString& infos) const;

    void saveTache(QXmlStreamWriter& stream) const;
    bool isTacheUnitaire() const { return false; }
    void accept(TacheVisitor* v);
};

/* ----- [END] Design Pattern Composite ----- */





/* ----- [BEGIN] Précédences ----- */

class Precedence;

typedef QList<const Tache*> ListTachesConst;

class PrecedenceManager: public Singleton<PrecedenceManager> {
private:
    QList<Precedence*> precedences;
public:
    void ajouterPrecedence(const Tache& Tpred,const Tache& Tsucc);
    ListTachesConst trouverPrecedences(const Tache& Tsucc) const;
};

class Precedence {
private:
    const Tache* pred;
    const Tache* succ;
    Precedence(const Tache& i, const Tache& j): pred(&i), succ(&j){}
public:
    const Tache& getPredecesseur() const { return *pred;}
    const Tache& getSuccesseur() const { return *succ;}
    friend void PrecedenceManager::ajouterPrecedence(const Tache& Tpred, const Tache& Tsucc);
};






/* ----- [END] Précédences ----- */







/* ----- [BEGIN]Design Pattern Visitor ----- */

class TacheVisitor{
public:
    virtual void visitTacheUnitaire(TacheUnitaire* TU) =0;
    virtual void visitTacheComposite(TacheComposite* TC) =0;
};

class TacheInformateur : public TacheVisitor{
public:
    void visitTacheUnitaire(TacheUnitaire* TU);
    void visitTacheComposite(TacheComposite* TC);
};

/* ----- [END]Design Pattern Visitor ----- */




class Evenement{ // Abstraite
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

    virtual bool isProgTache() const =0;
    virtual QString getTitre() const =0;
};


class ProgrammationActivite : public Evenement{
private:
    QString titre;
    QString description;
    QString lieu;

    ProgrammationActivite(const QDate& d, const QTime& h, const QTime& fin, const QString& t, const QString& desc, const QString& l)
        :Evenement(d, h, fin), titre(t), description(desc), lieu(l){}
    friend class ProgManager;
public:

    QString getDescription() const { return description; }
    QString getLieu() const { return lieu; }

    bool isProgTache() const { return false; }
    QString getTitre() const { return titre; }
};

class ProgrammationTache : public Evenement{
private:
    const TacheUnitaire* tache;
    ProgrammationTache(const QDate& d, const QTime& h, const QTime& fin, const TacheUnitaire& t): Evenement(d, h, fin), tache(&t){}
    friend class ProgManager;
public:
    const TacheUnitaire& getTache() const { return *tache; }

    virtual bool isProgTache() const { return true; }
    QString getTitre() const { return tache->getTitre(); }
};


typedef QList<Evenement*> ListEvent;
class ProgManager : public Singleton<ProgManager>{
private:
    ListEvent programmations;
    ProgrammationTache* trouverProgrammationT(const TacheUnitaire& TU) const;
    ProgrammationActivite* trouverProgrammationA(const ProgrammationActivite& PA) const;
    bool programmationExists(const QDate& d, const QTime& h, const QTime& fin);
    void ajouterProgrammation(Evenement& E);
public:
    void ajouterProgrammationT(const QDate& d, const QTime& h, const QTime& fin, const TacheUnitaire& TU);
    void ajouterProgrammationA(const QDate& d, const QTime& h, const QTime& fin, const QString& t, const QString& desc, const QString& l);


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



/* ----- [BEGIN]Projet ----- */

class Projet{
private:
    QString titre;
    QString description;
    QDate disponibilite;
    QDate echeance; // L’échéance d’un projet est une borne supérieure de l’ensemble des échéances des tâches de ce projet
    ListTaches taches;
    Projet(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const ListTaches& Taches):
            titre(t), description(desc), disponibilite(dispo), echeance(deadline), taches(Taches){}
    friend class ProjetManager;
public:
    QString getTitre() const { return titre; }
    void setTitre(const QString& str);
    QString getDescription() const { return description; }
    void setDescription(const QString& str);
    QDate getDateDisponibilite() const { return disponibilite; }
    QDate getDateEcheance() const { return echeance; }
    void setDatesDisponibiliteEcheance(const QDate& disp, const QDate& e){
        if (e<disp) throw CalendarException("erreur tâche : date échéance < date disponibilité");
        for (int i = 0; i < taches.size(); ++i)
            if(e<taches[i]->getDateEcheance()) throw CalendarException("erreur projet : date échéance < échéance d'une tache");
        disponibilite=disp; echeance=e;
    }
    const ListTaches& getTaches() const { return taches; }
    void setTaches(const ListTaches& T);
    void addTache(const Tache* t);
    void rmTache(const Tache* t);

    void ajouterInfos(QString& infos) const;
};

typedef QList<Projet*> ListProjet;

/* ----- [END]Projet ----- */

class ProjetManager : public Singleton<ProjetManager>{
private:
    ListProjet projets;

    QHash<QString, QString> tabParent;
    friend void Projet::setTaches(const ListTaches& T);
    friend void Projet::addTache(const Tache* t);
    friend void Projet::rmTache(const Tache* t);
    Projet* trouverProjet(const QString& titre) const;
public:
    Projet& ajouterProjet(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const ListTaches& Taches=ListTaches());

    bool isProjetExistant(const QString& titre) const { return trouverProjet(titre)!=0; }
    bool isTacheInProjet(const Tache& t);

    Projet& getProjet(const QString& titre);
    const Projet& getProjet(const QString& titre) const;

    Projet* getProjet(const Tache& t);
    const Projet* getProjet(const Tache& t)const;

    //void load(const QString& f);
    //void save(const QString& f);

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
};







#endif // CALENDAR_H
