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
    if (tabDuree.contains(&TU)){
        int dureeSec = QTime(0, 0).secsTo(d);
        tabDuree[&TU] = tabDuree[&TU].addSecs(dureeSec);
        if (tabDuree[&TU] > TU.getDuree()) throw CalendarException("erreur, ProgManager, durée écoulée > durée");
    }
    else tabDuree[&TU] = d;
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
    for (int i = 0; i < programmations.size(); ++i) delete programmations[i];
    programmations.clear();
    tabDuree.clear();
}

/*
void ProgManager::load1(QXmlStreamReader& xml){

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

void ProgManager::load2(QXmlStreamReader& xml){

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
*/


void ProgManager::save(QXmlStreamWriter& xml){
    // Sauvegarde des programmations
    xml.writeStartElement("programmations");
    for (int i = 0; i < programmations.size(); ++i)
        programmations[i]->save(xml);
    xml.writeEndElement();

    // Sauvegarde des durees programmées
    xml.writeStartElement("durees");
    for (tabDureeIterator it = tabDureeBegin(); it != tabDureeEnd(); ++it){
        xml.writeStartElement("dureeT");
            QString str;
            str.setNum(QTime(0, 0).secsTo((*it).value())/60);
            xml.writeTextElement("duree", str);
            xml.writeTextElement("tache", (*it).key()->getTitre());
        xml.writeEndElement();
    }
    xml.writeEndElement();
}



