#include "TacheManager.h"
#include "ProgManager.h"
#include "PrecedenceManager.h"

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
        if (tabParent.contains(sT[i]))
            throw CalendarException("erreur, TacheManager::ajouterTacheComposite, sous-tâche déjà subordonnée à une tâche");
        tabParent.insert(sT[i], TC);
    }
    return *TC;
}

void TacheManager::deleteTache(const QString& str){
    Tache& tacheToDelete = getTache(str);
    PrecedenceManager& PrM = *PrecedenceManager::getInstance();
    ProjetManager& PM = *ProjetManager::getInstance();

    //Supprimer les précédences
    ListTachesConst successeurs = PrM.trouverSuccesseurs(tacheToDelete);
    for (int i =0; i < successeurs.size(); ++i)
        PrM.supprimerPrecedence(tacheToDelete, *successeurs[i]);

    ListTachesConst predecesseurs = PrM.trouverPrecedences(tacheToDelete);
    for (int i =0; i < predecesseurs.size(); ++i)
        PrM.supprimerPrecedence(*predecesseurs[i], tacheToDelete);

    //Cas de la Tache Unitaire
    if (tacheToDelete.isTacheUnitaire()){
        TacheUnitaire& tacheUToDelete = dynamic_cast<TacheUnitaire&>(tacheToDelete);

        //Supprimer les programmations
        ProgManager& PM = *ProgManager::getInstance();
        PM.supprimerProgrammationsT(tacheUToDelete);

        //Supprimer la tâche
        for (int i = 0; i < taches.size(); ++i)
            if (taches[i] == &tacheUToDelete)
                taches.removeAt(i);
        if (getTacheMere(tacheUToDelete))
            dynamic_cast<TacheComposite*>(getTacheMere(tacheUToDelete))->rmSousTache(&tacheUToDelete);
    }
    //Cas de la Tache Composite
    else{
        TacheComposite& tacheCToDelete = dynamic_cast<TacheComposite&>(tacheToDelete);

        //Supprimer la tâche
        for (int i = 0; i < taches.size(); ++i)
            if (taches[i] == &tacheCToDelete)
                taches.removeAt(i);
        ListTachesConst rmST = tabParent.keys(&tacheCToDelete);
        for (int i = 0; i != rmST.size(); ++i)
            tacheCToDelete.rmSousTache(rmST[i]);
        if (getTacheMere(tacheCToDelete))
            dynamic_cast<TacheComposite*>(getTacheMere(tacheCToDelete))->rmSousTache(&tacheCToDelete);


        //Supprimer les sous-tâches
         for (int i = 0; i < rmST.size(); ++i) deleteTache(rmST[i]->getTitre());
    }

    //Suppression de l'appartenance à un projet
    if (PM.getProjet(tacheToDelete))
        PM.getProjet(tacheToDelete)->rmTache(&tacheToDelete);

    delete &tacheToDelete;
}

Tache& TacheManager::getTache(const QString& titre){
    Tache* t = trouverTache(titre);
    if (!t) throw CalendarException("erreur, TacheManager, tache inexistante");
    return *t;
}

const Tache& TacheManager::getTache(const QString& titre) const{
    return const_cast<TacheManager*>(this)->getTache(titre);
}


Tache* TacheManager::getTacheMere(const Tache& t){
    if (tabParent.contains(&t))
        return const_cast<TacheComposite*>(tabParent.value(&t));
    return 0;
}

const Tache* TacheManager::getTacheMere(const Tache& t) const{
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
        for (const_HashIterator<TacheManager, Tache, TacheComposite> it = tabParentBegin(); it != tabParentEnd(); ++it){
            xml.writeStartElement("linkT");
                xml.writeTextElement("parent", (*it).value()->getTitre());
                xml.writeTextElement("son", (*it).key()->getTitre());
            xml.writeEndElement();
        }
        xml.writeEndElement();
    }
}
