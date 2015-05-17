#include <typeinfo>

#include "Calendar.h"

#include <QFile>
#include <QTextCodec>
#include <QtXml>
#include <QMessageBox>



QTextStream& operator<<(QTextStream& fout, const Tache& t){
    fout<<t.getTitre()<<"\n";
    fout<<t.getDescription()<<"\n";
    fout<<t.getDateDisponibilite().toString()<<"\n";
    fout<<t.getDateEcheance().toString()<<"\n";
	return fout;
}

QTextStream& operator<<(QDataStream& f, const ProgrammationTache& p);



void Tache::setTitre(const QString& str){
  if (TacheManager::getInstance().isTacheExistante((str))) throw CalendarException("erreur TacheManager : tache id déjà existante");
  titre=str;
}

void TacheComposite::setSousTaches(const ListTaches &sT){
    for (int i = 0; i < sT.size(); ++i)
        if (!TacheManager::getInstance().isTacheExistante(sT[i]->getTitre()))
            throw CalendarException("Erreur tâche composite, tâche unitaire non trouvée");
    sousTaches = sT;
}

void TacheComposite::addSousTache(const Tache* t){
    for (int i = 0; i < sousTaches.size(); ++i)
        if (sousTaches[i] == t) throw CalendarException("erreur, TacheComposite, tâche déjà existante");
    sousTaches.append(const_cast<Tache*>(t));
}

void TacheComposite::rmSousTache(const Tache* t){
    for (int i = 0; i < sousTaches.size(); ++i){
        if (sousTaches[i] == t){
            sousTaches.removeAt(i);
            return;
        }
    }
    throw CalendarException("erreur, TacheComposite, tâche à supprimer non trouvée");
}

/* ----- [BEGIN]TacheManager ----- */

Tache* TacheManager::trouverTache(const QString& titre)const{
    for (int i = 0; i < taches.size(); ++i){
        if (titre == (taches.at(i))->getTitre()) return taches[i];
    }
	return 0;
}

Tache& TacheManager::ajouterTache(Tache& T){
    if (trouverTache(T.getTitre())) {throw CalendarException("erreur, TacheManager, tache deja existante");}
    taches.append(&T);
    return T;
}

TacheUnitaire& TacheManager::ajouterTacheUnitaire(TacheUnitaire& TU){
    return dynamic_cast<TacheUnitaire&>(ajouterTache(TU));
}

TacheComposite& TacheManager::ajouterTacheComposite(TacheComposite& TC){
    return dynamic_cast<TacheComposite&>(ajouterTache(TC));
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
                    ajouterTache(FabriqueTacheU::getInstance().creerTacheU(titre, description, duree, disponibilite, echeance, preemptive));
                    //ajouterTacheUnitaire(titre,description,duree,disponibilite,echeance,preemptive);
                }
                // Tache composite
                else{
                    ajouterTache(FabriqueTacheC::getInstance().creerTacheC(titre, description, disponibilite, echeance, ListTaches()));
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

TacheManager::Handler TacheManager::handler=TacheManager::Handler();

TacheManager& TacheManager::getInstance(){
    if (handler.instance==0) handler.instance=new TacheManager;
    return *(handler.instance);
}

void TacheManager::libererInstance(){
    if (handler.instance!=0) delete handler.instance;
    handler.instance=0;
}


/* ----- [END]TacheManager ----- */

//******************************************************************************************
// ProgTacheManager



// Classe Singleton //

Singleton::Handler Singleton::handler = Singleton::Handler();
Singleton& Singleton::getInstance(){
    if (handler.instance==0) handler.instance=new Singleton;
    return *(handler.instance);
}
void Singleton::libererInstance(){
    if (handler.instance!=0) delete handler.instance;
    handler.instance=0;
}


ProgrammationTache* ProgTacheManager::trouverProgrammation(const TacheUnitaire& TU) const{
    for (int i = 0; i < programmations.size(); ++i)
        if (&TU == &programmations[i]->getTache()) return programmations[i];
    return 0;
}

void ProgTacheManager::ajouterProgrammation(const QDate& d, const QTime& h, const TacheUnitaire& TU){
if (trouverProgrammation(TU) && !TU.isPreemptive()) {throw CalendarException("erreur, ProgTacheManager, tâche non préemptive déjà existante");}
// Rajouter les contraintes de précédence, de préemption, de disponibilité et d'échéance
ProgrammationTache* PT = new ProgrammationTache(d, h, TU);
programmations.append(PT);
}



ProgrammationActivite* ProgActiviteManager::trouverProgrammation(const ProgrammationActivite& PA) const{
    for (int i = 0; i < programmations.size(); ++i)
        if (&PA == programmations[i]) return programmations[i];
    return 0;
}

void ProgActiviteManager::ajouterProgrammation(const QDate& d, const QTime& h, const QString& t, const QString& desc, const QString& l){
if (trouverProgrammation(ProgrammationActivite(d, h, t, desc, l))) {throw CalendarException("erreur, ProgActiviteManager, activité déjà existante");}
ProgrammationActivite* PA = new ProgrammationActivite(d, h, t, desc, l);
programmations.append(PA);
}




/* --- Design Pattern Visitor --- */

void TacheUnitaire::accept(TacheVisitor* v){
    v->visitTacheUnitaire(this);
}
void TacheComposite::accept(TacheVisitor* v){
    v->visitTacheComposite(this);
}

void TacheInformateur::visitTacheUnitaire(TacheUnitaire* TU){
    qDebug() << "durée en min : " << QString(QString::number(static_cast<int>(TU->getDuree().getDureeEnMinutes()))) << "\n";
}
void TacheInformateur::visitTacheComposite(TacheComposite* TC){
    qDebug() << "titre tâche composite : " << QString(TC->getTitre()) << "\n";
}

/* --- Design Pattern Abstract Factory --- */

FabriqueTacheU::Handler FabriqueTacheU::handler=FabriqueTacheU::Handler();

FabriqueTacheU& FabriqueTacheU::getInstance(){
    if (handler.instance==0) handler.instance=new FabriqueTacheU;
    return *(handler.instance);
}

void FabriqueTacheU::libererInstance(){
    if (handler.instance!=0) delete handler.instance;
    handler.instance=0;
}

FabriqueTacheC::Handler FabriqueTacheC::handler=FabriqueTacheC::Handler();

FabriqueTacheC& FabriqueTacheC::getInstance(){
    if (handler.instance==0) handler.instance=new FabriqueTacheC;
    return *(handler.instance);
}

void FabriqueTacheC::libererInstance(){
    if (handler.instance!=0) delete handler.instance;
    handler.instance=0;
}
