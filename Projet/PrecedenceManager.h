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
    bool isPrecedence(const Tache& Tpred, const Tache& Tsucc) const;

    void load(QXmlStreamReader& xml);
    void save(QXmlStreamWriter& xml) const;

    bool isEmpty() const { return precedences.isEmpty(); }
};

#endif // PRECEDENCEMANAGER_H
