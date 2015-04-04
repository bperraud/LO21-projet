#include <iostream>
#include "fraction.h"

using namespace std;
using namespace MATH;

int main()
{
    Fraction f1(4,5);//Si d vaut 0, l'objet est qd même généré.
    try{
        f1.setFraction(4,0);
    }
    catch(FractionException e){cout << e.getInfo();}
    Fraction f2(10,8);
    Fraction f3 = 5 + f2;
    f3--;
    //Fraction f3 = somme(&f1, &f2);
    //f3.somme(&f1, &f2);
    //f.setFraction(3,4);

    cout << f1 << endl;
    return 0;
}
