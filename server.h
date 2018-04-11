#ifndef _ROCKET_SERVER_H_
#define _ROCKET_SERVER_H_

#include <stdint.h>
#include <string>
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

    //Run the server's io_service loop
    void run();

    //Stop the server
    void stop();
    
private:
    int         thread_pool_size_;
    uint32_t    port_;
};

}

#endif // _ROCKET_SERVER_H_