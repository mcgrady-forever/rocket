#ifndef ROCKET_BASE_SINGLETON_H_

#if __GNUC__ == 4 && (__GNUC_MINOR__ == 3 || __GNUC_MINOR__ > 3)

// gcc version greater than 4.3.3
// Dynamic initialization and destruction with concurrency
// https://gcc.gnu.org/projects/cxx-status.html#cxx11

template<class T>
class Singleton
{
public:
    static T instance() {
        static T singleton;
        return singleton;
    }

protected:
	Singleton() {}
	~Singleton() {}

private:
    ROCKET_DECLARE_UNCOPYABLE(Singleton);
};

#else

#include <iostream>
 
template<class T>
class Singleton
{
public:
	static T& instance()
	{
		pthread_once(&ponce_, &Singleton::init);
		return *value_;
	}
 
	static void init()
	{
		value_ = new T();
	}

protected:
	Singleton() {}
	~Singleton() {}

private:
	static pthread_once_t  _ponce;
	static T*              _value;

    ROCKET_DECLARE_UNCOPYABLE(Singleton);
};
 
template<typename T>
pthread_once_t Singleton<T>::_ponce = PTHREAD_ONCE_INIT;
 
template<typename T>
T* Singleton<T>::_value = NULL;

#endif

#endif // ROCKET_BASE_SINGLETON_H_
