#ifndef ROCKET_CONCURRENCY_LOCK_BASE_H_
#define ROCKET_CONCURRENCY_LOCK_BASE_H_

#include <pthread.h>
#include <cassert>


namespace rocket {
namespace concurrency {

class LockBase {
public:
    LockBase() {}
    virtual ~LockBase() {}

    virtual int Lock()  = 0;

    virtual int Unlock()  = 0;
};

} // namespace concurrency
} // namespace rocket

#endif  // ROCKET_CONCURRENCY_LOCK_BASE_H_