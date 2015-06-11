#ifndef TEMPLATE_H
#define TEMPLATE_H

template <typename T> class Singleton{
private:
    struct Handler{
        T* instance;
        Handler():instance(0){}
        ~Handler(){ if (instance) delete instance; }
    };
    static Handler handler;
protected:
    Singleton(){}
    ~Singleton(){}
public:
    static T* getInstance(){
        if (!handler.instance) handler.instance = new T;
        return (static_cast<T*>(handler.instance));
    }
    static void libererInstance(){
        if (handler.instance){
            delete handler.instance;
            handler.instance = 0;
        }
    }
};

template <typename T>
typename Singleton<T>::Handler Singleton<T>::handler = Singleton<T>::Handler();

template <class Manager, typename T> class Iterator{
    typename QList<T*>::iterator current;
    Iterator(typename QList<T*>::iterator u):current(u){}
    friend Manager;
public:
    Iterator(){}
    T& operator*() const { return **current; }
    bool operator!=(Iterator it) const { return current != it.current; }
    Iterator& operator++(){ ++current ; return *this; }
};

template <class Manager, typename T> class const_Iterator{
    typename QList<T*>::const_iterator current;
    const_Iterator(typename QList<T*>::const_iterator u):current(u){}
    friend Manager;
public:
    const_Iterator(){}
    const T& operator*() const { return **current; }
    bool operator!=(const_Iterator it) const { return current != it.current; }
    const_Iterator& operator++(){ ++current ; return *this; }
};





#endif // TEMPLATE_H
