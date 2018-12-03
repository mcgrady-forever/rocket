#ifndef ROCKET_CONCURRENCY_SPIN_LOCK_H_
#define ROCKET_CONCURRENCY_SPIN_LOCK_H_

#include <pthread.h>

#include "concurrency/lock_base.h"


namespace rocket {
namespace concurrency {

class SpinLock : public LockBase {
public:
	SpinLock();
	~SpinLock();

    int Lock() {
        return pthread_spin_lock(&_spin_lock);
    }

    int Unlock() {
        return pthread_spin_unlock(&_spin_lock);
    }

private:
	int init();

private:
    pthread_spinlock_t _spin_lock;
 };

} // namespace concurrency
} // namespace rocket

#endif  // ROCKET_CONCURRENCY_SPIN_LOCK_H_