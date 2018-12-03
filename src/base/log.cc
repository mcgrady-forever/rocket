#include "base/log.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <memory>

#include "concurrency/this_thread.h"

namespace rocket {
namespace base {

Log LOG;

void Log::DefaultFunc(Level level, const char* msg) {
    char str_time[22];
    int time_len = FormatTime(str_time, 22);
    if (time_len < 0) {
        return;
    }
    switch (level) {
        case Level::DEBUG:
            fprintf(stdout, "D%s %d%s\n", str_time,
                concurrency::ThisThread::GetId(), msg);
            break;
        case Level::INFO:
            fprintf(stdout, "I%s %d%s\n", str_time,
                concurrency::ThisThread::GetId(), msg);
            break;
        case Level::WARN:
            fprintf(stdout, "W%s %d%s\n", str_time,
                concurrency::ThisThread::GetId(), msg);
            break;
        case Level::ERROR:
            fprintf(stdout, "E%s %d%s\n", str_time,
                concurrency::ThisThread::GetId(), msg);
            break;
        case Level::FATAL:
            fprintf(stdout, "F%s %d%s\n", str_time,
                concurrency::ThisThread::GetId(), msg);
            abort();
            break;
        default:
            abort();
            break;
    }
}

int Log::FormatTime(char* buffer, size_t size) {
    uint64_t now = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    time_t sec = now / 1000000;
    int usec = now % 1000000;
    struct tm tm;
    localtime_r(&sec, &tm);
    int len = static_cast<int>(strftime(buffer, size, "%m%d %H:%M:%S", &tm));
    if (len == 0) {
        return -1;
    }
    len += snprintf(buffer + len, size - len, ".%06d", usec);
    if (len <= 0) {
        return -1;
    }
    buffer[len] = '\0';
    return len;
}

void Log::VPrintf(Level level, const char* fmt, va_list ap) {
    static const int kStackBufferSize = 256;
    char stack_buf[kStackBufferSize];

    int need = vsnprintf(stack_buf, kStackBufferSize, fmt, ap);
    if (need < kStackBufferSize) {
        _func(level, stack_buf);
    } else {
        std::unique_ptr<char[]> heap_buf(new char[need + 1]);
        int rval = vsnprintf(heap_buf.get(), need + 1, fmt, ap);
        if (rval != -1) {
            _func(level, heap_buf.get());
        }
    }
}

}  // namespace base
}  // namespace rocket