#include "logger.h"

#include <ctime>
#include <string>
#include <cassert>
#include <queue>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdarg.h>
#include <string.h>
#include <pthread.h>

#define LOG_BUF_SIZE 2048

class Mutex
{
public:
    Mutex()
    {
        int ret = pthread_mutex_init(&_mutex, NULL);
        assert(ret == 0);
        _initialized = true;
    }

    ~Mutex()
    {
        if (_initialized)
        {
            _initialized = false;
            int ret = pthread_mutex_destroy(&_mutex);
            perror("pthread_mutex_destroy");
            assert(ret == 0);
        }
    }

    void lock()
    {
        int ret = pthread_mutex_lock(&_mutex);
        assert(ret == 0);
    }

    void unlock()
    {
        int ret = pthread_mutex_unlock(&_mutex);
        assert(ret == 0);
    }

private:
    pthread_mutex_t _mutex;
    bool _initialized;
};

class Guard
{
public:
    Guard(Mutex& mutex) : _mutex(&mutex)
    {
        if (_mutex)
        {
            _mutex->lock();
        }
    }

    ~Guard()
    {
        if (_mutex)
        {
            _mutex->unlock();
        }
    }

private:
    Mutex* _mutex;
};

int LogConfig::parseLevel(const std::string& level)
{
    const char* level_str[] = { "debug", "info", "warn", "error" };

    for (int i = LEVEL_DEBUG; i <= LEVEL_ERROR; ++i)
    {
        if (0 == strcasecmp(level.c_str(), level_str[i]))
        {
            return i;
        }
    }

    return LEVEL_DEBUG;
}

int LogConfig::parseRotate(const std::string& rotate)
{
    const char* rotate_str[] = { "daily", "weekly", "monthly" };

    for (int i = ROTATE_DAILY; i <= ROTATE_MONTHLY; ++i)
    {
        if (0 == strcasecmp(rotate.c_str(), rotate_str[i]))
        {
            return i;
        }
    }

    return ROTATE_DAILY;
}

class LoggerImpl : public Logger
{
public:
    LoggerImpl() : _size(0)
    {
        _file = NULL;
        _initialized = false;
    }

    virtual void init(LogConfig& config)
    {
        Guard guard(_mutex);

        if (!_initialized)
        {
            _initialized = true;
            std::time_t tt = time(NULL);
            _tm_rotate = *localtime(&tt);
            _config = config;
            openLogFile();
        }
    }

    virtual void reconfig(LogConfig& config)
    {
        Guard guard(_mutex);

        assert(_initialized);

        if (_config.file != config.file)
        {
            closeLogFile();
            _config = config;
            openLogFile();
            return;
        }

        _config = config;
    }

    virtual void setLevel(int level)
    {
        Guard guard(_mutex);
        assert(_initialized);
        if (level < LEVEL_DEBUG || level > LEVEL_ERROR)
        {
            return;
        }
        _config.level = level;
    }

    virtual void setFile(const char* file)
    {
        Guard guard(_mutex);
        assert(_initialized);
        if (_config.file != file)
        {
            closeLogFile();
            _config.file = file;
            openLogFile();
        }
    }

    virtual void doLog(int level, const char* file, const int line, const char* format, ...)
    {
        Guard guard(_mutex);

        if (level < _config.level) return;

        if (level > LEVEL_ERROR) level = LEVEL_ERROR;

        // if (!_initialized) return;

        std::time_t tt = time(NULL);
        _tm_now = *localtime(&tt);

        rotateLogFile();

        const char* level_str[] = { "DEBUG", "INFO ", "WARN ", "ERROR" };
        int len;

        len = sprintf(_log_buf, "[%04d-%02d-%02d %02d:%02d:%02d] [%s] [%s:%i] ",
                      _tm_now.tm_year + 1900,
                      _tm_now.tm_mon + 1,
                      _tm_now.tm_mday,
                      _tm_now.tm_hour,
                      _tm_now.tm_min,
                      _tm_now.tm_sec,
                      level_str[level],
                      filename(file),
                      line);

        va_list ap;

        va_start(ap, format);

        int ret = vsnprintf(_log_buf + len, LOG_BUF_SIZE - len, format, ap);

        if (ret > 0 && ret < LOG_BUF_SIZE - len - 1)
        {
            len += ret;
        }
        else
        {
            len += LOG_BUF_SIZE - len - 1;
        }

        va_end(ap);

        if (_config.display)
        {
            fprintf(stderr, "%s\n", _log_buf);
        }

        if (_file)
        {
            fprintf(_file, "%s\n", _log_buf);
            fflush(_file);
            _size += len + 1;
        }
    }

protected:
    void openLogFile()
    {
        if (_config.file.empty())
        {
            return;
        }

        _file = fopen(_config.file.c_str(), "ab");
        if (_file == NULL)
        {
            fprintf(stderr, "failed to open %s: %s\n",
                    _config.file.c_str(), strerror(errno));
        }
    }

    void closeLogFile()
    {
        if (_file)
        {
            fclose(_file);
            _file = NULL;
        }
    }

    void rotateLogFile()
    {
        if (_config.file.empty())
        {
            closeLogFile();
            return;
        }

        if (!_file) {
            return;
        }

        bool rotate = false;
        if (_size >= _config.size)
        {
            rotate = true;
        }
        else if (_config.rotate == ROTATE_DAILY &&
                 _tm_now.tm_mday != _tm_rotate.tm_mday)
        {
            rotate = true;
        }
        else if (_config.rotate == ROTATE_WEEKLY &&
                 _tm_now.tm_mday != _tm_rotate.tm_mday &&
                 _tm_now.tm_wday == _tm_rotate.tm_wday)
        {
            rotate = true;
        }
        else if (_config.rotate == ROTATE_MONTHLY &&
                 _tm_now.tm_mon != _tm_rotate.tm_mon)
        {
        }

        if (rotate)
        {
            if (_config.count > 0 && _file_queue.size() >= _config.count)
            {
                remove(_file_queue.front().c_str());
                _file_queue.pop();
            }

            closeLogFile();
            std::string baklog = getLogFileName(_tm_rotate);
            rename(_config.file.c_str(), baklog.c_str());
            openLogFile();

            _file_queue.push(baklog);
            _tm_rotate = _tm_now;
            _size = 0;
        }
    }

    std::string getLogFileName(struct tm xtm)
    {
        if (_config.file.empty()) return "";

        char timestamp[100];
        sprintf(timestamp, "%04d%02d%02d%02d%02d",
                xtm.tm_year + 1900,
                xtm.tm_mon + 1,
                xtm.tm_mday,
                xtm.tm_hour,
                xtm.tm_min);

        return _config.file + "." + timestamp;
    }

    /**
     * get filename from path
     */
    const char* filename(const char* path)
    {
        const char* name = strrchr(path, '/');
        if (name)
        {
            return name + 1;
        }
        return path;
    }

private:
    bool _initialized;
    FILE* _file;
    LogConfig _config;
    struct tm _tm_rotate;
    struct tm _tm_now;
    size_t _size;
    char _log_buf[LOG_BUF_SIZE];
    Mutex _mutex;
    std::queue<std::string> _file_queue;
};

Logger& Logger::getLogger()
{
    static LoggerImpl logger;
    return logger;
}
