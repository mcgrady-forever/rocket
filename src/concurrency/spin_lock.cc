#include "concurrency/spin_lock.h"


namespace rocket {
namespace concurrency {

SpinLock::SpinLock() {
	init();
}

SpinLock::~SpinLock() {
	pthread_spin_destroy(&_spin_lock);
}

int SpinLock::init() {
	return pthread_spin_init(&_spin_lock, PTHREAD_PROCESS_PRIVATE);
}

} // namespace concurrency
} // namespace rocket