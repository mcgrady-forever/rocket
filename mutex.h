#ifndef _MUTEX_H_
#define _MUTEX_H_

#include <cassert>

namespace rocket {

class Mutex {
public:
    Mutex()
    {
        int ret = pthread_mutex_init(&mutex_, NULL);
        assert(ret == 0);
        initialized_ = true;
    }

    ~Mutex()
    {
        if (initialized_)
        {
            initialized_ = false;
            int ret = pthread_mutex_destroy(&mutex_);
            perror("pthread_mutex_destroy");
            assert(ret == 0);
        }
    }

    void lock()
    {
        int ret = pthread_mutex_lock(&mutex_);
        assert(ret == 0);
    }

    void unlock()
    {
        int ret = pthread_mutex_unlock(&mutex_);
        perror("pthread_mutex_unlock");
        assert(ret == 0);
    }

private:
    pthread_mutex_t     mutex_;
    bool                initialized_;
};

class Lock {
public:
    Lock(Mutex& mutex) 
        : mutex_(&mutex) { 
        if (mutex_ != NULL) {
            mutex_->lock(); 
        }
    }
    ~Lock() { mutex_->unlock(); }

private:
    Mutex*       mutex_;
};

}

#endif