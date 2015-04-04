#include "fraction.h"

using namespace MATH;

void Fraction::setFraction(int n, int d){
    numerateur = n;
    if (d != 0)
        denominateur = d;
    else
        throw FractionException("erreur\n");
    simplification();
}

// à ajouter en tant méthode privée de la classe Fraction
void Fraction::simplification(){
// si le numerateur est 0, le denominateur prend la valeur 1
if (numerateur==0) { denominateur=1; return; }
/* un denominateur ne devrait pas être 0;
si c’est le cas, on sort de la méthode */
if (denominateur==0) return;
/* utilisation de l’algorithme d’Euclide pour trouver le Plus Grand Commun
Denominateur (PGCD) entre le numerateur et le denominateur */
int a=numerateur, b=denominateur;
// on ne travaille qu’avec des valeurs positives...
if (a<0) a=-a; if (b<0) b=-b;
while(a!=b){ if (a>b) a=a-b; else b=b-a; }
// on divise le numerateur et le denominateur par le PGCD=a
numerateur/=a; denominateur/=a;
// si le denominateur est négatif, on fait passer le signe - au numerateur
if (denominateur<0) { denominateur=-denominateur; numerateur=-numerateur; }
}

void Fraction::somme(const Fraction* x, const Fraction* y){
    this->Fraction::setFraction(
    x->numerateur * y->denominateur
    + y->numerateur * x->denominateur,
    x->denominateur * y->denominateur
    );
}

Fraction MATH::somme(const Fraction* x, const Fraction* y){
    Fraction f(
    x->getNumerateur() * y->getDenominateur()
    + y->getNumerateur() * x->getDenominateur(),
    x->getDenominateur() * y->getDenominateur()
    );
    return f;
}

Fraction MATH::operator+(const Fraction& x, const Fraction& y){
    Fraction f(
    x.getNumerateur() * y.getDenominateur()
    + y.getNumerateur() * x.getDenominateur(),
    x.getDenominateur() * y.getDenominateur()
    );
    return f;
}

MATH::Fraction& MATH::Fraction::operator++(){ // préfixe
    setFraction(numerateur+denominateur, denominateur);
    return *this;
}
MATH::Fraction MATH::Fraction::operator++(int){ // postfixe
    Fraction tmp = *this; // construction de tmp par recopie de *this
    setFraction(numerateur+denominateur, denominateur);
    return tmp;
}

Fraction& MATH::operator--(Fraction& f){ // préfixe
    f.setFraction(f.getNumerateur()-f.getDenominateur(), f.getDenominateur());
    return f;
}
Fraction MATH::operator--(Fraction& f, int){ // postfixe
    Fraction tmp = f;
    f.setFraction(f.getNumerateur()-f.getDenominateur(), f.getDenominateur());
    return tmp;
}

std::ostream& operator<<(std::ostream& s, const Fraction& f){
  s << f.getNumerateur();
  if (f.getDenominateur() != 1)
    s << "/" << f.getDenominateur();
  return s;
}
