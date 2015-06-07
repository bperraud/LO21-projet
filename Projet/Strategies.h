#ifndef STRATEGIES_H
#define STRATEGIES_H

#include <QTextCodec>
#include <QtXml>

#include <QFile>

#include "Calendar.h"

// ----- [BEGIN] Load Strategies -----

class LoadStrategy{
protected:
    QString file;
public:
    virtual void load(const QString& f) = 0;
    virtual ~LoadStrategy(){}
};

class LoadXML : public LoadStrategy{
public:
    void load(const QString& f);
};
class LoadTXT : public LoadStrategy{
public:
    void load(const QString& f);
};

// ----- [END] Load Strategies -----

// ----- [BEGIN] Save Strategies -----

class SaveStrategy{
protected:
    QString file;
public:
    virtual void save(const QString& f) = 0;
    virtual ~SaveStrategy(){}
};

class SaveXML : public SaveStrategy{
public:
    void save(const QString& f);
};
class SaveTXT : public SaveStrategy{
public:
    void save(const QString& f);
};

// ----- [END] Save Strategies -----


#endif // STRATEGIES_H
