#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T> class Singleton{
private:
    static T* instance;
protected:
    Singleton(){}
    ~Singleton(){}
public:
    static T* getInstance(){
        if (!instance) instance = new T;
        return (static_cast<T*>(instance));
    }
    static void libererInstance(){
        if (instance){
            delete instance;
            instance = 0;
        }
    }
};

template <typename T>
T *Singleton<T>::instance = 0;

#endif // SINGLETON_H

