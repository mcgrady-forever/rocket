#ifndef _ROCKET_SERVER_H_
#define _ROCKET_SERVER_H_

#include <stdint.h>
#include <string>
#include <vector>
#include "thread_wrapper.h"
#include "event_wrapper.h"
#include "connection.h"
//#include <boost/asio.hpp>
//#include <boost/noncopyable.hpp>
//#include <boost/shared_ptr.hpp>

//using boost::asio::ip::tcp;

namespace rocket
{

class Server
{
public:
    explicit Server(const uint32_t port, 
                    const int network_threads_num = 1,
                    const int work_threads_num = 1);
    ~Server() {}

    // init
    int init();

    // Run the server's io_service loop
    void run();

    // Stop the server
    void stop();

    void WorkThreadFunc();

    static void ReadCallback(int fd);
    static void WriteCallback(int fd);
    
private:
    int                           network_threads_num_;
    int                           work_threads_num_;
    uint32_t                      port_;
    int                           listenfd_;
    std::vector<ThreadBase*>      network_threads_;
    std::vector<ThreadBase*>      work_threads_;
    volatile bool                 stop_;

    static ConnectionQueue               connection_que_;
    static EventWrapper                  connection_que_ev_;
};

}

#endif // _ROCKET_SERVER_H_