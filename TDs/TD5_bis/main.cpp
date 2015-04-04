#include "Calendar.h"

int main(){
	TacheManager& manager = TacheManager::getTacheManager();
	manager.load("taches.dat");
    for(TacheManager::Iterator it
    = manager.getIterator(); !it.isDone(); it.next()){
        std::cout<<it.current()<<"\n";
    }
    for(TacheManager::iterator it=manager.begin();it!=manager.end();
    ++it) std::cout<<*it<<"\n";
    /*end() renvoie un it�rateur qui d�signe la t�che (fictive)
    situ�e apr�s la derni�re t�che*/
	//manager.ajouterTache("AW4","Partage des scenarii",Duree(1,30),Date(1,1,2015),Date(1,9,2015));
	//manager.ajouterTache("AW2","Identification des competences",Duree(2,30),Date(1,1,2015),Date(1,12,2015));
	//manager.save("taches.dat");
	system("pause");
	return 0;
}
