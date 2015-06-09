#include "ProgManager.h"
#include "TacheManager.h"

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


void ProgManager::load1(QXmlStreamReader& xml){

    QString titre;
    QString description;
    QString lieu;
    QString tache;
    QDate date;
    QTime debut;
    QTime fin;
    bool activite;

    QXmlStreamAttributes attributes = xml.attributes();
    if(attributes.hasAttribute("activite")){
        QString val = attributes.value("activite").toString();
        activite = (val == "true" ? true : false);
    }
    else throw CalendarException("Erreur load Prog, pas d'attr trouvé !");

    xml.readNext();

    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "evenement")){
        if(xml.tokenType() == QXmlStreamReader::StartElement){
            if(xml.name() == "date"){
                xml.readNext();
                date = QDate::fromString(xml.text().toString(),Qt::ISODate);
            }
            if(xml.name() == "debut"){
                xml.readNext();
                debut.setHMS((xml.text().toInt())/60,(xml.text().toInt())%60,0);
            }
            if(xml.name() == "fin"){
                xml.readNext();
                fin.setHMS((xml.text().toInt())/60,(xml.text().toInt())%60,0);
            }
            if(xml.name() == "titre"){
                xml.readNext();
                titre = xml.text().toString();
            }
            if(xml.name() == "description"){
                xml.readNext();
                description = xml.text().toString();
            }
            if(xml.name() == "lieu"){
                xml.readNext();
                lieu = xml.text().toString();
            }
            if(xml.name() == "tache"){
                xml.readNext();
                tache = xml.text().toString();
            }
        }
        xml.readNext();
    }
    if (activite) ajouterProgrammationA(date, debut, fin, titre, description, lieu);
    else ajouterProgrammationT(date, debut, fin, dynamic_cast<TacheUnitaire&>(TacheManager::getInstance()->getTache(tache)));
}

void ProgManager::load2(QXmlStreamReader& xml){

    QTime duree;
    QString tache;

    xml.readNext();

    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "dureeT")){
        if(xml.tokenType() == QXmlStreamReader::StartElement){
            if(xml.name() == "duree"){
                xml.readNext();
                duree.setHMS((xml.text().toInt())/60,(xml.text().toInt())%60,0);
            }
            if(xml.name() == "tache"){
                xml.readNext();
                tache = xml.text().toString();
            }
        }
        xml.readNext();
    }
    tabDuree[&dynamic_cast<TacheUnitaire&>(TacheManager::getInstance()->getTache(tache))] = duree;
}



void ProgManager::save(QXmlStreamWriter& xml){
    // Sauvegarde des programmations
    if (!programmations.isEmpty()){
        xml.writeStartElement("programmations");
        for (int i = 0; i < programmations.size(); ++i)
            programmations[i]->save(xml);
        xml.writeEndElement();
    }

    // Sauvegarde des durées programmées
    if (!tabDuree.isEmpty()){
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
}



