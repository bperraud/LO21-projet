#include <iostream>
#include "fonction.h"

using namespace std;

int main()
{
    int a = 5;
    int b = 10;
    inverse(&a, &b);
    cout << "a = " << a << ", b = " << b << endl;
    int& pt1 = a;
    int& pt2 = b;
    inverse(pt1, pt2);
    cout << "a = " << a << ", b = " << b << endl;
    essai E;
    E.n = 5;
    E.x = 2.5;
    essai& ref_E = E;
    raz(ref_E);
    cout << "E.n = " << E.n << ", E.x = " << E.x << endl;
    essai_init();
    personne p;
    raz(&p);
    affiche_struct(&p);
    essai_comptes();

    cout << endl;

    personne tab[10];
    init_struct(&tab[0], "Marie", 12);
    affiche_struct(&tab[0]); cout << endl;
    copy_struct(&tab[0], &tab[1]);
    affiche_tab(tab, 2); cout << endl;
    raz(&tab[0]);
    affiche_struct(&tab[0]); cout << endl;
    personne tab2[10];
    copy_tab(tab, tab2, 2);
    affiche_tab(tab2, 2);
    return 0;
}
