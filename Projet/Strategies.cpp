#include "Strategies.h"

// ----- [BEGIN] Load Strategies -----

void LoadXML::load(const QString& f){

    qDebug() << f;




}


void LoadTXT::load(const QString& f){ qDebug() << "Load TXT" << f; }

// ----- [END] Load Strategies -----

// ----- [BEGIN] Save Strategies -----

void SaveXML::save(const QString& f){

    qDebug() << f;




}


void SaveTXT::save(const QString& f){ qDebug() << "Save TXT" << f; }

// ----- [END] Save Strategies -----
