#include "Strategies.h"
#include "TacheManager.h"
#include "ProgManager.h"
#include "ProjetManager.h"
#include "PrecedenceManager.h"

// ----- [BEGIN] Load Strategies -----

void LoadXML::load(const QString& f){

    // Cleaning
    ProgManager::getInstance()->~ProgManager();
    ProjetManager::getInstance()->~ProjetManager();
    PrecedenceManager::getInstance()->~PrecedenceManager();
    TacheManager::getInstance()->~TacheManager();

    // Load
    file = f;
    QFile fin(file);
    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text))
        throw CalendarException("Erreur ouverture fichier");
    QXmlStreamReader xml(&fin);

    while(!xml.atEnd() && !xml.hasError()){
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartDocument) continue;
        if(token == QXmlStreamReader::StartElement){
            if(xml.name() == "calendar") continue;
            if(xml.name() == "taches") continue;
            if(xml.name() == "tache") TacheManager::getInstance()->loadTaches(xml);
            if(xml.name() == "hierarchieT") continue;
            if(xml.name() == "linkT") TacheManager::getInstance()->loadHierarchie(xml);
            if(xml.name() == "precedences") continue;
            if(xml.name() == "precedence") PrecedenceManager::getInstance()->load(xml);
            if(xml.name() == "projets") continue;
            if(xml.name() == "projet") ProjetManager::getInstance()->loadProjets(xml);
            if(xml.name() == "hierarchieP") continue;
            if(xml.name() == "linkP") ProjetManager::getInstance()->loadHierarchie(xml);
            if(xml.name() == "programmations") continue;
            if(xml.name() == "evenement") ProgManager::getInstance()->loadEvts(xml);
            if(xml.name() == "durees") continue;
            if(xml.name() == "dureeT") ProgManager::getInstance()->loadDurees(xml);
        }
    }
    if(xml.hasError()) throw CalendarException("Erreur lecteur fichier, parser xml");
    xml.clear();
}

void LoadXML::load(const QString& f, QDate jour){
    // Cleaning
    ProgManager::getInstance()->~ProgManager();

    // Load
    file = f;
    QFile fin(file);
    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text))
        throw CalendarException("Erreur ouverture fichier");
    QXmlStreamReader xml(&fin);

    while(!xml.atEnd() && !xml.hasError()){
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartDocument) continue;
        if(token == QXmlStreamReader::StartElement){
            if(xml.name() == "calendar") continue;
            if(xml.name() == "taches") continue;
            if(xml.name() == "tache") continue;
            if(xml.name() == "hierarchieT") continue;
            if(xml.name() == "linkT") continue;
            if(xml.name() == "precedences") continue;
            if(xml.name() == "precedence") continue;
            if(xml.name() == "projets") continue;
            if(xml.name() == "projet") continue;
            if(xml.name() == "hierarchieP") continue;
            if(xml.name() == "linkP") continue;
            if(xml.name() == "programmations") continue;
            if(xml.name() == "evenement") ProgManager::getInstance()->loadEvts(xml, jour);
            if(xml.name() == "durees") continue;
            if(xml.name() == "dureeT") ProgManager::getInstance()->loadDurees(xml);
        }
    }
    if(xml.hasError()) throw CalendarException("Erreur lecteur fichier , parser xml");
    xml.clear();
}

void LoadXML::load(const QString& f, const Projet* projet){
    // Cleaning
    ProgManager::getInstance()->~ProgManager();

    // Load
    file = f;
    QFile fin(file);
    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text))
        throw CalendarException("Erreur ouverture fichier");
    QXmlStreamReader xml(&fin);

    while(!xml.atEnd() && !xml.hasError()){
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartDocument) continue;
        if(token == QXmlStreamReader::StartElement){
            if(xml.name() == "calendar") continue;
            if(xml.name() == "taches") continue;
            if(xml.name() == "tache") continue;
            if(xml.name() == "hierarchieT") continue;
            if(xml.name() == "linkT") continue;
            if(xml.name() == "precedences") continue;
            if(xml.name() == "precedence") continue;
            if(xml.name() == "projets") continue;
            if(xml.name() == "projet") continue;
            if(xml.name() == "hierarchieP") continue;
            if(xml.name() == "linkP") continue;
            if(xml.name() == "programmations") continue;
            if(xml.name() == "evenement") ProgManager::getInstance()->loadEvts(xml, QDate(), projet);
            if(xml.name() == "durees") continue;
            if(xml.name() == "dureeT") ProgManager::getInstance()->loadDurees(xml);
        }
    }
    if(xml.hasError()) throw CalendarException("Erreur lecteur fichier , parser xml");
    xml.clear();
}

void LoadTXT::load(const QString&){}
void LoadTXT::load(const QString&, QDate){}
void LoadTXT::load(const QString&, const Projet*){}

// ----- [END] Load Strategies -----

// ----- [BEGIN] Save Strategies -----

void SaveXML::save(const QString& f, QDate day, const Projet* projet){
    TacheManager& TM = *TacheManager::getInstance();
    PrecedenceManager& PreM = *PrecedenceManager::getInstance();
    ProjetManager& PM = *ProjetManager::getInstance();
    ProgManager& ProgM = *ProgManager::getInstance();

    file = f;
    QFile newfile(file);

    if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text))
        throw CalendarException(QString("Erreur sauvegarde XML : ouverture fichier xml"));

    QXmlStreamWriter xml(&newfile);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();
        if (!(TM.isEmpty() || PreM.isEmpty() || PM.isEmpty() || ProgM.isEmpty())){
            xml.writeStartElement("calendar");
                // Sauvegarde des tâches
                TM.save(xml);
                // Sauvegarde des contraintes de précédence
                PreM.save(xml);
                // Sauvegarde des projets
                PM.save(xml);
                // Sauvegarde des programmations
                ProgM.save(xml, day, projet);
            xml.writeEndElement();
        }
    xml.writeEndDocument();
    newfile.close();
}



void SaveTXT::save(const QString&, QDate, const Projet*){}


// ----- [END] Save Strategies -----
