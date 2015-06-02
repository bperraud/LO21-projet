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

QTextStream& operator<<(QTextStream& fout, const ProgrammationTache& p){
    fout<<p.getDate().toString()<<"\n";
    fout<<p.getHoraire().toString()<<"\n";
    fout<<p.getTache().getTitre()<<"\n";
    return fout;
}


void Tache::setTitre(const QString& str){
  if (TacheManager::getInstance()->isTacheExistante((str))) throw CalendarException("erreur TacheManager : tache id déjà existante");
  titre=str;
}

void Tache::ajouterInfos(QString& infos) const{
    infos.append("Titre : ").append(this->getTitre()).append("\n")
            .append("Description : ").append(this->getDescription()).append("\n")
            .append("Disponibilité : ").append(this->getDateDisponibilite().toString()).append("\n")
            .append("Échéance : ").append(this->getDateEcheance().toString()).append("\n");
}



void TacheUnitaire::ajouterInfos(QString& infos) const{
    Tache::ajouterInfos(infos);
    infos.append("Durée : ").append(this->getDuree().toString("hh'h'mm")).append("\n")
            .append(QString(this->isPreemptive() ? "Préemptive" : "Non preemptive")).append("\n");
}

void TacheUnitaire::saveTache(QXmlStreamWriter& stream){
    stream.writeStartElement("tache");
    stream.writeAttribute("preemptive", (this->isPreemptive())?"true":"false");
    stream.writeTextElement("titre", this->getTitre());
    stream.writeTextElement("description", this->getDescription());
    stream.writeTextElement("disponibilite", this->getDateDisponibilite().toString(Qt::ISODate));
    stream.writeTextElement("echeance", this->getDateEcheance().toString(Qt::ISODate));
    QString str;
    str.setNum(this->getDuree().minute()+(this->getDuree().hour())*60);
    stream.writeTextElement("duree",str);
    stream.writeEndElement();
}

void TacheComposite::ajouterInfos(QString& infos) const{
    Tache::ajouterInfos(infos);
    for (int i = 0; i < sousTaches.size(); ++i)
        infos.append("Sous-tâche : ").append(sousTaches[i]->getTitre()).append("\n");
}

void TacheComposite::saveTache(QXmlStreamWriter& stream){
    stream.writeStartElement("tache");
    stream.writeTextElement("titre", this->getTitre());
    stream.writeTextElement("description", this->getDescription());
    stream.writeTextElement("disponibilite", this->getDateDisponibilite().toString(Qt::ISODate));
    stream.writeTextElement("echeance", this->getDateEcheance().toString(Qt::ISODate));
    for (int i = 0; i < this->getSousTaches().size(); ++i)
        stream.writeTextElement("sous-tache", this->getSousTaches()[i]->getTitre());
    stream.writeEndElement();
}

void TacheComposite::setSousTaches(const ListTaches &sT){
    for (int i = 0; i < sT.size(); ++i){
        if (TacheManager::getInstance()->tabParent.contains(sT[i]->getTitre()))
            throw CalendarException("Erreur tâche composite (set), tâche déjà associée à une autre tâche");
        TacheManager::getInstance()->tabParent.insert(sT[i]->getTitre(), this->getTitre());
    }
    sousTaches = sT;
}

void TacheComposite::addSousTache(const Tache* t){

    for (int i = 0; i < sousTaches.size(); ++i)
        if (sousTaches[i] == t) throw CalendarException("erreur, TacheComposite, tâche déjà existante");
    if (TacheManager::getInstance()->tabParent.contains(t->getTitre()))
        if(TacheManager::getInstance()->tabParent.value(t->getTitre()) != this->getTitre())
            throw CalendarException("Erreur tâche composite (add), tâche déjà associée à une autre tâche");

    sousTaches.append(const_cast<Tache*>(t));
    TacheManager::getInstance()->tabParent.insert(t->getTitre(), this->getTitre());
}

void TacheComposite::rmSousTache(const Tache* t){
    for (int i = 0; i < sousTaches.size(); ++i){
        if (sousTaches[i] == t){
            sousTaches.removeAt(i);
            TacheManager::getInstance()->tabParent.remove(t->getTitre());
            return;
        }
    }
    throw CalendarException("erreur, TacheComposite, tâche à supprimer non trouvée");
}

//******************************************************************************************
// PrecedenceManager

//Etablir comment trouver les précédences d'une tache + vérifier que précédence pas déjà existante.


void PrecedenceManager::ajouterPrecedence(const Tache &Tpred, const Tache &Tsucc){
    if (Tpred.getDateDisponibilite() > Tsucc.getDateEcheance())
        throw CalendarException("erreur, PrecedenceManager, tentative de programmer une précédence avec une tache disponible après échéance de sa successeur.");
    for (int i = 0; i < precedences.size(); ++i) {
        if (&precedences[i]->getPredecesseur() == &Tpred && &precedences[i]->getSuccesseur() == &Tsucc)
            throw CalendarException("erreur, PrecedenceManager, précédence déjà existante");
        if (&precedences[i]->getPredecesseur() == &Tsucc && &precedences[i]->getSuccesseur() == &Tpred)
            throw CalendarException("erreur, PrecedenceManager, tentative d'implémenter un circuit de précédences");
    };
    Precedence* P = new Precedence(Tpred, Tsucc);
    precedences.append(P);
}

