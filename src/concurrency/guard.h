#ifndef ROCKET_CONCURRENCY_GUARD_H_
#define ROCKET_CONCURRENCY_GUARD_H_

#include <cassert>


namespace rocket {
namespace concurrency {

template<class LockType>
class Guard
{
public:
	Guard(LockType* lock)
		: _lock(lock) {
		assert(_lock);
		_lock->Lock();
	}

	~Guard() {
		assert(_lock);
		_lock->Unlock();
	}

private:
	LockType* _lock;
};

} // namespace concurrency
} // namespace rocket

#endif // ROCKET_CONCURRENCY_GUARD_H_