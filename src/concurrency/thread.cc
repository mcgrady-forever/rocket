#include "concurrency/thread.h"

#include <pthread.h>
#include <cassert>

#include <memory>

#include "base/log.h"


namespace rocket {
namespace concurrency {

Thread::Thread(std::shared_ptr<Task> task, const std::string& name)
    : _name(name) 
    , _task(task) {
    if (_name.length() > MAX_NAME_LEN) {
        Fatal("Thread name is too long: %s", _name.c_str());
    }
}

Thread::Thread(const std::function<bool()>& closure, const std::string& name)
    : Thread(std::static_pointer_cast<Task>(std::make_shared<InternalTask>(closure)), name) {
}

Thread::~Thread() {
    Stop();
}

void Thread::Start() {
    Debug("thread start...");
    if (!_task.get()) {
        Fatal("task_ is null, please pass a valid task!");
    }

    Status old = Status::INIT;
    if (!_status.compare_exchange_strong(old, Status::RUNNING)) {
        Fatal("Failed to start thread, invalid thread status: %d",
              static_cast<int>(old));
    }

    if (_thread.get()) {
        Fatal("thread_ is not null, there must be something wrong.");
    }

    
    _thread = std::make_shared<std::thread>([this] () {
#if defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
        pthread_setname_np(_name.c_str());
#else
        pthread_setname_np(pthread_self(), _name.c_str());
#endif
        Debug("thread start 1...");
        (*_task)();
    });
}

void Thread::Stop() {
    Status old = Status::RUNNING;
    if (!_status.compare_exchange_strong(old, Status::STOP)) {
        return;
    }

    _task->Stop();

    if (_thread->joinable()) {
        _thread->join();
    }
    _thread.reset();
}

void Thread::Join() {
    if (!_thread.get()) {
        return;
    }

    if (!IsJoinable()) {
        //Fatal("The thread is not joinable!");
    }
    _thread->join();
    _thread.reset();
    _status.store(Status::STOP, std::memory_order_release);
}

}  // namespace concurrency
}  // namespace rocket