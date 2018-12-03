#include "concurrency/rw_lock.h"


namespace rocket {
namespace concurrency {

RWLock::RWLock() {
	init();
}

RWLock::~RWLock() {
	pthread_rwlock_destroy(&_rwlock);
}

int RWLock::init() {
	return pthread_rwlock_init(&_rwlock, NULL);
}

} // namespace concurrency
} // namespace rocket