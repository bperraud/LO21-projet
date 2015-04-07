#ifndef CALENDAR_h
#define CALENDAR_h
#include<string>
#include<iostream>
#include "timing.h"

using namespace std;
using namespace TIME;

/* Exception pour le Calendar*/
class CalendarException{
public:
	CalendarException(const string& message):info(message){}
	string getInfo() const { return info; }
private:
	string info;
};

/* Déclaration de Tache pour TacheManager*/
class Tache;

/*------------------------------------------------------
Classe TacheManager, Singleton (avec Handler) contenant
plusieurs classes iterator
--------------------------------------------------------*/

class TacheManager{

private:

    struct Handler{
        TacheManager* TM;
        Handler():TM(0){};
        ~Handler(){delete TM;}
    };
    static Handler handler;

	Tache** taches;
	unsigned int nb;
	unsigned int nbMax;

	void addItem(Tache* t);
	Tache* trouverTache(const string& id) const;
	string file;
	TacheManager();
	~TacheManager();
	TacheManager(const TacheManager& um);
	TacheManager& operator=(const TacheManager& um);

public:

    static TacheManager& getTacheManager();
    static void freeTacheManager();

	Tache& ajouterTache(const string& id, const string& t, const Duree& dur, const Date& dispo, const Date& deadline);
	Tache& getTache(const string& id);
	const Tache& getTache(const string& code) const;

	void load(const string& f);
	void save(const string& f);

    friend class Iterator;

    class Iterator{
    private:
        // infos
        int indice_tache;
    public:
        Iterator(): indice_tache(0){}
        Tache& current() const {
            if (indice_tache >= getTacheManager().nb)
                throw "indirection d'un iterateur en fin de sequence";
            return *getTacheManager().taches[indice_tache];
        }
        bool isDone() const {return indice_tache==getTacheManager().nb;};
        void next() {
            if (indice_tache >= getTacheManager().nb)
                throw "incrementation d'un iterateur en fin de sequence";
            ++indice_tache;
        }
	};

	Iterator getIterator() {return Iterator();}

    class iterator{
    private:
        int indice_tache;
    public:
        iterator(): indice_tache(0){}

        // Surchage des opérateurs
        Tache& operator*() const {
            if (indice_tache >= getTacheManager().nb)
                throw "indirection d'un iterateur en fin de sequence";
            return *getTacheManager().taches[indice_tache];
        }
        iterator& operator++() {
            if (indice_tache >= getTacheManager().nb)
                throw "incrementation d'un iterateur en fin de sequence";
            ++indice_tache;
            return *this;
        }
        bool operator!=(iterator it) const {
            return indice_tache != it.indice_tache;
        }
        bool operator==(iterator it) const {
            return indice_tache == it.indice_tache;
        }

        friend class TacheManager;
	};

	friend class iterator;

	iterator begin() {return iterator();}
	iterator end(){
	    iterator tmp; tmp.indice_tache = nb; return tmp;
	}

	friend class DisponibiliteFilterIterator;
    class DisponibiliteFilterIterator{
        private:
        // infos
        int indice_tache;
        Date date_filtre;
        public:
        DisponibiliteFilterIterator(const Date& d);
	    Tache& current() const {
	        if (indice_tache >= getTacheManager().nb)
                throw "indirection d'un iterateur en fin de sequence";
            return *getTacheManager().taches[indice_tache];
        }
	    bool isDone() const {return indice_tache==getTacheManager().nb;};
	    void next();
	};

	DisponibiliteFilterIterator
	getDisponibiliteFilterIterator(Date& d){
	    return DisponibiliteFilterIterator(d);
    }
};

/*------------------------------------------------------
Classe Tache
--------------------------------------------------------*/

class Tache{
private:
    string identificateur;
    string titre;
    Duree duree;
    Date disponibilite;
    Date echeance;
    Tache(const Tache& t);
    Tache& operator=(const Tache& t);
    Tache(const string& id, const string& t, const Duree& dur, const Date& dispo, const Date& deadline):
        identificateur(id),titre(t),duree(dur),disponibilite(dispo),echeance(deadline){}
    friend Tache& TacheManager::ajouterTache(const string& id, const string& t, const Duree& dur, const Date& dispo, const Date& deadline);
public:
    string getId() const { return identificateur; }
    string getTitre() const { return titre; }
    Duree getDuree() const { return duree; }
    Date getDateDisponibilite() const {  return disponibilite; }
    Date getDateEcheance() const {  return echeance; }
};

/*------------------------------------------------------
Classe Programmation
--------------------------------------------------------*/

class Programmation{
	const Tache* tache;
	Date date;
	Horaire horaire;
public:
	Programmation(const Tache& t, const Date& d, const Horaire& h):tache(&t), date(d), horaire(h){}
	const Tache& getTache() const { return *tache; }
	Date getDate() const { return date; }
	Horaire getHoraire() const { return horaire; }
};

/*------------------------------------------------------
Classe ProgrammationManager
--------------------------------------------------------*/

class ProgrammationManager{
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
	void ajouterProgrammation(const Tache& t, const Date& d, const Horaire& h);
};

ostream& operator<<(ostream& f, const Tache& t);
ostream& operator<<(ostream& f, const Programmation& p);

#endif
