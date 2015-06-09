#include "ProjetManager.h"
#include "TacheManager.h"

Projet* ProjetManager::trouverProjet(const QString& titre) const{
    for (int i = 0; i < projets.size(); ++i){
        if (titre == (projets.at(i))->getTitre()) return projets[i];
    }
    return 0;
}

Projet& ProjetManager::ajouterProjet(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const ListTaches& Taches){
    Projet* P = new Projet(t, desc, dispo, deadline);
    if (trouverProjet(P->getTitre()))
        throw CalendarException("erreur, ProjetManager, projet déjà existant");
    for (TacheManager::iterator i = TacheManager::getInstance()->begin(); i != TacheManager::getInstance()->end(); ++i)
        if ((*i).getTitre() == P->getTitre())
            throw CalendarException("erreur, ProjetManager, tâche portant déjà ce nom");
    for (int i = 0; i < Taches.size(); ++i){
        if (isTacheInProjet(*Taches[i]))
            throw CalendarException("Erreur ProjetManager, tâche appartenant déjà à un projet");
        ProjetManager::getInstance()->tabParent.insert(Taches[i], P);
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
    TacheManager& TM = *TacheManager::getInstance();
    if (tabParent.contains(&t))
        return const_cast<Projet*>(tabParent.value(&t));
    else if (TM.tabParent.contains(&t))
        return getProjet(*TM.tabParent.value(&t));
    return 0;
}

const Projet* ProjetManager::getProjet(const Tache& t)const{
    return const_cast<ProjetManager*>(this)->getProjet(t);
}

bool ProjetManager::isTacheInProjet(const Tache& t){
    TacheManager& TM = *TacheManager::getInstance();
    if (ProjetManager::getInstance()->tabParent.contains(&t))
        return true;
    if (TM.tabParent.contains(&t))
        return isTacheInProjet(*TM.tabParent[&t]);
    return false;
}

void ProjetManager::loadProjets(QXmlStreamReader& xml){
    QString titre;
    QString description;
    QDate disponibilite;
    QDate echeance;

    xml.readNext();

    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "projet")){
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
        }
        xml.readNext();
    }
    ajouterProjet(titre, description, disponibilite, echeance);
}

void ProjetManager::loadHierarchie(QXmlStreamReader& xml){
    QString projet;
    QString tache;

    xml.readNext();

    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "linkP")){
        if(xml.tokenType() == QXmlStreamReader::StartElement){
            if(xml.name() == "projet"){
                xml.readNext();
                projet = xml.text().toString();
            }
            if(xml.name() == "tache"){
                xml.readNext();
                tache = xml.text().toString();
            }
        }
        xml.readNext();
    }
    tabParent[&TacheManager::getInstance()->getTache(tache)] = &getProjet(projet);
}


void ProjetManager::save(QXmlStreamWriter& xml) const{
    // Sauvegarde des projets
    if (!projets.isEmpty()){
        xml.writeStartElement("projets");
        for (int i = 0; i < projets.size(); ++i)
            projets[i]->save(xml);
        xml.writeEndElement();
    }

    // Sauvegarde de la hiérarchie
    if (!tabParent.isEmpty()){
        xml.writeStartElement("hierarchieP");
        for (const_tabParentIterator it = tabParentBegin(); it != tabParentEnd(); ++it){
            xml.writeStartElement("linkP");
                xml.writeTextElement("projet", (*it).value()->getTitre());
                xml.writeTextElement("tache", (*it).key()->getTitre());
            xml.writeEndElement();
        }
        xml.writeEndElement();
    }
}


ProjetManager::~ProjetManager(){
    for (int i = 0; i < projets.size(); ++i) delete projets[i];
    projets.clear();
    tabParent.clear();
}
