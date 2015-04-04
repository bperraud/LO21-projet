#include<iostream>
using namespace std;

void exerciceA() {
cout<<"donnez le rayon entier d'un cercle:";
int r;
cin>>r;
double p, s;
const double PI = 3.14159;
p=2*PI*r;
s=PI*r*r;
cout<<"le cercle de rayon "<<r<<" a un perimetre de "<<p<<" et une surface de "<<s<<endl;
}
