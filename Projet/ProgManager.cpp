#include "ProgManager.h"

ProgrammationTache* ProgManager::trouverProgrammationT(const TacheUnitaire& TU) const{
    for (int i = 0; i < programmations.size(); ++i)
        if (programmations[i]->isProgTache())
            if (&TU == &(dynamic_cast<ProgrammationTache*>(programmations[i])->getTache())) return dynamic_cast<ProgrammationTache*>(programmations[i]);
    return 0;
}

ProgrammationActivite* ProgManager::trouverProgrammationA(const ProgrammationActivite& PA) const{
    for (int i = 0; i < programmations.size(); ++i)
        if (&PA == programmations[i]) dynamic_cast<ProgrammationActivite*>(programmations[i]);
    return 0;
}

void ProgManager::ajouterProgrammation(Evenement& E){
    if (programmationExists(E.getDate(), E.getHoraire(), E.getHoraireFin())) throw CalendarException("erreur, ProgManager, horaire déjà pris");
    programmations.append(&E);
}

void ProgManager::ajouterProgrammationT(const QDate& d, const QTime& h, const QTime& fin, const TacheUnitaire& TU){
    if (trouverProgrammationT(TU) && !TU.isPreemptive()) {throw CalendarException("erreur, ProgManager, tâche non préemptive déjà programmée");}
    // Rajouter les contraintes de précédence, de préemption, de disponibilité et d'échéance
    ProgrammationTache* PT = new ProgrammationTache(d, h, fin, TU);
    ajouterProgrammation(*PT);
}

void ProgManager::ajouterProgrammationA(const QDate& d, const QTime& h, const QTime& fin, const QString& t, const QString& desc, const QString& l){
    if (trouverProgrammationA(ProgrammationActivite(d, h, fin, t, desc, l))) {throw CalendarException("erreur, ProgManager, activité déjà existante");}
    ProgrammationActivite* PA = new ProgrammationActivite(d, h, fin, t, desc, l);
    ajouterProgrammation(*PA);
}

void ProgManager::updateDuree(const TacheUnitaire& TU, QTime d){
    if (tabDuree.contains(TU.getTitre())){
        int dureeSec = QTime(0, 0).secsTo(d);
        tabDuree[TU.getTitre()] = tabDuree[TU.getTitre()].addSecs(dureeSec);
        if (tabDuree[TU.getTitre()] > TU.getDuree()) throw CalendarException("erreur, ProgManager, durée écoulée > durée");
    }
    else tabDuree[TU.getTitre()] = d;
}

bool ProgManager::programmationExists(const QDate& d, const QTime& h, const QTime& fin){
    for (int i = 0; i < programmations.size(); ++i)
        if (programmations[i]->getDate() == d &&
                ((h <= programmations[i]->getHoraire() && fin > programmations[i]->getHoraire())
                ||
                 (h < programmations[i]->getHoraireFin() && fin >= programmations[i]->getHoraireFin())
                )
           )
            return true;
    return false;
}

ProgManager::~ProgManager(){
    //if (file!="") save(file);
    for (int i = 0; i < programmations.size(); ++i) delete programmations[i];
    programmations.clear();
    tabDuree.clear();
    //file="";
}
