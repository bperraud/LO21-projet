#include "Strategies.h"
#include "TacheManager.h"
#include "ProgManager.h"
#include "ProjetManager.h"
//#include "PrecedenceManager.h"

// ----- [BEGIN] Load Strategies -----

void LoadXML::load(const QString& f){

    // Cleaning
    TacheManager::getInstance()->~TacheManager();
    ProjetManager::getInstance()->~ProjetManager();
    ProgManager::getInstance()->~ProgManager();

    // Load
    file = f;
    QFile fin(file);

    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text))
        throw CalendarException("Erreur ouverture fichier tâches");

    QXmlStreamReader xml(&fin);

    while(!xml.atEnd() && !xml.hasError()){


        QXmlStreamReader::TokenType token = xml.readNext();

        if(token == QXmlStreamReader::StartDocument) continue;
        if(token == QXmlStreamReader::StartElement){
            if(xml.name() == "calendar") continue;
            if(xml.name() == "taches") continue;
            if(xml.name() == "tache") TacheManager::getInstance()->load1(xml);
            if(xml.name() == "hierarchieT") continue;
            if(xml.name() == "linkT") TacheManager::getInstance()->load2(xml);
            if(xml.name() == "projets") continue;
            if(xml.name() == "projet") ProjetManager::getInstance()->load1(xml);
            if(xml.name() == "hierarchieP") continue;
            if(xml.name() == "linkP") ProjetManager::getInstance()->load2(xml);
        }

    }
    if(xml.hasError()) throw CalendarException("Erreur lecteur fichier taches, parser xml");
    xml.clear();


}


void LoadTXT::load(const QString& f){ qDebug() << "Load TXT" << f; }

// ----- [END] Load Strategies -----

// ----- [BEGIN] Save Strategies -----

void SaveXML::save(const QString& f){
    TacheManager& TM = *TacheManager::getInstance();
    ProjetManager& PM = *ProjetManager::getInstance();

    file = f;
    QFile newfile(file);

    if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text))
        throw CalendarException(QString("Erreur sauvegarde XML : ouverture fichier xml"));

    QXmlStreamWriter xml(&newfile);
    xml.setAutoFormatting(true);

    xml.writeStartDocument();
        xml.writeStartElement("calendar");
            // Sauvegarde des tâches
            TM.save1(xml);
            // Sauvegarde des projets
            PM.save(xml);
        xml.writeEndElement();
    xml.writeEndDocument();
    newfile.close();
}


void SaveTXT::save(const QString& f){ qDebug() << "Save TXT" << f; }

// ----- [END] Save Strategies -----
