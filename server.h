#ifndef _ROCKET_SERVER_H_
#define _ROCKET_SERVER_H_

#include <stdint.h>
#include <string>
#include <vector>
#include "thread_wrapper.h"
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
                    const int thread_pool_size = 1);
    ~Server() {}

    // init
    int init();

    // Run the server's io_service loop
    void run();

    // Stop the server
    void stop();
    
private:
    int                           thread_pool_size_;
    uint32_t                      port_;
    int                           listenfd_;
    std::vector<ThreadBase*>      threads_;
};

}

#endif // _ROCKET_SERVER_H_