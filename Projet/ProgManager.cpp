#include "ProgManager.h"
#include "TacheManager.h"
#include "ProjetManager.h"
#include "PrecedenceManager.h"

ProgrammationTache* ProgManager::trouverProgrammationT(const TacheUnitaire& TU) const{
    for (int i = 0; i < programmations.size(); ++i)
        if (programmations[i]->isProgTache())
            if (&TU == &(dynamic_cast<ProgrammationTache*>(programmations[i])->getTache())) return dynamic_cast<ProgrammationTache*>(programmations[i]);
    return 0;
}

bool ProgManager::isPrecedenceProgramme(const QDate& d, const QTime& h, const Tache& pred) const{
    if (pred.isTacheUnitaire()){
        const TacheUnitaire& predU = dynamic_cast<const TacheUnitaire&>(pred);
        if (programmationExists(trouverProgrammationT(predU)->getDate(), trouverProgrammationT(predU)->getHoraire(), trouverProgrammationT(predU)->getHoraireFin())){
            if (trouverProgrammationT(predU)->getDate() == d){
                if (trouverProgrammationT(predU)->getHoraireFin() > h) return false;
            }
            else if (trouverProgrammationT(predU)->getDate() > d) return false;
        }
        else return false;
    }
    else {
        const TacheComposite& predC = dynamic_cast<const TacheComposite&>(pred);
        TacheManager& TM = *TacheManager::getInstance();
        ListTachesConst composants = TM.tabParent.keys(&predC);
        for (int i = 0; i < composants.size(); ++i){
            isPrecedenceProgramme(d, h, *composants[i]);
        }
    }

    return true;
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
    // Contraintes de précédence, de disponibilité et d'échéance
    if (d < TU.getDateDisponibilite() || d > TU.getDateEcheance())
        throw CalendarException("erreur, ProgManager, incohérence programmation avec dates de disponibilité et d'échéance de la tâche");
    PrecedenceManager& PrM = *PrecedenceManager::getInstance();
    ListTachesConst pred = PrM.trouverPrecedences(TU);
    for (int i =0; i < pred.size(); ++i){
        if (!isPrecedenceProgramme(d, h, *pred[i])) throw CalendarException("Erreur, ProgManager, une tâche précédente au moins n'est pas encore accomplie sur ce créneau");
    }
    ProgrammationTache* PT = new ProgrammationTache(d, h, fin, TU);
    ajouterProgrammation(*PT);
}

void ProgManager::ajouterProgrammationA(const QDate& d, const QTime& h, const QTime& fin, const QString& t, const QString& desc, const QString& l){
    if (trouverProgrammationA(ProgrammationActivite(d, h, fin, t, desc, l))) {throw CalendarException("erreur, ProgManager, activité déjà existante");}
    ProgrammationActivite* PA = new ProgrammationActivite(d, h, fin, t, desc, l);
    ajouterProgrammation(*PA);
}

void ProgManager::supprimerProgrammationT(const TacheUnitaire& TU){
    if (trouverProgrammationT(TU)){
        for (int i = 0; i != programmations.size(); ++i)
            if (programmations[i] == trouverProgrammationT(TU))
                programmations.removeAt(i);
        tabDuree.remove(&TU);
        delete trouverProgrammationT(TU);
    }
}

void ProgManager::updateDuree(const TacheUnitaire& TU, QTime d){
    if (tabDuree.contains(&TU)){
        int dureeSec = QTime(0, 0).secsTo(d);
        tabDuree[&TU] = tabDuree[&TU].addSecs(dureeSec);
        if (tabDuree[&TU] > TU.getDuree()) throw CalendarException("erreur, ProgManager, durée écoulée > durée");
    }
    else tabDuree[&TU] = d;
}

bool ProgManager::programmationExists(const QDate& d, const QTime& h, const QTime& fin) const{
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

ListEventConst ProgManager::getProgWeek(const QDate& jour) const{
    ListEventConst LE;
    QDate first, last;
    first = jour.addDays(-jour.dayOfWeek()+1);
    last = first.addDays(6);
    for (int i = 0; i < programmations.size(); ++i){
        QDate date = programmations[i]->getDate();
        if (date >= first && date <= last) LE.append(programmations[i]);
    }
    return LE;
}

ListEventConst ProgManager::getProgProj(const Projet& projet) const{
    ListEventConst LE;
    for (int i = 0; i < programmations.size(); ++i){
        if (programmations[i]->isProgTache()){
            ProgrammationTache* prog = dynamic_cast<ProgrammationTache*>(programmations[i]);
            if (&prog->getProjet() == &projet) LE.append(programmations[i]);
        }
    }
    return LE;
}

void ProgManager::loadEvts(QXmlStreamReader& xml, QDate jour, const Projet *projet){
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

    if (activite){
        if ((jour.isNull() && projet == 0) || (!jour.isNull() && date >= jour && date <= jour.addDays(6)))
            ajouterProgrammationA(date, debut, fin, titre, description, lieu);
    }
    else{
        if ((jour.isNull() && projet == 0) || (!jour.isNull() && date >= jour && date <= jour.addDays(6)))
            ajouterProgrammationT(date, debut, fin, dynamic_cast<TacheUnitaire&>(TacheManager::getInstance()->getTache(tache)));
        else if (jour.isNull() && projet != 0){
            if (ProjetManager::getInstance()->getProjet(TacheManager::getInstance()->getTache(tache)) == projet)
                ajouterProgrammationT(date, debut, fin, dynamic_cast<TacheUnitaire&>(TacheManager::getInstance()->getTache(tache)));
        }
    }
}

void ProgManager::loadDurees(QXmlStreamReader& xml){
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

void ProgManager::save(QXmlStreamWriter& xml, QDate jour, const Projet* projet) const{
    // Sauvegarde des programmations
    ListEventConst LE;
    if (!jour.isNull()) LE = getProgWeek(jour);
    else if (projet != 0) LE = getProgProj(*projet);
    else{
        for (int i = 0; i < programmations.size(); ++i)
            LE.append(programmations[i]);
    }
    if (!LE.isEmpty()){
        xml.writeStartElement("programmations");
        for (int i = 0; i < LE.size(); ++i)
            LE[i]->save(xml);
        xml.writeEndElement();
    }

    // Sauvegarde des durées programmées
    if (!tabDuree.isEmpty()){
        xml.writeStartElement("durees");
        for (const_HashIterator<ProgManager, TacheUnitaire, QTime> it = tabDureeBegin(); it != tabDureeEnd(); ++it){
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
