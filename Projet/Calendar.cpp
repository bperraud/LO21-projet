#include <typeinfo>

#include "Calendar.h"
#include "TacheManager.h"
#include "ProgManager.h"
#include "PrecedenceManager.h"

#include <QFile>
#include <QTextCodec>
#include <QtXml>
#include <QMessageBox>



void Tache::setTitre(const QString& str){
    if (TacheManager::getInstance()->isTacheExistante((str))) throw CalendarException("erreur TacheManager : tache id déjà existante");
    titre = str;
}

void Tache::ajouterInfos(QString& infos) const{
    infos.append("Titre : ").append(this->getTitre()).append("\n")
            .append("Description : ").append(this->getDescription()).append("\n")
            .append("Disponibilité : ").append(this->getDateDisponibilite().toString()).append("\n")
            .append("Échéance : ").append(this->getDateEcheance().toString()).append("\n")
            .append("Contraintes de précédence :\n");
    ListTachesConst precedences = PrecedenceManager::getInstance()->trouverPrecedences(*this);
    for (int i = 0; i < precedences.size(); ++i)
        infos.append("   - ").append(precedences[i]->getTitre()).append("\n");
}

void Tache::save(QXmlStreamWriter& stream) const{
    stream.writeStartElement("tache");
        if (isTacheUnitaire())
            stream.writeAttribute("preemptive", (dynamic_cast<const TacheUnitaire*>(this)->isPreemptive()) ? "true" : "false");
        stream.writeTextElement("titre", this->getTitre());
        stream.writeTextElement("description", this->getDescription());
        stream.writeTextElement("disponibilite", this->getDateDisponibilite().toString(Qt::ISODate));
        stream.writeTextElement("echeance", this->getDateEcheance().toString(Qt::ISODate));
        saveTache(stream);
    stream.writeEndElement();
}


void TacheUnitaire::ajouterInfos(QString& infos) const{
    Tache::ajouterInfos(infos);
    infos.append("Durée : ").append(this->getDuree().toString("hh'h'mm")).append("\n")
            .append(QString(this->isPreemptive() ? "Préemptive" : "Non preemptive")).append("\n");
}

void TacheUnitaire::saveTache(QXmlStreamWriter& stream) const{
    QString str;
    str.setNum(QTime(0, 0).secsTo(this->getDuree())/60);
    stream.writeTextElement("duree", str);
}

QTime TacheUnitaire::getDureeRestante() const{
    ProgManager& PM = *ProgManager::getInstance();
    QTime d = duree;
    if (PM.tabDuree.contains(titre)){
        QTime dureeDone = PM.tabDuree.value(titre);
        int dureeSec = QTime(0, 0).secsTo(dureeDone);
        d = d.addSecs(-dureeSec);
    }
    return d;
}


void TacheComposite::ajouterInfos(QString& infos) const{
    Tache::ajouterInfos(infos);
    infos.append("Sous-tâches :\n");
    for (int i = 0; i < sousTaches.size(); ++i)
        infos.append("   - ").append(sousTaches[i]->getTitre()).append("\n");
}

void TacheComposite::saveTache(QXmlStreamWriter& stream) const{
    for (int i = 0; i < this->getSousTaches().size(); ++i)
        stream.writeTextElement("sous-tache", this->getSousTaches()[i]->getTitre());
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


