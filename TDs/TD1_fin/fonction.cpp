#include <iostream>
#include "fonction.h"

void exercice_surcharge(){
int i=3,j=15;
float x=3.14159,y=1.414;
char c='A';
double z=3.14159265;
fct(i); //appel 1
fct(x); //appel 2
fct(i,y); //appel 3
fct(x,j); //appel 4
fct(c); //appel 5
fct(i,(float)j); //appel 6
fct((float)i,c); //appel 7
fct(i,z); //appel 8*/
fct(z,(float)z); //appel 9
}

void exercice10(){
    double* pt1=reinterpret_cast<double*>(4096);
    double* pt2=reinterpret_cast<double*>(4096);
    void* pt3=pt1; // conversion implicite de double* vers void* ok
    pt1=static_cast<double*>(pt3);
    pt1=static_cast<double*>(pt3);
    double d1=36;
    const double d2=36;
    double* pt4=&d1;
    const double* pt5=&d1;
    *pt4=2.1; // d1=2.1
    //*pt5=2.1; erreur : pointeur const, valeur pointée non modifiable
    //pt4=&d2; conversion implicite de const double* vers double* impossible
    pt5=&d2;
    double* const pt6=&d1; // pt6 est un pointeur constant
    //pt6=&d1; il ne peut pas être affecté
    *pt6=2.78; // à partir d'un pointeur constant, on peut modifier la valeur pointée
    //double* const pt6b=&d2; pt6b n'est pas un pointeur const (c'est un pointeur constant), on ne peut donc pas y stocker l'adresse d'une constante
    const double* const pt7=&d1;
    //pt7=&d1; pointeur constant => on ne peut pas changer sa valeur
    //*pt7=2.78; pointeur const => on ne peut pas modifier la valeur pointée
    double const* pt8=&d1; // équivalent à const double*
    pt8=&d2;
    pt8=&d1;
    //*pt8=3.14; valeur pointée non modifiable
}

void exercice11(){
    //double& d1=36; erreur car 36 n'est pas une lvalue stockée en mémoire (n'a pas d'adresse donc ne peut pas être référencé)
    double d2=36;
    double& ref=d2;
    ref=4;
    const double d3=36;
    const double& d4=36; //un espace mémoire temp va être utilisé pour stocker la valeur 36; c'est cet espace mémoire qui est référencé. On note que d4 est une référence const : on ne peut donc pas modifier la valeur référencée
    const double& d5=d2; //d5 est une réf  const
    //d5=21; on ne peut donc pas modifier la valeur pointée
    const double& d6=d3;
    //double& ref2=d6; d6 est une constante, conversion de const double& vers double& impossible
    int i=4;
    //double& d7= i; conversion int& => double& impossible, pour init une réf, il faut une lvalue du même type
    const double& d8=i; //avec les références const, les conversions habituelles sont autorisées. Cependant, ce n'est pas la lvalue i qui est réellement référencée : un espace mémoire est temporairement utilisée pour y stocker le résultat de la conversion de i en double; c'est cet espace mémoire qui sera référencé.
    //d8=3; d8 est une réf  const
}
