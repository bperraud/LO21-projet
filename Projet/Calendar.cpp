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
    if (PM.tabDuree.contains(this)){
        QTime dureeDone = PM.tabDuree.value(this);
        int dureeSec = QTime(0, 0).secsTo(dureeDone);
        d = d.addSecs(-dureeSec);
    }
    return d;
}


void TacheComposite::ajouterInfos(QString& infos) const{
    TacheManager& TM = *TacheManager::getInstance();
    Tache::ajouterInfos(infos);
    infos.append("Sous-tâches :\n");
    for (TacheManager::tabParentIterator it = TM.tabParentBegin(); it != TM.tabParentEnd(); ++it)
        /*if ((*it).value() == this->getTitre())
            infos.append("   - ").append((*it).key()).append("\n");*/
        if ((*it).value() == this)
            infos.append("   - ").append((*it).key()->getTitre()).append("\n");
}



ListTachesConst TacheComposite::getSousTaches() const{
    TacheManager& TM = *TacheManager::getInstance();
    //ListTaches LT;
    //QList<QString> LS = TacheManager::getInstance()->tabParent.keys(this->getTitre());
    ListTachesConst LT = TM.tabParent.keys(this);
    //for (int i = 0; i < LS.size(); ++i) LT.append(&TacheManager::getInstance()->getTache(LS[i]));
    return LT;
}


void TacheComposite::setSousTaches(const ListTaches &sT){
    TacheManager& TM = *TacheManager::getInstance();
    for (int i = 0; i < sT.size(); ++i){
        //if (TacheManager::getInstance()->tabParent.contains(sT[i]->getTitre()))
        if (TM.tabParent.contains(sT[i]))
            throw CalendarException("Erreur tâche composite (set), tâche déjà associée à une autre tâche");
        //TM.tabParent.insert(sT[i]->getTitre(), this->getTitre());
        TM.tabParent.insert(sT[i], this);
    }
}

void TacheComposite::addSousTache(const Tache* t) const{
    TacheManager& TM = *TacheManager::getInstance();
    //if (TM.tabParent.contains(t->getTitre()))
    if (TM.tabParent.contains(t))
        throw CalendarException("Erreur tâche composite (add), tâche déjà associée à une tâche");
    //TM.tabParent.insert(t->getTitre(), this->getTitre());
    TM.tabParent.insert(t, this);
}

void TacheComposite::rmSousTache(const Tache* t){
    TacheManager& TM = *TacheManager::getInstance();
    //if (TM.tabParent.contains(t->getTitre())){
    if (TM.tabParent.contains(t)){
        if (TM.tabParent[t] != this) throw CalendarException("erreur, TacheComposite, tâche à supprimer n'appartient pas à this");
        //TM.tabParent.remove(t->getTitre());
        TM.tabParent.remove(t);
        return;
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

void Projet::setDatesDisponibiliteEcheance(const QDate& disp, const QDate& e){
    ProjetManager& PM = *ProjetManager::getInstance();
    if (e < disp) throw CalendarException("erreur tâche : date échéance < date disponibilité");
    for (ProjetManager::tabParentIterator it = PM.tabParentBegin(); it != PM.tabParentEnd(); ++it)
        if ((*it).value() == this)
            if(e < (*it).key()->getDateEcheance()) throw CalendarException("erreur projet : date échéance < échéance d'une tache");
    disponibilite = disp;
    echeance = e;
}

ListTachesConst Projet::getTaches() const{
    ProjetManager& PM = *ProjetManager::getInstance();
    ListTachesConst LT = PM.tabParent.keys(this);
    return LT;
}

void Projet::setTaches(const ListTaches &T){
    ProjetManager& PM = *ProjetManager::getInstance();
    TacheManager& TM = *TacheManager::getInstance();
    for (int i = 0; i < T.size(); ++i){
        if (!TM.isTacheExistante(T[i]->getTitre()))
            throw CalendarException("Erreur projet, tâche non trouvée");
        if (PM.isTacheInProjet(*T[i]))
            throw CalendarException("Erreur projet, tâche appartenant déjà à un projet");
        PM.tabParent.insert(T[i], this);
    }
}

void Projet::addTache(const Tache* t){
    ProjetManager& PM = *ProjetManager::getInstance();
    for (ProjetManager::tabParentIterator it = PM.tabParentBegin(); it != PM.tabParentEnd(); ++it)
        if ((*it).key() == t && (*it).value() == this)
            throw CalendarException("erreur, Projet, tâche déjà existante");
    if (PM.isTacheInProjet(*t))
        throw CalendarException("Erreur projet, tâche appartenant déjà à un projet");
    PM.tabParent.insert(t, this);
}

void Projet::rmTache(const Tache* t){
    ProjetManager& PM = *ProjetManager::getInstance();
    if (PM.tabParent.contains(t)){
        if (PM.tabParent[t] != this) throw CalendarException("erreur, Projet, tâche à supprimer n'appartient pas à this");
        PM.tabParent.remove(t);
        return;
    }
    throw CalendarException("erreur, Projet, tâche à supprimer non trouvée");
}

void Projet::ajouterInfos(QString& infos) const{
    infos.append("Titre : ").append(this->getTitre()).append("\n")
            .append("Description : ").append(this->getDescription()).append("\n")
            .append("Disponibilité : ").append(this->getDateDisponibilite().toString()).append("\n")
            .append("Échéance : ").append(this->getDateEcheance().toString()).append("\n");
    ListTachesConst LT = getTaches();
    for (int i = 0; i < LT.size(); ++i)
        infos.append("Tâche : ").append(LT[i]->getTitre()).append("\n");
}

void Projet::save(QXmlStreamWriter& stream) const{
    stream.writeStartElement("projet");
        stream.writeTextElement("titre", this->getTitre());
        stream.writeTextElement("description", this->getDescription());
        stream.writeTextElement("disponibilite", this->getDateDisponibilite().toString(Qt::ISODate));
        stream.writeTextElement("echeance", this->getDateEcheance().toString(Qt::ISODate));
    stream.writeEndElement();
}


/* --- [END] Projet --- */

/* --- [BEGIN] Programmations --- */

void Evenement::save(QXmlStreamWriter& stream) const{
    stream.writeStartElement("evenement");
        stream.writeAttribute("activite", ((this)->isProgTache()) ? "false" : "true");
        stream.writeTextElement("date", this->getDate().toString(Qt::ISODate));
        QString str;
        str.setNum(QTime(0, 0).secsTo(this->getHoraire())/60);
        stream.writeTextElement("debut", str);
        str.setNum(QTime(0, 0).secsTo(this->getHoraireFin())/60);
        stream.writeTextElement("fin", str);
        saveEvt(stream);
    stream.writeEndElement();
}

void ProgrammationActivite::saveEvt(QXmlStreamWriter& stream) const{
    stream.writeTextElement("titre", this->getTitre());
    stream.writeTextElement("description", this->getDescription());
    stream.writeTextElement("lieu", this->getLieu());
}

void ProgrammationTache::saveEvt(QXmlStreamWriter& stream) const{
    stream.writeTextElement("tache", this->getTache().getTitre());
}








