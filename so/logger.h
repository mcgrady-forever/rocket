#ifndef _LOGGER_H
#define _LOGGER_H

#include <string>

#define LOGGER Logger::getLogger()
#define LOG_DEBUG(...) LOGGER.doLog(LEVEL_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)  LOGGER.doLog(LEVEL_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)  LOGGER.doLog(LEVEL_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) LOGGER.doLog(LEVEL_ERROR, __FILE__, __LINE__, __VA_ARGS__)

enum LOG_LEVEL
{
    LEVEL_DEBUG = 0,
    LEVEL_INFO,
    LEVEL_WARN,
    LEVEL_ERROR,
};

enum LOG_ROTATE
{
    ROTATE_DAILY = 0,
    ROTATE_WEEKLY,
    ROTATE_MONTHLY,
};

struct LogConfig
{
    std::string file;
    int level;
    int rotate;
    unsigned count;
    size_t size;
    bool display;  // display to screen

    LogConfig() : file(), level(LEVEL_DEBUG), rotate(ROTATE_DAILY), count(10), size(104857600), display(true)
    {
    }

    int parseLevel(const std::string& level);
    int parseRotate(const std::string& rotate);
};

class Logger
{
public:
    virtual void init(LogConfig& config) = 0;
    virtual void reconfig(LogConfig& config) = 0;
    virtual void setLevel(int level) = 0;
    virtual void setFile(const char* logfile) = 0;
    virtual void doLog(int level, const char* file, const int line, const char* format, ...) = 0;

    static Logger& getLogger();
};

#endif
