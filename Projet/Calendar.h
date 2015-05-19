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


class TacheUnitaire;
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

    virtual void ajouterInfos(QString& liste){
        liste.append(this->getTitre()).append("\n")
                .append(this->getDescription()).append("\n")
                .append(this->getDateDisponibilite().toString()).append("\n")
                .append(this->getDateEcheance().toString()).append("\n");
    }

    virtual void saveTache(QXmlStreamWriter& stream) =0;
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
    friend class FabriqueTacheU;
    friend class TacheManager;
public:
    ~TacheUnitaire(){}
    Duree getDuree() const { return duree; }
    void setDuree(const Duree& d) { duree=d; }
    bool isPreemptive() const { return preemptive; }
    void setPreemptive(bool b = true) { preemptive=b; }

    void ajouterInfos(QString& liste){
        Tache::ajouterInfos(liste);
        liste.append(QString::number(this->getDuree().getDureeEnMinutes())).append("min\n")
                .append(QString(this->isPreemptive() ? "preemptive" : "non preemptive")).append("\n")
                .append("\n\n");
    }

    void saveTache(QXmlStreamWriter& stream){
        stream.writeStartElement("tache");
        stream.writeAttribute("preemptive", (this->isPreemptive())?"true":"false");
        stream.writeTextElement("titre", this->getTitre());
        stream.writeTextElement("description", this->getDescription());
        stream.writeTextElement("disponibilite", this->getDateDisponibilite().toString(Qt::ISODate));
        stream.writeTextElement("echeance", this->getDateEcheance().toString(Qt::ISODate));
        QString str;
        str.setNum(this->getDuree().getDureeEnMinutes());
        stream.writeTextElement("duree",str);
        stream.writeEndElement();
    }
    bool isTacheUnitaire() const { return true; }
    void accept(TacheVisitor* v);
};


class TacheComposite : public Tache{
private:
    ListTaches sousTaches;
    TacheComposite(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const ListTaches& sT):
            Tache(t, desc, dispo, deadline), sousTaches(sT){}
    friend class FabriqueTacheC;
    friend class TacheManager;
public:
    ~TacheComposite(){}
    const ListTaches& getSousTaches() const { return sousTaches; }
    void setSousTaches(const ListTaches& sT);
    void addSousTache(const Tache* t);
    void rmSousTache(const Tache* t);

    void ajouterInfos(QString& liste){
        Tache::ajouterInfos(liste);
        liste.append("composite").append("\n");
        for (int i = 0; i < sousTaches.size(); ++i)
            liste.append("sous-tache : ").append(sousTaches[i]->getTitre()).append("\n");
        liste.append("\n\n");
    }

    void saveTache(QXmlStreamWriter& stream){
        stream.writeStartElement("tache");
        stream.writeTextElement("titre", this->getTitre());
        stream.writeTextElement("description", this->getDescription());
        stream.writeTextElement("disponibilite", this->getDateDisponibilite().toString(Qt::ISODate));
        stream.writeTextElement("echeance", this->getDateEcheance().toString(Qt::ISODate));
        for (int i = 0; i < this->getSousTaches().size(); ++i)
            stream.writeTextElement("sous-tache", this->getSousTaches()[i]->getTitre());
        stream.writeEndElement();
    }
    bool isTacheUnitaire() const { return false; }
    void accept(TacheVisitor* v);
};

/* ----- [END] Design Pattern Composite ----- */

struct HierarchyTachesC{
    QString mere;
    QString fille;
    HierarchyTachesC(QString m, QString f):mere(m),fille(f){}
};

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
















class Evenement{
private:
    QDate date;
    QTime horaire;

    Evenement(const QDate& d, const QTime& h): date(d), horaire(h){}
    friend class ProgrammationActivite;
    friend class ProgrammationTache;
public:

    QDate getDate() const { return date; }
    QTime getHoraire() const { return horaire; }
};

class ProgrammationActivite : public Evenement{
private:
    QString titre;
    QString description;
    QString lieu;

    ProgrammationActivite(const QDate& d, const QTime& h, const QString& t, const QString& desc, const QString& l)
        :Evenement(d, h), titre(t), description(desc), lieu(l){}
    friend class ProgActiviteManager;
public:

    QString getTitre() const { return titre; }

    QString getDescription() const { return description; }
    QString getLieu() const { return lieu; }
};



class ProgrammationTache : public Evenement{
private:
    const TacheUnitaire* tache;
    ProgrammationTache(const QDate& d, const QTime& h, const TacheUnitaire& t): Evenement(d, h), tache(&t){}
    friend class ProgTacheManager;
public:
    const TacheUnitaire& getTache() const { return *tache; }
};





class ProgTacheManager : public Singleton<ProgTacheManager>{
private:
    QList<ProgrammationTache*> programmations;
    ProgrammationTache* trouverProgrammation(const TacheUnitaire& TU) const;
public:
    void ajouterProgrammation(const QDate& d, const QTime& h, const TacheUnitaire& TU);
};

class ProgTacheManager;

class ProgActiviteManager : public Singleton<ProgActiviteManager>{
private:
    QList<ProgrammationActivite*> programmations;
    ProgrammationActivite* trouverProgrammation(const ProgrammationActivite& PA) const;
public:
    void ajouterProgrammation(const QDate& d, const QTime& h, const QString& t, const QString& desc, const QString& l);
};







/* ----- [BEGIN]Projet ----- */

class Projet{
private:
    QString titre;
    QString description;
    QDate disponibilite;
    QDate echeance; // L’échéance d’un projet est une borne supérieure de l’ensemble des échéances des tâches de ce projet
    ListTaches taches;
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
};

/* ----- [END]Projet ----- */







#endif // CALENDAR_H
