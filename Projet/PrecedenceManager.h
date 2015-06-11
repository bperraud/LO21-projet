#ifndef PRECEDENCEMANAGER_H
#define PRECEDENCEMANAGER_H

#include "Calendar.h"

class PrecedenceManager: public Singleton<PrecedenceManager>{
private:
    QList<Precedence*> precedences;
public:
    ~PrecedenceManager();

    void ajouterPrecedence(const Tache& Tpred,const Tache& Tsucc);
    void supprimerPrecedence(const Tache& Tpred, const Tache& Tsucc);
    ListTachesConst trouverPrecedences(const Tache& Tsucc) const;
    ListTachesConst trouverSuccesseurs(const Tache& Tpred) const;
    bool isPrecedence(const Tache& Tpred, const Tache& Tsucc) const;

    void load(QXmlStreamReader& xml);
    void save(QXmlStreamWriter& xml) const;

    bool isEmpty() const { return precedences.isEmpty(); }

    Iterator<PrecedenceManager, Precedence> begin(){ return Iterator<PrecedenceManager, Precedence>(precedences.begin()); }
    Iterator<PrecedenceManager, Precedence> end(){ return Iterator<PrecedenceManager, Precedence>(precedences.end()); }

    const_Iterator<PrecedenceManager, Precedence> begin() const { return const_Iterator<PrecedenceManager, Precedence>(precedences.begin()); }
    const_Iterator<PrecedenceManager, Precedence> end() const { return const_Iterator<PrecedenceManager, Precedence>(precedences.end()); }
};

#endif // PRECEDENCEMANAGER_H
