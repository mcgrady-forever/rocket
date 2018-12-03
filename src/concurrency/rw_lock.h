#ifndef ROCKET_CONCURRENCY_RW_LOCK_H_
#define ROCKET_CONCURRENCY_RW_LOCK_H_

#include <pthread.h>
#include <cassert>

#include "concurrency/lock_base.h"


namespace rocket {
namespace concurrency {

class RWLock {
public:
    RWLock();
    ~RWLock();
    
    int RLock() {
        return pthread_rwlock_rdlock(&_rwlock);
    }

    int WLock() {
        return pthread_rwlock_wrlock(&_rwlock);
    }

    int Unlock() {
        return pthread_rwlock_unlock(&_rwlock);
    }

private:
	int init();

private:
    pthread_rwlock_t _rwlock;
};

} // namespace concurrency
} // namespace rocket

#endif  // ROCKET_CONCURRENCY_RW_LOCK_H_