#include "concurrency/mutex_lock.h"

namespace rocket {
namespace concurrency {

MutexLock::MutexLock() {
	init();
}

MutexLock::~MutexLock() {
	pthread_mutex_destroy(&_mutex);
}

int MutexLock::init() {
	return pthread_mutex_init(&_mutex, NULL);
}

} // namespace concurrency
} // namespace rocket