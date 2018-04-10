#include <string.h>
#include <errno.h>

#include "config.h"
#include "config_reader.h"

namespace thunder
{

bool Config::load(const char* file)
{
    ConfigReader cr;

    if (!cr.Load(file))
    {
        return false;
    }

    // log config
    std::string level = cr.GetString("log", "level", "debug");
    std::string rotate = cr.GetString("log", "rotate", "daily");
        _log_cfg.level = _log_cfg.parseLevel(level);
    _log_cfg.rotate = _log_cfg.parseRotate(rotate);
    _log_cfg.file = cr.GetString("log", "file", "./log/ff.log");
    _log_cfg.size = cr.GetNumber<size_t>("log", "size", 104857600);
    _log_cfg.count = cr.GetNumber<unsigned>("log", "count", 50);

    // server config
    _ip = cr.GetString("server", "ip", "0.0.0.0");

    _port = cr.GetNumber<int>("server", "port", 5555);
    if (_port <= 0)
    {
        _port = 5555;
    }

    _net_nthreads = cr.GetNumber<int>("server", "net_nthreads", 10);
    if (_net_nthreads <= 0)
    {
        _net_nthreads = 3;
    }

    _work_nthreads = cr.GetNumber<int>("server", "work_nthreads", 10);
    if (_work_nthreads <= 0)
    {
        _work_nthreads = 3;
    }

    _timeout = cr.GetNumber<int>("server", "timeout", 10);
    if (_timeout < 0)
    {
        _timeout = 0; // 永不超时
    }

    _enable_thread_name = cr.GetBool("server", "enable_thread_name", false);

    // app config
    _app_so = cr["app.so"];
    _app_conf = cr["app.conf"];

    return true;
}

}