#include "ProjetManager.h"
#include "TacheManager.h"

Projet* ProjetManager::trouverProjet(const QString& titre) const{
    for (int i = 0; i < projets.size(); ++i){
        if (titre == (projets.at(i))->getTitre()) return projets[i];
    }
    return 0;
}

Projet& ProjetManager::ajouterProjet(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const ListTaches& Taches){
    Projet* P = new Projet(t, desc, dispo, deadline, Taches);
    if (trouverProjet(P->getTitre()))
        throw CalendarException("erreur, ProjetManager, projet déjà existant");
    for (TacheManager::iterator i = TacheManager::getInstance()->begin(); i != TacheManager::getInstance()->end(); ++i)
        if ((*i).getTitre() == P->getTitre())
            throw CalendarException("erreur, ProjetManager, tâche portant déjà ce nom");
    for (int i = 0; i < Taches.size(); ++i){
        if (isTacheInProjet(*Taches[i]))
            throw CalendarException("Erreur ProjetManager, tâche appartenant déjà à un projet");
        ProjetManager::getInstance()->tabParent.insert(Taches[i]->getTitre(), P->getTitre());
    }
    projets.append(P);
    return *P;
}

Projet& ProjetManager::getProjet(const QString& titre){
    Projet* p=trouverProjet(titre);
    if (!p) throw CalendarException("erreur, ProjetManager, projet inexistant");
    return *p;
}

const Projet& ProjetManager::getProjet(const QString& titre) const{
    return const_cast<ProjetManager*>(this)->getProjet(titre);
}

Projet* ProjetManager::getProjet(const Tache& t){
    if (tabParent.contains(t.getTitre()))
        return &getProjet(tabParent.value(t.getTitre()));
    return 0;
}

const Projet* ProjetManager::getProjet(const Tache& t)const{
    return const_cast<ProjetManager*>(this)->getProjet(t);
}


bool ProjetManager::isTacheInProjet(const Tache& t){
    TacheManager& TM = *TacheManager::getInstance();
    if (ProjetManager::getInstance()->tabParent.contains(t.getTitre()))
        return true;
    if (TM.tabParent.contains(&t))
        return isTacheInProjet(*TM.tabParent[&t]);
    return false;
}

//void load(const QString& f);
//void save(const QString& f);



ProjetManager::~ProjetManager(){

}

