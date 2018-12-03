#include "concurrency/this_thread.h"

#include <errno.h>
#include <string.h>
#include <pthread.h>
#if defined(linux) || defined(__linux) || defined(__linux__)
#include <syscall.h>
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#include <sys/syscall.h>
#endif
#include <unistd.h>

namespace rocket {
namespace concurrency {

int ThisThread::GetId() {
    static thread_local pid_t tid = 0;
    if (tid == 0) {
#if defined(linux) || defined(__linux) || defined(__linux__) || \
    defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
    #ifndef __NR_gettid
        #if defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
            #define __NR_gettid SYS_gettid
        #else
            #define __NR_gettid 224
        #endif
    #endif
        tid = syscall(__NR_gettid);
#endif
        if (tid <= 0) {
    #if defined(linux) || defined(__linux) || defined(__linux__)
            tid = getpid();
    #elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
            tid = GetCurrentThreadId();
    #else
            tid = (pid_t)(uintptr_t)pthread_self();
    #endif
        }
    }
    return tid;
}

int ThisThread::GetLastError() {
    return errno;  // thraed local variable
}

void ThisThread::SetLastError(int err) {
    errno = err;
}

std::string ThisThread::GetLastErrorString() {
    return strerror(errno);
}

std::string ThisThread::GetErrorString(int err) {
    return strerror(err);
}

}  // namespace concurrency
}  // namespace rocket