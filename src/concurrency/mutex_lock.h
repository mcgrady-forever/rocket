#ifndef ROCKET_CONCURRENCY_MUTEX_LOCK_H_
#define ROCKET_CONCURRENCY_MUTEX_LOCK_H_

#include <pthread.h>
#include <cassert>

#include "concurrency/lock_base.h"


namespace rocket {
namespace concurrency {

class MutexLock : public LockBase {
public:
	MutexLock();
	~MutexLock();

    int Lock() {
        return pthread_mutex_lock(&_mutex);
    }

    int Unlock() {
        return pthread_mutex_unlock(&_mutex);
    }

private:
	int init();

private:
    pthread_mutex_t _mutex;
};

} // namespace concurrency
} // namespace rocket

#endif  // ROCKET_CONCURRENCY_MUTEX_LOCK_H_