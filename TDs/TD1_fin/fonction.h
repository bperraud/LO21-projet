void exercice_surcharge();

inline int fct(int x){ std::cout<<"1:"<<x<<"\n"; return 0; }
inline int fct(float y){ std::cout<<"2:"<<y<<"\n"; return 0; }
inline int fct(int x, float y){ std::cout<<"3:"<<x<<y<<"\n"; return 0; }
inline float fct(float x, int y){ std::cout<<"4:"<<x<<y<<"\n"; return 3.14; }
