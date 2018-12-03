#ifndef ROCKET_CONCURRENCY_THREAD_H_
#define ROCKET_CONCURRENCY_THREAD_H_

#include <memory>
#include <atomic>
#include <thread>
#include <string>

#include "concurrency/task.h"
#include "concurrency/spin_lock.h"
#include "base/macro.h"


namespace rocket {
namespace concurrency {

// DO NOT inherit it, instead, inherit the Task interface
class Thread {
public:
    using Id = std::thread::id;
    static const size_t MAX_NAME_LEN = 16;

    enum Status : int {
        INIT       = 0x0,
        RUNNING    = 0x1,
        STOP       = 0x2,
    };

    explicit Thread(std::shared_ptr<Task> task,
                    const std::string& name = "");
    explicit Thread(const std::function<bool()>& closure,
                    const std::string& name = "");

    ~Thread();

    Status GetStatus() {
        return _status.load(std::memory_order_acquire);
    }
  
    void Start();

    void Stop();

    const std::string& name() const {
        return _name;
    }

    Id GetId() {
        return _thread->get_id();
    }
  
    void Join();
    
    // check whether this thread is joinable
    bool IsJoinable() {
        return _thread->joinable();
    }

 private:
    class InternalTask : public Task {
    public:
        InternalTask(const std::function<bool()>& closure) 
            : _closure(closure) {
        }
        ~InternalTask() {};

        bool operator()(void* arg = NULL) {
            (void) arg;
            return _closure();
        }

    private:
        std::function<bool()> _closure;
    };

    std::string                     _name;
    std::shared_ptr<Task>           _task;
    std::shared_ptr<std::thread>    _thread;
    std::atomic<Status>             _status {Status::INIT};

    ROCKET_DECLARE_UNCOPYABLE(Thread);
};

}  // namespace concurrency
}  // namespace rocket

#endif  // ROCKET_CONCURRENCY_THREAD_H_