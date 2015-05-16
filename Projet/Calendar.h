#ifndef CALENDAR_h
#define CALENDAR_h

#include <typeinfo>

#include <QString>
#include <QDate>
#include <QTextStream>
#include <QDebug>
#include <QTextCodec>
#include <QtXml>

using namespace std;

class CalendarException{
public:
    CalendarException(const QString& message):info(message){}
    QString getInfo() const { return info; }
private:
    QString info;
};


/*! \class Duree
        \brief Classe permettant de manipuler des durees
        L'utilisation de cette classe nécessite des dates valides au sens commun du terme.
        Déclenchement d'exception dans le cas contraire
*/
class Duree{
public:
    //! Constructeur à partir de heure et minute
    /*! \param h heure avec h>=0
        \param m minute avec 0<=m<=59
        */
    Duree(unsigned int h, unsigned int m):nb_minutes(h*60+m) {if (m>59) throw CalendarException("erreur: initialisation duree invalide");}
    //! Constructeur à partir de minute
    /*! \param m minute avec m>=0
        */
    Duree(unsigned int m=0):nb_minutes(m) {}
    void setDuree(unsigned int minutes) { nb_minutes=minutes; }
    void setDuree(unsigned int heures, unsigned int minutes) { if (heures==0 && minutes>59) throw CalendarException("erreur: initialisation duree invalide"); nb_minutes=heures*60+minutes; }
    unsigned int getDureeEnMinutes() const { return nb_minutes; } //<!Retourne la duree en minutes
    double getDureeEnHeures() const { return double(nb_minutes)/60; } //<!Retourne la duree en heures
    unsigned int getMinute() const { return nb_minutes%60; }
    unsigned int getHeure() const { return nb_minutes/60; }
    void afficher(QTextStream& f) const; //<!Affiche la duree sous le format hhHmm
private:
    unsigned int nb_minutes;
};

QTextStream& operator<<(QTextStream& f, const Duree & d);
QTextStream& operator>>(QTextStream&, Duree&); //lecture format hhHmm


class TacheVisitor;

class Tache{
private:
    QString titre;
    QString description;
    QDate disponibilite;
    QDate echeance;

    Tache(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline):
            titre(t), description(desc), disponibilite(dispo), echeance(deadline){}
    Tache(const Tache& t);
    Tache& operator=(const Tache&);

    friend class TacheUnitaire;
    friend class TacheComposite;
public:
    virtual ~Tache(){}
    QString getTitre() const { return titre; }
    void setTitre(const QString& str);
    QString getDescription() const { return description; }
    void setDescription(const QString& str) { description=str; }
    QDate getDateDisponibilite() const {  return disponibilite; }
    QDate getDateEcheance() const {  return echeance; }
    void setDatesDisponibiliteEcheance(const QDate& disp, const QDate& e) {
        if (e<disp) throw CalendarException("erreur tâche : date echéance < date disponibilité");
        disponibilite=disp; echeance=e;
    }
    /*virtual Duree getDuree() const =0;
    virtual void setDuree(const Duree& d) =0;
    virtual bool isPreemptive() const =0;
    virtual void setPreemptive(bool b = true) =0;*/

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
    friend class TacheManager;
public:
    ~TacheComposite(){}
    const ListTaches& getSousTaches() const { return sousTaches; }
    void setSousTaches(const ListTaches& sT);
    void addSousTache(const Tache* t);
    void rmSousTache(const Tache* t);
    /*Duree getDuree() const { return Duree(); }
    void setDuree(const Duree&){}
    bool isPreemptive() const { return false; }
    void setPreemptive(bool){}*/

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

struct HierarchyTachesC{
    QString mere;
    QString fille;
    HierarchyTachesC(QString m, QString f):mere(m),fille(f){}
};

/* --- [BEGIN]Design Pattern Visitor --- */

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

/* --- [END]Design Pattern Visitor --- */





class TacheManager{
private:
    ListTaches taches;

    Tache* trouverTache(const QString& titre) const;
    QString file;
    TacheManager(){}
	~TacheManager();
	TacheManager(const TacheManager& um);
    TacheManager& operator=(const TacheManager& um);
	struct Handler{
		TacheManager* instance;
		Handler():instance(0){}
		~Handler(){ if (instance) delete instance; }
	};
    static Handler handler;
public:
    Tache& ajouterTacheUnitaire(const QString& t, const QString& desc, const Duree& dur, const QDate& dispo, const QDate& deadline, bool preempt=false);
    Tache& ajouterTacheComposite(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const ListTaches& LT);
    Tache& getTache(const QString& titre);
    bool isTacheExistante(const QString& titre) const { return trouverTache(titre)!=0; }
    const Tache& getTache(const QString& titre) const;
    void load(const QString& f);
    void save(const QString& f);
	static TacheManager& getInstance();
	static void libererInstance();

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

class Evenement{
private:
    QDate date;
    QTime horaire;
public:
    Evenement(const QDate& d, const QTime& h): date(d), horaire(h){}
    QDate getDate() const { return date; }
    QTime getHoraire() const { return horaire; }
};

class ProgrammationActivite : public Evenement{
private:
    QString titre;
    QString description;
    QString lieu;
public:
    ProgrammationActivite(const QDate& d, const QTime& h, const QString& t, const QString& desc, const QString& l)
        :Evenement(d, h), titre(t), description(desc), lieu(l){}
    QString getTitre() const { return titre; }
    QString getDescription() const { return description; }
    QString getLieu() const { return lieu; }
};

class ProgrammationTache : public Evenement{
private:
    const TacheUnitaire* tache;
public:
    ProgrammationTache(const QDate& d, const QTime& h, const TacheUnitaire& t): Evenement(d, h), tache(&t){}
    const TacheUnitaire& getTache() const { return *tache; }
};


template<class Prog, class Obj> class ProgManager{
private:
    QList<Prog*> programmations;
    Prog* trouverProgrammation(const Obj& P) const;
public:
    QString a;
};
/*template<> class ProgManager<ProgrammationActivite, ProgrammationActivite>{

};*/

class PMTache : public ProgManager<ProgrammationTache, TacheUnitaire>{
public:
    QString b;
};


class ProgrammationManager{
private:
    ProgrammationTache** programmations;
	unsigned int nb;
	unsigned int nbMax;
    void addItem(ProgrammationTache* t);
    ProgrammationTache* trouverProgrammation(const TacheUnitaire& t) const;
public:
	ProgrammationManager();
	~ProgrammationManager();
	ProgrammationManager(const ProgrammationManager& um);
	ProgrammationManager& operator=(const ProgrammationManager& um);
    void ajouterProgrammation(const TacheUnitaire& t, const QDate& d, const QTime& h);
};



#endif
