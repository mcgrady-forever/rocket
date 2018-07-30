#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <getopt.h>
#include <iostream>
#include <string>
//#include "glog/logging.h"
#include "logger.h"
#include "config.h"
#include "load_so.h"
#include "server.h"

using namespace std;
using namespace rocket;

static const char *opt_config = NULL;

static bool do_daemonize = false;

static const char *CONFIG_DEFAULT_PID_FILE = "rocket.pid";

void usage_exit(const char *program, int status)
{
    FILE *stream = status != 0 ? stderr : stdout;

    fprintf(stream,
            "Usage: %s [options]\n\n"
            "Options:\n"
            "  -c, --config <configfile>    specify config file\n"
            "  -h, --help                   show help message\n"
            "  -d, --daemon                 run as a daemon\n"
            "  -v, --version                show version\n",
            program);

    exit(status);
}

void version_exit(int status)
{
    FILE *stream = status != 0 ? stderr : stdout;

    fprintf(stream, "Built: " __DATE__ " " __TIME__ "\n");

    exit(status);
}

int parse_options(int argc, char **argv)
{
    int c;
    static struct option long_options[] = {
        {"config", required_argument, NULL, 'c'},
        {"help", no_argument, NULL, 'h'},
        {"daemon", no_argument, NULL, 'd'},
        {"version", no_argument, NULL, 'v'},
        {0, 0, 0, 0}
    };

    while ((c = getopt_long(argc, argv, "c:hdv", long_options, NULL)) != -1)
    {
        switch (c)
        {
            case 'c':   /* config */
                opt_config = optarg;
                break;
            case 'h':   /* help */
                usage_exit(argv[0], 0);
                break;
            case 'd':   /* daemonize */
                do_daemonize = true;
                break;
            case 'v':   /* version */
                version_exit(0);
                break;
            default:
                fprintf(stderr, "no command line options\n");
                return -1;
        }
    }

    if (optind < argc)
    {
        fprintf(stderr, "unexpected argument %s\n", argv[optind]);
        return -1;
    }

    if (opt_config == NULL)
    {
        fprintf(stderr, "no config file is specified\n");
        return -1;
    }

    return 0;
}

int daemonize() {
    int fd;

    switch (fork()) {
        case -1:
            return -1;
        case 0:
            break;
        default:
            _exit(EXIT_SUCCESS);
    }

    /* creates a new session if the calling process is not a process group leader.
       https://linux.die.net/man/2/setsid
    */
    if (setsid() == -1)
        return -1;

    //将stdin、stdout、stderr重定向到/dev/null(或称空设备，它丢弃一切写入其中的数据（但报告写入操作成功），读取它则会立即得到一个EOF。)
    if ((fd = open("/dev/null", O_RDWR, 0)) != -1) {
        if(dup2(fd, STDIN_FILENO) < 0) {
            perror("dup2 stdin");
            return -1;
        }
        if(dup2(fd, STDOUT_FILENO) < 0) {
            perror("dup2 stdout");
            return -1;
        }
        if(dup2(fd, STDERR_FILENO) < 0) {
            perror("dup2 stderr");
            return -1;
        }

        if (fd > STDERR_FILENO) {
            if(close(fd) < 0) {
                perror("close");
                return -1;
            }
        }
    }
    return (0);
}

void createPidFile(void) {
    /* Try to write the pid file in a best-effort way. */
    FILE *fp = fopen(CONFIG_DEFAULT_PID_FILE,"w");
    if (fp) {
        fprintf(fp,"%d\n",(int)getpid());
        fclose(fp);
    }
}

int main(int argc, char* argv[])
{
    /* parse parameter option */
    if (parse_options(argc, argv) != 0)
    {
        exit(EXIT_FAILURE);
    }

    Config cfg;

    if (!cfg.load(opt_config))
    {
        fprintf(stderr, "load config file %s failed\n", opt_config);
        exit(EXIT_FAILURE);
    }

    std::cout << "log path:" << cfg._log_cfg.file << std::endl;
    /*
    //google::InitGoogleLogging(argv[0]);  
    //FLAGS_log_dir = "/home/chris/myspace/rocket/log/";
    //LOG(INFO) << "INFO";  
    //LOG(INFO) << "INFO1";  
    LOG(WARNING) << "WARNING";  
    LOG(WARNING) << "WARNING1";  
    LOG(ERROR) << "ERROR";  
    LOG(ERROR) << "ERROR1";  
    //LOG(FATAL) << "FATAL";  
    google::ShutdownGoogleLogging();  
    */
    
    LOGGER.init(cfg._log_cfg);

    LOG_INFO("rocket server listen on %s:%d", cfg._ip.c_str(), cfg._port);
    LOG_INFO("server thread pool size:%d", cfg._net_nthreads);

    // 加载so
    if (0 == load_so(cfg._app_so.c_str()))
    {
        int ret = rocket_dll.rocket_handle_init((void *)cfg._app_conf.c_str());
        if (ret != 0)
        {
            LOG_ERROR("handle init error, ret: %d", ret);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        LOG_ERROR("load so %s failed", cfg._app_so.c_str());
        exit(EXIT_FAILURE);
    }

    // run as daemon if daemonize is set true
    if (do_daemonize)
    {
        if (daemonize() == -1)
        {
            fprintf(stderr, "failed to daemonize\n");
            exit(EXIT_FAILURE);
        }
    }

    //create pid file
    createPidFile();

    short port = cfg._port;
    std::size_t thread_pool_size = cfg._net_nthreads;

    /*
    boost::asio::io_gservice io_service;
    Server s(io_service, port, thread_pool_size);
    io_service.run();
    */

    Server s(port, thread_pool_size);
    s.run();

    while (true) {
        usleep(1);
    }

    return 0;
}