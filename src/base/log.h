#ifndef ROCKET_BASE_LOG_H_
#define ROCKET_BASE_LOG_H_

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>

namespace rocket {
namespace base {

class Log {
public:
    enum Level : uint8_t {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
    };

    Log() : _func(&DefaultFunc) {
    }

    inline void set_func(void (*func)(Level level, const char*)) {
        _func = func;
    }

    inline void Debug(const char* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        VPrintf(Level::DEBUG, fmt, ap);
        va_end(ap);
    }
    inline void Info(const char* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        VPrintf(Level::INFO, fmt, ap);
        va_end(ap);
    }
    inline void Warn(const char* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        VPrintf(Level::WARN, fmt, ap);
        va_end(ap);
    }
    inline void Error(const char* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        VPrintf(Level::ERROR, fmt, ap);
        va_end(ap);
    }
    inline void Fatal(const char* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        VPrintf(Level::FATAL, fmt, ap);
        va_end(ap);
    }

    static void DefaultFunc(Level level, const char* msg);

private:
    void VPrintf(Level level, const char* fmt, va_list ap);

    static int FormatTime(char* buffer, size_t size);

    void (*_func)(Level level, const char*);
};

extern Log LOG;

#define Debug(fmt, ...) \
  rocket::base::LOG.Debug(" %s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__);

#define Info(fmt, ...) \
  rocket::base::LOG.Info(" %s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__);

#define Warn(fmt, ...) \
  rocket::base::LOG.Warn(" %s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__);

#define Error(fmt, ...) \
  rocket::base::LOG.Error(" %s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__);

#define Fatal(fmt, ...) \
  rocket::base::LOG.Fatal(" %s:%d] " fmt, __FILE__, __LINE__, ##__VA_ARGS__);

}  // namespace base
}  // namespace rocket

#endif  // ROCKET_BASE_LOG_H_
