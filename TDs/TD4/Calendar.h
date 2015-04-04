#ifndef CALENDAR_h
#define CALENDAR_h

#include<string>
#include<iostream>

#include "timing.h"

using namespace std;
using namespace TIME;

class CalendarException{
public:
    CalendarException(const string& message):info(message){}
    string getInfo() const { return info; }
private:
    string info;
};

class Tache{
    private:
    string identificateur;
    string titre;
    Duree duree;
    Date disponibilite;
    Date echeance;
    public:
    Tache(const string& id,const string& t,
    const Duree& d, const Date& disp,
    const Date& e): identificateur(id), titre(t),
    duree(d), disponibilite(disp), echeance(e)
    {};
    const string& getId() const { return identificateur; }
    const string& getTitre() const { return titre; }
    const Duree& getDuree() const { return duree; }
    const Date& getDisp() const { return disponibilite; }
    const Date& getEcheance() const { return echeance; }
};

class TacheManager{
    private:
    Tache** taches;
    unsigned int nb;
    unsigned int nbMax;
    void addItem(Tache* t);
    public:
    TacheManager(){
        nb = 0;
        nbMax = 10;
        taches = new Tache*[nbMax];
    }
     ~TacheManager();
    TacheManager(const TacheManager& tm);
    TacheManager& operator=(const TacheManager& tm);
    void ajouterTache(const string& id,
    const string& t, const Duree& dur,
    const Date& dispo, const Date& deadline);
    Tache& getTache(const string& id);
    const Tache& getTache(const string& id) const;
};

class Programmation{
    private:
    Date date;
    Horaire horaire;
    Tache* tache;
    public:
    /* accesseurs standards
    pas besoin d'un destructeur car on ne veut
    pas que les taches pointées soient par exemple
    supprimées
    */
};

#endif
