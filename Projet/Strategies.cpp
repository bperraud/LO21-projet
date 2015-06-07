#include "Strategies.h"

//#include "ProjetManager"
#include "TacheManager.h"
#include "ProgManager.h"
//#include "PrecedenceManager"

// ----- [BEGIN] Load Strategies -----

void LoadXML::load(const QString& f){

    qDebug() << f;




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
