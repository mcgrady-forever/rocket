#ifndef _CONFIG_H
#define _CONFIG_H

#include <string>

#include "logger.h"

namespace rocket
{

class Config
{
public:
    bool load(const char* file);

public:
    std::string _ip;
    int _port;
    int _net_nthreads;
    int _work_nthreads;
    int _timeout;
    bool _enable_thread_name;

    std::string _app_so;
    std::string _app_conf;

    LogConfig _log_cfg;
};

}

#endif