#include "Strategies.h"

//#include "ProjetManager"
#include "TacheManager.h"
#include "ProgManager.h"
//#include "PrecedenceManager"

// ----- [BEGIN] Load Strategies -----

void LoadXML::load(const QString& f){qDebug() << "debut LoadXML::load";

    TacheManager::getInstance()->~TacheManager();
    ProgManager::getInstance()->~ProgManager();

    file = f;
    QFile fin(file);

    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text))
        throw CalendarException("Erreur ouverture fichier tâches");

    QXmlStreamReader xml(&fin);

    while(!xml.atEnd() && !xml.hasError()){
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartDocument) continue;
        if(token == QXmlStreamReader::StartElement){
            if(xml.name() == "taches") continue;
            if(xml.name() == "tache"){

                TacheManager::getInstance()->load1(xml);

            }

        }
    }
    if(xml.hasError())
        throw CalendarException("Erreur lecteur fichier taches, parser xml");
    xml.clear();
    // Traitement des taches composites
    TacheManager& TM = *TacheManager::getInstance();
    for (QHash<QString, QString>::const_iterator i = TM.tabParent.constBegin(); i != TM.tabParent.constEnd(); ++i)
        dynamic_cast<TacheComposite&>(TM.getTache(i.value())).addSousTache(&TM.getTache(i.key()));
    qDebug() << "fin LoadXML::load";
}


void LoadTXT::load(const QString& f){ qDebug() << "Load TXT" << f; }

// ----- [END] Load Strategies -----

// ----- [BEGIN] Save Strategies -----

void SaveXML::save(const QString& f){
    TacheManager& TM = *TacheManager::getInstance();

    file = f;
    QFile newfile(file);

    if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text))
        throw CalendarException(QString("Erreur sauvegarde XML : ouverture fichier xml"));

    QXmlStreamWriter stream(&newfile);
    stream.setAutoFormatting(true);

    // Début document
    stream.writeStartDocument();

    // Sauvegarde des tâches

    stream.writeStartElement("taches");
    for (TacheManager::iterator it = TM.begin(); it != TM.end(); ++it)
        (*it).save(stream);
    stream.writeEndElement();

    stream.writeEndDocument();
    newfile.close();





}


void SaveTXT::save(const QString& f){ qDebug() << "Save TXT" << f; }

// ----- [END] Save Strategies -----
