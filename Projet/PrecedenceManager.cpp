#include "PrecedenceManager.h"
#include "TacheManager.h"

void PrecedenceManager::ajouterPrecedence(const Tache &Tpred, const Tache &Tsucc){
    if (Tpred.getDateDisponibilite() > Tsucc.getDateEcheance())
        throw CalendarException("erreur, PrecedenceManager, tentative de programmer une précédence avec une tache disponible après échéance de sa successeur.");
    for (int i = 0; i < precedences.size(); ++i) {
        if (&precedences[i]->getPredecesseur() == &Tpred && &precedences[i]->getSuccesseur() == &Tsucc)
            throw CalendarException("erreur, PrecedenceManager, précédence déjà existante");
        if (&precedences[i]->getPredecesseur() == &Tsucc && &precedences[i]->getSuccesseur() == &Tpred)
            throw CalendarException("erreur, PrecedenceManager, tentative d'implémenter un circuit de précédences");
    };
    Precedence* P = new Precedence(Tpred, Tsucc);
    precedences.append(P);
}

void PrecedenceManager::supprimerPrecedence(const Tache& Tpred, const Tache& Tsucc){
    if (isPrecedence(Tpred, Tsucc)){
        for (int i = 0; i < precedences.size(); ++i)
            if (&precedences[i]->getPredecesseur() == &Tpred && &precedences[i]->getSuccesseur() == &Tsucc)
                precedences.removeAt(i);
    }
    else throw CalendarException("erreur, PrecedenceManager, tentative d'effacer une precedence qui n'existe pas.");

}

ListTachesConst PrecedenceManager::trouverPrecedences(const Tache& Tsucc) const{
    ListTachesConst LT;
    for (int i = 0; i < precedences.size(); ++i)
        if (&precedences[i]->getSuccesseur() == &Tsucc)
            LT.append(&precedences[i]->getPredecesseur());
    return LT;
}

ListTachesConst PrecedenceManager::trouverSuccesseurs(const Tache& Tpred) const{
    ListTachesConst LT;
    for (int i = 0; i < precedences.size(); ++i)
        if (&precedences[i]->getPredecesseur() == &Tpred)
            LT.append(&precedences[i]->getSuccesseur());
    return LT;
}

bool PrecedenceManager::isPrecedence(const Tache& Tpred, const Tache& Tsucc) const {
    for (int i = 0; i < precedences.size(); ++i)
        if ((&precedences[i]->getPredecesseur() == &Tpred) && (&precedences[i]->getSuccesseur() == &Tsucc))
            return true;
    return false;
}

PrecedenceManager::~PrecedenceManager(){
    for (int i = 0; i < precedences.size(); ++i) delete precedences[i];
    precedences.clear();
}

void PrecedenceManager::load(QXmlStreamReader& xml){
    QString pred, succ;
    xml.readNext();

    while(!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == "precedence")){
        if(xml.tokenType() == QXmlStreamReader::StartElement){
            if(xml.name() == "pred"){
                xml.readNext();
                pred = xml.text().toString();
            }
            if(xml.name() == "succ"){
                xml.readNext();
                succ = xml.text().toString();
            }
        }
        xml.readNext();
    }
    ajouterPrecedence(TacheManager::getInstance()->getTache(pred), TacheManager::getInstance()->getTache(succ));
}

void PrecedenceManager::save(QXmlStreamWriter& xml) const{
    if (!precedences.isEmpty()){
        xml.writeStartElement("precedences");
        for (int i = 0; i < precedences.size(); ++i)
            precedences[i]->save(xml);
        xml.writeEndElement();
    }
}
