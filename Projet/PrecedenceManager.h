#ifndef PRECEDENCEMANAGER_H
#define PRECEDENCEMANAGER_H

#include "Calendar.h"

class PrecedenceManager: public Singleton<PrecedenceManager>{
private:
    QList<Precedence*> precedences;
public:
    void ajouterPrecedence(const Tache& Tpred,const Tache& Tsucc);
    void supprimerPrecedence(const Tache& Tpred, const Tache& Tsucc);
    ListTachesConst trouverPrecedences(const Tache& Tsucc) const;
    bool isPrecedence(const Tache& Tpred, const Tache& Tsucc) const;
};

#endif // PRECEDENCEMANAGER_H
