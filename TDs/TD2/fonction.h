#ifndef FONCTION_H_INCLUDED
#define FONCTION_H_INCLUDED

struct essai{
    int n;
    float x;
};

class point{
private:
    int x;
    int y;
    int z;
public:
    void init(int _x = 0, int _y = 0, int _z = 0);
};

struct personne{
    char nom[30];
    unsigned int age;
};

struct compte{
    char id[20];
    int solde;
};

void inverse(int* a, int* b);
void inverse(int& a, int& b);
void raz(essai* e);
void raz(essai& e);

void essai_init(void);
void essai_alloc(void);

void raz(personne* p);
void affiche_struct(const personne* p);
void affiche_tab(const personne* tab, const int nb);
void init_struct(personne* p, char* nom, int age);
void copy_struct(personne* p1, personne* p2);
void copy_tab(personne* p1, personne* p2, int nb);

int& operation(compte* tab, const char* id);
void essai_comptes(void);

#endif // FONCTION_H_INCLUDED
