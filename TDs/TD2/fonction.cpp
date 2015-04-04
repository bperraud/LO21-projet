#include <iostream>
#include <string>
#include <cstring>
#include "fonction.h"

void inverse(int* a, int* b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void inverse(int& a, int& b){
    int tmp = a;
    a = b;
    b = tmp;
}

void raz(essai* e){
    e->n = 0;
    e->x = 0;
}

void raz(essai& e){
    e.n = 0;
    e.x = 0;
}

//exercice 14

void truc1(const int& x){
    std::cout<<x<<"\n";
    //x++; pas de modif à partir d'une réf const
    std::cout<<x<<"\n";
}
void truc2(int& x){
    x++;
    truc1(x); //conversion implicite de int& vers const int& possible
    x++;
}
void truc3(const int& x){
    //truc2(x); conversion de const int& vers int& impossible
    truc1(x);
    double y=x;
    //double& z=x; il n'y a pas de correspondance de type
    double& u=y;
    //double& t=y-3; y-3 n'est pas une lvalue
}

//exercice 15

void point::init(int _x, int _y, int _z){
x=_x; y=_y; z=_z;
}

void essai_init(){
using namespace std;
point p;
p.init();
//cout << "x = " << p.x << ", y = " << p.y << ", z = " << p.z << endl;
/*init(ref_p,1);
cout << "x = " << p.x << ", y = " << p.y << ", z = " << p.z << endl;
init(ref_p,1,2);
cout << "x = " << p.x << ", y = " << p.y << ", z = " << p.z << endl;
init(ref_p,1,2,3);
cout << "x = " << p.x << ", y = " << p.y << ", z = " << p.z << endl;
*/}

//exercice 16

void essai_alloc(){
    int* pt_int = new int;
    double* pt_double = new double[100];
    delete pt_int;
    delete[] pt_double;
}

//exercice 17

void raz(personne* p){
    p->nom[0] = '\0';
    p->age = 0;
}

void affiche_struct(const personne* p){
    using namespace std;
    cout << p->nom << " a " << p->age << " ans." << endl;
}

void affiche_tab(const personne* tab, const int nb){
    using namespace std;
    for (int i = 0; i < nb; i++)
        affiche_struct(&tab[i]);
}

void init_struct(personne* p, char* nom, int age){
    p->age = age;
    int i = 0;
    while(nom[i]!='\0'){
        p->nom[i] = nom[i];
        i++;
    }
    p->nom[i] = '\0';
}

void copy_struct(personne* p1, personne* p2){
    p2->age = p1->age;
    strcpy(p2->nom, p1->nom);
}

void copy_tab(personne* p1, personne* p2, int nb){
    for(int i = 0; i<nb; i++)
        p2[i] = p1[i];

}

//exercice 18

int& operation(compte* tab, const char* id){
    for(int i = 0; i<4; i++)
        if(strcmp(id, tab[i].id) == 0)
            return tab[i].solde;
}

void essai_comptes(){
    using namespace std;
    compte tab[4]={
        {"courant", 0},{"codevi", 1500 },
        {"epargne", 200 }, { "cel", 300 }
    };
    operation(tab,"courant")=100;
    operation(tab,"codevi")+=100;
    operation(tab,"cel")-=50;
    for(int i=0; i<4; i++)
        cout<<tab[i].id<<" : "<<tab[i].solde<<"\n";
}



