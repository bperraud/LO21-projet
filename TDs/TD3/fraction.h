#include <iostream>
#include <cstring>

#ifndef FRACTION_H_INCLUDED
#define FRACTION_H_INCLUDED

namespace MATH{

    class FractionException{
        char info[256];
    public:
        FractionException(const char* e){std::strcpy(info, e);}
        const char* getInfo() const {return info;}
    };

    class Fraction{
    private:
        int numerateur;
        int denominateur;

        void simplification();
    public:
        Fraction(int n = 0, int d = 1): numerateur(n), denominateur((d != 0) ? d : 1){
            simplification();
            std::cout << "Objet cree ! ad : " << this << std::endl;
            };
        int getNumerateur() const {return numerateur;};
        int getDenominateur() const {return denominateur;};
        void setFraction(int n, int d);
        //On ne renvoie rien avec somme, on considère cette méthode comme un accesseur en écriture particulier.
        void somme(const Fraction* x, const Fraction* y);
        ~Fraction(){std::cout << "Objet detruit ! ad : " << this << std::endl;};

        Fraction& operator++(); // préfixe
        Fraction operator++(int); // postfixe
    };

Fraction somme(const Fraction* x, const Fraction* y);
Fraction operator+(const Fraction& x, const Fraction& y);

Fraction& operator--(Fraction& f); // préfixe
Fraction operator--(Fraction& f, int); // postfixe

}

std::ostream& operator<<(std::ostream& s, const MATH::Fraction& f);

#endif // FRACTION_H_INCLUDED
