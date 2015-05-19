#include "TacheManager.h"
#include "CalendarException.h"

/* ----- [BEGIN]TacheManager ----- */

Tache* TacheManager::trouverTache(const QString& titre)const{
    for (int i = 0; i < taches.size(); ++i){
        if (titre == (taches.at(i))->getTitre()) return taches[i];
    }
    return 0;
}

void TacheManager::ajouterTache(Tache& T){
    if (trouverTache(T.getTitre())) {throw CalendarException("erreur, TacheManager, tache deja existante");}
    taches.append(&T);
}

TacheUnitaire& TacheManager::ajouterTacheUnitaire(const QString& t, const QString& desc, const Duree& dur, const QDate& dispo, const QDate& deadline, bool preempt){
    TacheUnitaire* TU = new TacheUnitaire(t, desc, dur, dispo, deadline, preempt);
    ajouterTache(*TU);
    return *TU;
}

TacheComposite& TacheManager::ajouterTacheComposite(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const ListTaches& sT){
    TacheComposite* TC = new TacheComposite(t, desc, dispo, deadline, sT);
    ajouterTache(*TC);
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

TacheManager::~TacheManager(){
    if (file!="") save(file);
    for (int i = 0; i < taches.size(); ++i) delete taches[i];
    file="";
}

void TacheManager::load(const QString& f){
    //qDebug()<<"debut load\n";
    this->~TacheManager();
    file=f;
    QFile fin(file);

    QList<HierarchyTachesC> TachesC;
    // If we can't open it, let's show an error message.
    if (!fin.open(QIODevice::ReadOnly | QIODevice::Text))
        throw CalendarException("Erreur ouverture fichier tâches");
    // QXmlStreamReader takes any QIODevice.
    QXmlStreamReader xml(&fin);
    //qDebug()<<"debut fichier\n";
    // We'll parse the XML until we reach end of it.
    while(!xml.atEnd() && !xml.hasError()){
        // Read next element.
        QXmlStreamReader::TokenType token = xml.readNext();
        // If token is just StartDocument, we'll go to next.
        if(token == QXmlStreamReader::StartDocument) continue;
        // If token is StartElement, we'll see if we can read it.
        if(token == QXmlStreamReader::StartElement) {
            // If it's named taches, we'll go to the next.
            if(xml.name() == "taches") continue;
            // If it's named tache, we'll dig the information from there.
            if(xml.name() == "tache"){
                //qDebug()<<"new tache\n";
                QString titre;
                QString description;
                QDate disponibilite;
                QDate echeance;
                Duree duree;
                bool preemptive;

                bool unitaire = true;

                QXmlStreamAttributes attributes = xml.attributes();
                /* Let's check that Task has attribute. */
                if(attributes.hasAttribute("preemptive")){
                    QString val = attributes.value("preemptive").toString();
                    preemptive= (val == "true" ? true : false);
                }
                //qDebug()<<"preemptive="<<preemptive<<"\n";

                xml.readNext();
                //We're going to loop over the things because the order might change.
                //We'll continue the loop until we hit an EndElement named tache.


                while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "tache")){
                    if(xml.tokenType() == QXmlStreamReader::StartElement){
                        // We've found identificateur
                        if(xml.name() == "titre"){
                            xml.readNext(); titre=xml.text().toString();
                            //qDebug()<<"id="<<titre<<"\n";
                        }

                        // We've found description
                        if(xml.name() == "description"){
                            xml.readNext(); description=xml.text().toString();
                            //qDebug()<<"description="<<description<<"\n";
                        }
                        // We've found disponibilite
                        if(xml.name() == "disponibilite"){
                            xml.readNext();
                            disponibilite=QDate::fromString(xml.text().toString(),Qt::ISODate);
                            //qDebug()<<"disp="<<disponibilite.toString()<<"\n";
                        }
                        // We've found echeance
                        if(xml.name() == "echeance"){
                            xml.readNext();
                            echeance=QDate::fromString(xml.text().toString(),Qt::ISODate);
                            //qDebug()<<"echeance="<<echeance.toString()<<"\n";
                        }
                        // We've found duree
                        if(xml.name() == "duree"){
                            xml.readNext();
                            duree.setDuree(xml.text().toString().toUInt());
                            //qDebug()<<"duree="<<duree.getDureeEnMinutes()<<"\n";
                        }
                        // We've found sous-tache
                        if(xml.name() == "sous-tache"){
                            xml.readNext();
                            TachesC.append(HierarchyTachesC(titre, xml.text().toString()));
                            unitaire = false;
                            //sousTaches.append(&getTache(xml.text().toString()));
                            //qDebug()<<"sous-tache\n";
                        }

                    }
                    // ...and next...
                    xml.readNext();
                }
                //qDebug()<<"ajout tache "<<titre<<"\n";
                // Tache unitaire
                if(unitaire){
                    ajouterTacheUnitaire(titre, description, duree, disponibilite, echeance, preemptive);
                    //ajouterTacheUnitaire(titre,description,duree,disponibilite,echeance,preemptive);
                }
                // Tache composite
                else{
                    ajouterTacheComposite(titre, description, disponibilite, echeance, ListTaches());
                    //ajouterTacheComposite(titre,description,disponibilite,echeance,ListTaches());
                    unitaire = true;
                }
            }
        }
    }
    // Error handling.
    if(xml.hasError())
        throw CalendarException("Erreur lecteur fichier taches, parser xml");
    // Removes any device() or data from the reader * and resets its internal state to the initial state.
    xml.clear();
    // Traitement des taches composites (ajout de leurs sous-taches
    for (int i = 0; i < TachesC.size(); ++i)
        dynamic_cast<TacheComposite&>(getTache(TachesC[i].mere)).addSousTache(&getTache(TachesC[i].fille));
    //qDebug()<<"fin load\n";
}

void  TacheManager::save(const QString& f){
    file=f;
    QFile newfile( file);
    if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text))
        throw CalendarException(QString("erreur sauvegarde tâches : ouverture fichier xml"));
    QXmlStreamWriter stream(&newfile);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("taches");
    for (int i = 0; i < taches.size(); ++i)
        taches[i]->saveTache(stream);
    stream.writeEndElement();
    stream.writeEndDocument();
    newfile.close();
}




/* ----- [END]TacheManager ----- */
