#include "Calendar.h"

void TacheManager::ajouterTache(const string& id,
const string& t, const Duree& dur,
const Date& dispo, const Date& deadline){
    Tache* une_tache = new Tache(id, t, dur, dispo, deadline);
    addItem(une_tache);
}

void TacheManager::addItem(Tache* t){
    if (nb%nbMax > 0 || nb == 0){
        taches[nb] = t;
        nb++;
    }
    else{ // nb%10 == 0 && nb > 0
        Tache** new_taches = new Tache*[nb + nbMax];
        for (unsigned int i = 0; i < nb; i++)
            new_taches[i] = taches[i];
        // std::memcpy(new_taches, taches, sizeof(Tache*)*nb);
        new_taches[nb] = t;
        nb++;
        Tache** old_taches = taches;
        taches = new_taches;
        delete[] old_taches;
    }
}

Tache& TacheManager::getTache(const string& id){
    unsigned int i = 0;
    while(i < nb)
        if (taches[i]->getId() == id)
            return *taches[i];
        else i++;
    throw CalendarException("Tache inexistante");
}

const Tache& TacheManager::getTache(const string& id) const{
    /* this est de type "const TacheManager*"
    si on la convertit en TacheManager*
    et que l'on applique la méthode getTache sur
    le résultat de la conversion, on utilise la
    méthode non const
    */
    return const_cast<TacheManager*>(this)->getTache(id);
}

TacheManager::~TacheManager(){
    for (unsigned int i = 0; i < nb; i++)
        delete taches[i];
    delete[] taches;
    nb = 0; nbMax = 0; taches = 0;
}

TacheManager::TacheManager(const TacheManager& tm):
nb(0), nbMax(tm.nbMax){
    taches = new Tache*[nbMax];
    /*for (unsigned int i = 0; i < nb; i++)
        ajouterTache(tm.taches[i]->getId(),
tm.taches[i]->getTitre(), tm.taches[i]->getDuree(),
tm.taches[i]->getDisp(), tm.taches[i]->getEcheance());
    */
    *this = tm;
}

TacheManager& TacheManager::operator=
(const TacheManager& tm){
    /* destruction des taches et du tableeau
    de pointeurs existants
    */
    this->~TacheManager();
    for (unsigned int i = 0; i < nb; i++)
        ajouterTache(tm.taches[i]->getId(),
tm.taches[i]->getTitre(), tm.taches[i]->getDuree(),
tm.taches[i]->getDisp(), tm.taches[i]->getEcheance());
}