void PrecedenceManager::supprimerPrecedence(const Tache& Tpred, const Tache& Tsucc){
    if (PrecedenceManager::isPrecedence(Tpred, Tsucc))
        for (int i = 0; i < precedences.size(); ++i)
            if (&precedences[i]->getPredecesseur() == &Tpred && &precedences[i]->getSuccesseur() == &Tsucc)
                precedences.removeAt(i);
    else
        throw CalendarException("erreur, PrecedenceManager, tentative d'effacer une precedence qui n'existe pas.");

}

ListTachesConst PrecedenceManager::trouverPrecedences(const Tache& Tsucc) const{
    ListTachesConst LT;
    for (int i = 0; i < precedences.size(); ++i)
        if (&precedences[i]->getSuccesseur() == &Tsucc)
            LT.append(&precedences[i]->getPredecesseur());
    return LT;
}

bool PrecedenceManager::isPrecedence(const Tache& Tpred, const Tache& Tsucc) const {
    for (int i = 0; i < precedences.size(); ++i)
        if ((&precedences[i]->getPredecesseur() == &Tpred) && (&precedences[i]->getSuccesseur() == &Tsucc))
            return true;
    return false;

}

/* --- [BEGIN]ProgManager --- */

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
    if (trouverProgrammationA(ProgrammationActivite(d, h, fin, t, desc, l))) {throw CalendarException("erreur, ProgActiviteManager, activité déjà existante");}
    ProgrammationActivite* PA = new ProgrammationActivite(d, h, fin, t, desc, l);
    ajouterProgrammation(*PA);
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

/* --- [END]ProgManager --- */


/* --- Design Pattern Visitor --- */

void TacheUnitaire::accept(TacheVisitor* v){
    v->visitTacheUnitaire(this);
}
void TacheComposite::accept(TacheVisitor* v){
    v->visitTacheComposite(this);
}

void TacheInformateur::visitTacheUnitaire(TacheUnitaire* TU){
    qDebug() << "durée : " << TU->getDuree().toString("hh'h'mm") << "\n";
}
void TacheInformateur::visitTacheComposite(TacheComposite* TC){
    qDebug() << "titre tâche composite : " << QString(TC->getTitre()) << "\n";
}



/* --- [BEGIN]Projet --- */

void Projet::setTaches(const ListTaches &T){
    for (int i = 0; i < T.size(); ++i){
        if (!TacheManager::getInstance()->isTacheExistante(T[i]->getTitre()))
            throw CalendarException("Erreur projet, tâche non trouvée");
        if (ProjetManager::getInstance()->isTacheInProjet(*T[i]))
            throw CalendarException("Erreur projet, tâche appartenant déjà à un projet");
        ProjetManager::getInstance()->tabParent.insert(T[i]->getTitre(), this->getTitre());
    }
    taches = T;
}

void Projet::addTache(const Tache* t){
    for (int i = 0; i < taches.size(); ++i)
        if (taches[i] == t) throw CalendarException("erreur, Projet, tâche déjà existante");
    if (ProjetManager::getInstance()->isTacheInProjet(*t))
        throw CalendarException("Erreur projet, tâche appartenant déjà à un projet");
    taches.append(const_cast<Tache*>(t));
    ProjetManager::getInstance()->tabParent.insert(t->getTitre(), this->getTitre());
}

void Projet::rmTache(const Tache* t){
    for (int i = 0; i < taches.size(); ++i){
        if (taches[i] == t){
            taches.removeAt(i);
            ProjetManager::getInstance()->tabParent.remove(t->getTitre());
            return;
        }
    }
    throw CalendarException("erreur, Projet, tâche à supprimer non trouvée");
}

void Projet::ajouterInfos(QString& infos) const{
    infos.append("Titre : ").append(this->getTitre()).append("\n")
            .append("Description : ").append(this->getDescription()).append("\n")
            .append("Disponibilité : ").append(this->getDateDisponibilite().toString()).append("\n")
            .append("Échéance : ").append(this->getDateEcheance().toString()).append("\n");
    for (int i = 0; i < taches.size(); ++i)
        infos.append("Tâche : ").append(taches[i]->getTitre()).append("\n");
}

/* --- [END]Projet --- */

/* --- [BEGIN]ProjetManager --- */

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
    if (ProjetManager::getInstance()->tabParent.contains(t.getTitre()))
        return true;
    return false;
}

//void load(const QString& f);
//void save(const QString& f);

/* --- [END]ProjetManager --- */
