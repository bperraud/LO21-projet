#include "TacheManager.h"

Tache* TacheManager::trouverTache(const QString& titre) const{
    for (int i = 0; i < taches.size(); ++i){
        if (titre == (taches.at(i))->getTitre()) return taches[i];
    }
    return 0;
}

void TacheManager::ajouterTache(Tache& T){
    if (trouverTache(T.getTitre())) {throw CalendarException("erreur, TacheManager, tâche déjà existante");}
    taches.append(&T);
}

TacheUnitaire& TacheManager::ajouterTacheUnitaire(const QString& t, const QString& desc, const QTime& dur, const QDate& dispo, const QDate& deadline, bool preempt){
    TacheUnitaire* TU = new TacheUnitaire(t, desc, dur, dispo, deadline, preempt);
    ajouterTache(*TU);
    return *TU;
}

TacheComposite& TacheManager::ajouterTacheComposite(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const ListTaches& sT){
    TacheComposite* TC = new TacheComposite(t, desc, dispo, deadline);
    ajouterTache(*TC);
    for (int i = 0; i < sT.size(); ++i){
        //if (tabParent.contains(sT[i]->getTitre()))
        if (tabParent.contains(sT[i]))
            throw CalendarException("erreur, TacheManager::ajouterTacheComposite, sous-tâche déjà subordonnée à une tâche");
        //tabParent.insert(sT[i]->getTitre(), TC->getTitre());
        tabParent.insert(sT[i], TC);
    }
    return *TC;
}

Tache& TacheManager::getTache(const QString& titre){
    Tache* t=trouverTache(titre);
    if (!t) throw CalendarException("erreur, TacheManager, tache inexistante");
    return *t;
}

const Tache& TacheManager::getTache(const QString& titre)const{
    return const_cast<TacheManager*>(this)->getTache(titre);
}


Tache* TacheManager::getTacheMere(const Tache& t){
    if (tabParent.contains(&t))
        return const_cast<TacheComposite*>(tabParent.value(&t));
    return 0;
}

const Tache* TacheManager::getTacheMere(const Tache& t)const{
    return const_cast<TacheManager*>(this)->getTacheMere(t);
}

TacheManager::~TacheManager(){
    for (int i = 0; i < taches.size(); ++i) delete taches[i];
    taches.clear();
    tabParent.clear();
}

void TacheManager::loadTaches(QXmlStreamReader& xml){

    QString titre;
    QString description;
    QDate disponibilite;
    QDate echeance;
    QTime duree;
    bool preemptive;
    bool unitaire;

    QXmlStreamAttributes attributes = xml.attributes();
    if(attributes.hasAttribute("preemptive")){
        QString val = attributes.value("preemptive").toString();
        preemptive = (val == "true" ? true : false);
        unitaire = true;
    }
    else unitaire = false;

    xml.readNext();

    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "tache")){
        if(xml.tokenType() == QXmlStreamReader::StartElement){
            if(xml.name() == "titre"){
                xml.readNext();
                titre = xml.text().toString();
            }
            if(xml.name() == "description"){
                xml.readNext();
                description = xml.text().toString();
            }
            if(xml.name() == "disponibilite"){
                xml.readNext();
                disponibilite = QDate::fromString(xml.text().toString(),Qt::ISODate);
            }
            if(xml.name() == "echeance"){
                xml.readNext();
                echeance = QDate::fromString(xml.text().toString(),Qt::ISODate);
            }
            if(xml.name() == "duree"){
                xml.readNext();
                duree.setHMS((xml.text().toInt())/60,(xml.text().toInt())%60,0);
            }
        }
        xml.readNext();
    }
    if (unitaire) ajouterTacheUnitaire(titre, description, duree, disponibilite, echeance, preemptive);
    else ajouterTacheComposite(titre, description, disponibilite, echeance);
}

void TacheManager::loadHierarchie(QXmlStreamReader& xml){
    QString mere;
    QString fille;

    xml.readNext();

    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "linkT")){
        if(xml.tokenType() == QXmlStreamReader::StartElement){
            if(xml.name() == "parent"){
                xml.readNext();
                mere = xml.text().toString();
            }
            if(xml.name() == "son"){
                xml.readNext();
                fille = xml.text().toString();
            }
        }
        xml.readNext();
    }
    tabParent[&getTache(fille)] = &dynamic_cast<TacheComposite&>(getTache(mere));
}


void TacheManager::save(QXmlStreamWriter& xml) const{
    // Sauvegarde des tâches
    if (!taches.isEmpty()){
        xml.writeStartElement("taches");
        for (int i = 0; i < taches.size(); ++i)
            taches[i]->save(xml);
        xml.writeEndElement();
    }

    // Sauvegarde de la hiérarchie
    if (!tabParent.isEmpty()){
        xml.writeStartElement("hierarchieT");
        for (const_tabParentIterator it = tabParentBegin(); it != tabParentEnd(); ++it){
            xml.writeStartElement("linkT");
                xml.writeTextElement("parent", (*it).value()->getTitre());
                xml.writeTextElement("son", (*it).key()->getTitre());
            xml.writeEndElement();
        }
        xml.writeEndElement();
    }
}
