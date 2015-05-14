#ifndef CALENDAR_h
#define CALENDAR_h

#include <QString>
#include <QDate>
#include <QTextStream>
#include <QDebug>

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

/*class Tache{
private:


};*/

class Tache{
private:
    QString titre;
    QString description;
    Duree duree;
    QDate disponibilite;
    QDate echeance;
    bool preemptive;
    Tache(const QString& id, const QString& t, const Duree& dur, const QDate& dispo, const QDate& deadline, bool preempt=false):
            titre(id),description(t),duree(dur),disponibilite(dispo),echeance(deadline),preemptive(preempt){}
	Tache(const Tache& t);
	Tache& operator=(const Tache&);
	friend class TacheManager;
public:
    QString getTitre() const { return titre; }
    void setTitre(const QString& str);
    QString getDescription() const { return description; }
    void setDescription(const QString& str) { description=str; }
    Duree getDuree() const { return duree; }
    void setDuree(const Duree& d) { duree=d; }
    QDate getDateDisponibilite() const {  return disponibilite; }
    QDate getDateEcheance() const {  return echeance; }
    void setDatesDisponibiliteEcheance(const QDate& disp, const QDate& e) {
        if (e<disp) throw CalendarException("erreur Tâche : date echéance < date disponibilité");
        disponibilite=disp; echeance=e;
    }
    bool isPreemptive() const { return preemptive; }
    void setPreemptive(bool b = true) { preemptive=b; }
};

QTextStream& operator<<(QTextStream& f, const Tache& t);

class TacheManager{
private:
    typedef QList<Tache*> ListTaches;
    ListTaches taches;

    Tache* trouverTache(const QString& id) const;
    QString file;
    TacheManager(){}
	~TacheManager();
	TacheManager(const TacheManager& um);
    TacheManager& operator=(const TacheManager& um);
	struct Handler{
		TacheManager* instance;
		Handler():instance(0){}
        // destructeur appelé à la fin du programme
		~Handler(){ if (instance) delete instance; }
	};
    static Handler handler;
public:
    Tache& ajouterTache(const QString& id, const QString& t, const Duree& dur, const QDate& dispo, const QDate& deadline, bool preempt=false);
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

class Programmation{
	const Tache* tache;
    QDate date;
    QTime horaire;
public:
    Programmation(const Tache& t, const QDate& d, const QTime& h):tache(&t), date(d), horaire(h){}
	const Tache& getTache() const { return *tache; }
    QDate getDate() const { return date; }
    QTime getHoraire() const { return horaire; }
};

class ProgrammationManager {
private:
	Programmation** programmations;
	unsigned int nb;
	unsigned int nbMax;
	void addItem(Programmation* t);
	Programmation* trouverProgrammation(const Tache& t) const;
public:
	ProgrammationManager();
	~ProgrammationManager();
	ProgrammationManager(const ProgrammationManager& um);
	ProgrammationManager& operator=(const ProgrammationManager& um);
    void ajouterProgrammation(const Tache& t, const QDate& d, const QTime& h);
};

#endif
