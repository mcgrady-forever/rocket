#ifndef ROCKET_CONCURRENCY_TASK_H_
#define ROCKET_CONCURRENCY_TASK_H_

#include <memory>
#include <atomic>

namespace rocket {
namespace concurrency {

// we use this functor class as entity of thread
class Task {
public:
    Task() : _stop(false) {
    };
    virtual ~Task() {}

    // the executer
    virtual bool operator()(void* arg = NULL) = 0;

    virtual void Stop() {
        _stop.store(true, std::memory_order_release);
    }

    bool IsStopped() {
        return _stop.load(std::memory_order_acquire);
    }

protected:
    std::atomic<bool> _stop;
};

}  // namespace concurrency
}  // namespace rocket

#endif  // ROCKET_CONCURRENCY_TASK_H_