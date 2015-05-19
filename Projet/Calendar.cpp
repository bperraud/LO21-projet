#include <typeinfo>

#include "Calendar.h"
#include"TacheManager.h"

#include <QFile>
#include <QTextCodec>
#include <QtXml>
#include <QMessageBox>



QTextStream& operator<<(QTextStream& fout, const Tache& t){
    fout<<t.getTitre()<<"\n";
    fout<<t.getDescription()<<"\n";
    fout<<t.getDateDisponibilite().toString()<<"\n";
    fout<<t.getDateEcheance().toString()<<"\n";
	return fout;
}

QTextStream& operator<<(QDataStream& f, const ProgrammationTache& p);



void Tache::setTitre(const QString& str){
  if (TacheManager::getInstance()->isTacheExistante((str))) throw CalendarException("erreur TacheManager : tache id déjà existante");
  titre=str;
}

void TacheComposite::setSousTaches(const ListTaches &sT){
    for (int i = 0; i < sT.size(); ++i)
        if (!TacheManager::getInstance()->isTacheExistante(sT[i]->getTitre()))
            throw CalendarException("Erreur tâche composite, tâche unitaire non trouvée");
    sousTaches = sT;
}

void TacheComposite::addSousTache(const Tache* t){
    for (int i = 0; i < sousTaches.size(); ++i)
        if (sousTaches[i] == t) throw CalendarException("erreur, TacheComposite, tâche déjà existante");
    sousTaches.append(const_cast<Tache*>(t));
}

void TacheComposite::rmSousTache(const Tache* t){
    for (int i = 0; i < sousTaches.size(); ++i){
        if (sousTaches[i] == t){
            sousTaches.removeAt(i);
            return;
        }
    }
    throw CalendarException("erreur, TacheComposite, tâche à supprimer non trouvée");
}



//******************************************************************************************
// ProgTacheManager





ProgrammationTache* ProgTacheManager::trouverProgrammation(const TacheUnitaire& TU) const{
    for (int i = 0; i < programmations.size(); ++i)
        if (&TU == &programmations[i]->getTache()) return programmations[i];
    return 0;
}

void ProgTacheManager::ajouterProgrammation(const QDate& d, const QTime& h, const TacheUnitaire& TU){
if (trouverProgrammation(TU) && !TU.isPreemptive()) {throw CalendarException("erreur, ProgTacheManager, tâche non préemptive déjà existante");}
// Rajouter les contraintes de précédence, de préemption, de disponibilité et d'échéance
ProgrammationTache* PT = new ProgrammationTache(d, h, TU);
programmations.append(PT);
}



ProgrammationActivite* ProgActiviteManager::trouverProgrammation(const ProgrammationActivite& PA) const{
    for (int i = 0; i < programmations.size(); ++i)
        if (&PA == programmations[i]) return programmations[i];
    return 0;
}

void ProgActiviteManager::ajouterProgrammation(const QDate& d, const QTime& h, const QString& t, const QString& desc, const QString& l){
if (trouverProgrammation(ProgrammationActivite(d, h, t, desc, l))) {throw CalendarException("erreur, ProgActiviteManager, activité déjà existante");}
ProgrammationActivite* PA = new ProgrammationActivite(d, h, t, desc, l);
programmations.append(PA);
}




/* --- Design Pattern Visitor --- */

void TacheUnitaire::accept(TacheVisitor* v){
    v->visitTacheUnitaire(this);
}
void TacheComposite::accept(TacheVisitor* v){
    v->visitTacheComposite(this);
}

void TacheInformateur::visitTacheUnitaire(TacheUnitaire* TU){
    qDebug() << "durée en min : " << QString(QString::number(static_cast<int>(TU->getDuree().getDureeEnMinutes()))) << "\n";
}
void TacheInformateur::visitTacheComposite(TacheComposite* TC){
    qDebug() << "titre tâche composite : " << QString(TC->getTitre()) << "\n";
}

/* --- Design Pattern Factory --- */


