#ifndef STRATEGIES_H
#define STRATEGIES_H

#include <QTextCodec>
#include <QtXml>

#include <QFile>

#include "Calendar.h"

// ----- [BEGIN] Load Strategies -----

class Projet;

class LoadStrategy{
protected: QString file;
public:
    virtual void load(const QString& f) = 0;
    virtual void load(const QString& f, QDate day) = 0;
    virtual void load(const QString& f, const Projet* projet) = 0;

    virtual ~LoadStrategy(){}
};

class LoadXML : public LoadStrategy{
public:
    void load(const QString& f);
    void load(const QString& f, QDate day);
    void load(const QString& f, const Projet* projet);
};
class LoadTXT : public LoadStrategy{
public:
    void load(const QString&);
    void load(const QString& f, QDate);
    void load(const QString& f, const Projet*);
};

// ----- [END] Load Strategies -----

// ----- [BEGIN] Save Strategies -----

class SaveStrategy{
protected: QString file;
public:
    virtual void save(const QString& f, QDate day = QDate(), const Projet* projet = 0) = 0;
    virtual ~SaveStrategy(){}
};

class SaveXML : public SaveStrategy{
public:
    void save(const QString& f, QDate day = QDate(), const Projet* projet = 0);
};
class SaveTXT : public SaveStrategy{
public:
    void save(const QString& f, QDate day = QDate(), const Projet* projet = 0);
};

// ----- [END] Save Strategies -----


#endif // STRATEGIES_H
