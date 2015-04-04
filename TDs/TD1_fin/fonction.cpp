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
    //*pt5=2.1; erreur : pointeur const, valeur point�e non modifiable
    //pt4=&d2; conversion implicite de const double* vers double* impossible
    pt5=&d2;
    double* const pt6=&d1; // pt6 est un pointeur constant
    //pt6=&d1; il ne peut pas �tre affect�
    *pt6=2.78; // � partir d'un pointeur constant, on peut modifier la valeur point�e
    //double* const pt6b=&d2; pt6b n'est pas un pointeur const (c'est un pointeur constant), on ne peut donc pas y stocker l'adresse d'une constante
    const double* const pt7=&d1;
    //pt7=&d1; pointeur constant => on ne peut pas changer sa valeur
    //*pt7=2.78; pointeur const => on ne peut pas modifier la valeur point�e
    double const* pt8=&d1; // �quivalent � const double*
    pt8=&d2;
    pt8=&d1;
    //*pt8=3.14; valeur point�e non modifiable
}

void exercice11(){
    //double& d1=36; erreur car 36 n'est pas une lvalue stock�e en m�moire (n'a pas d'adresse donc ne peut pas �tre r�f�renc�)
    double d2=36;
    double& ref=d2;
    ref=4;
    const double d3=36;
    const double& d4=36; //un espace m�moire temp va �tre utilis� pour stocker la valeur 36; c'est cet espace m�moire qui est r�f�renc�. On note que d4 est une r�f�rence const : on ne peut donc pas modifier la valeur r�f�renc�e
    const double& d5=d2; //d5 est une r�f  const
    //d5=21; on ne peut donc pas modifier la valeur point�e
    const double& d6=d3;
    //double& ref2=d6; d6 est une constante, conversion de const double& vers double& impossible
    int i=4;
    //double& d7= i; conversion int& => double& impossible, pour init une r�f, il faut une lvalue du m�me type
    const double& d8=i; //avec les r�f�rences const, les conversions habituelles sont autoris�es. Cependant, ce n'est pas la lvalue i qui est r�ellement r�f�renc�e : un espace m�moire est temporairement utilis�e pour y stocker le r�sultat de la conversion de i en double; c'est cet espace m�moire qui sera r�f�renc�.
    //d8=3; d8 est une r�f  const
}
