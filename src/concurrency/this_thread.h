#ifndef ROCKET_CONCURRENCY_THIS_THREAD_H_
#define ROCKET_CONCURRENCY_THIS_THREAD_H_

#include <errno.h>
#include <pthread.h>
#include <string.h>

#include <string>
#include <thread>

namespace rocket {
namespace concurrency {

class ThisThread {
    public:
    // this method can get the thread id with int type
    static int GetId();

    static int GetLastError();

    static void SetLastError(int err);

    static std::string GetLastErrorString();

    static std::string GetErrorString(int err);
};

}  // namespace concurrency
}  // namespace rocket

#endif  // ROCKET_CONCURRENCY_THIS_THREAD_H_