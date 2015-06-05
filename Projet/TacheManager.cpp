#include "TacheManager.h"

Tache* TacheManager::trouverTache(const QString& titre) const{
    //qDebug() << "début trouverTache\n";
    //qDebug() << "taches.size :" << QString::number(taches.size()) << "\n";
    for (int i = 0; i < taches.size(); ++i){
        if (titre == (taches.at(i))->getTitre()) return taches[i];
    }
    return 0;
}

void TacheManager::ajouterTache(Tache& T){
    //qDebug() << "début ajouterTache\n";
    //qDebug() << T.getTitre() << "\n";
    if (trouverTache(T.getTitre())) {throw CalendarException("erreur, TacheManager, tâche déjà existante");}
    taches.append(&T);
}

TacheUnitaire& TacheManager::ajouterTacheUnitaire(const QString& t, const QString& desc, const QTime& dur, const QDate& dispo, const QDate& deadline, bool preempt){
    //qDebug() << "début ajouterTacheUnitaire\n";
    TacheUnitaire* TU = new TacheUnitaire(t, desc, dur, dispo, deadline, preempt);
    ajouterTache(*TU);
    return *TU;
}

TacheComposite& TacheManager::ajouterTacheComposite(const QString& t, const QString& desc, const QDate& dispo, const QDate& deadline, const ListTaches& sT){
    TacheComposite* TC = new TacheComposite(t, desc, dispo, deadline, sT);
    ajouterTache(*TC);
    for (int i = 0; i < sT.size(); ++i){
        if (tabParent.contains(sT[i]->getTitre()))
            if (tabParent.value(sT[i]->getTitre()) != TC->getTitre())
                throw CalendarException("erreur, TacheManager::ajouterTacheComposite, sous-tâche déjà subordonnée à une autre tâche");
        tabParent.insert(sT[i]->getTitre(), TC->getTitre());
    }
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


Tache* TacheManager::getTacheMere(const Tache& t){
    if (tabParent.contains(t.getTitre()))
        return &getTache(tabParent.value(t.getTitre()));
    return 0;
}

const Tache* TacheManager::getTacheMere(const Tache& t)const{
    return const_cast<TacheManager*>(this)->getTacheMere(t);
}

TacheManager::~TacheManager(){
    //if (file!="") save(file);
    for (int i = 0; i < taches.size(); ++i) delete taches[i];
    file="";
}

void TacheManager::load(const QString& f){
    qDebug()<<"debut load\n";
    this->~TacheManager();
    file=f;
    QFile fin(file);

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
                QTime duree;
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
                            qDebug()<<"id="<<titre<<"\n";
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
                            duree.setHMS((xml.text().toInt())/60,(xml.text().toInt())%60,0);
                            //qDebug()<<"duree="<<duree.getDureeEnMinutes()<<"\n";
                            qDebug() << "durée : " << duree.toString("hh'h'mm") << "\n";
                        }
                        // We've found sous-tache
                        if(xml.name() == "sous-tache"){
                            xml.readNext();
                            //hierarchieTemp.append(new HierarchyTachesC(titre, xml.text().toString()));
                            tabParent.insert(xml.text().toString(), titre);
                            unitaire = false;
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
                }
                // Tache composite
                else{
                    ajouterTacheComposite(titre, description, disponibilite, echeance);
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
    // Traitement des taches composites (ajout de leurs sous-taches)
    for (QHash<QString, QString>::const_iterator i = tabParent.constBegin(); i != tabParent.constEnd(); ++i)
        dynamic_cast<TacheComposite&>(getTache(i.value())).addSousTache(&getTache(i.key()));
    qDebug()<<"fin load\n";
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
