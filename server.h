#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "session.hpp"

using boost::asio::ip::tcp;

namespace rocket
{

class Server
{
public:
    explicit Server(const unsigned short port, 
                    const std::size_t thread_pool_size = 1);

    //Run the server's io_service loop
    void run();

    //Stop the server
    void stop();
    
private:
    int thread_pool_size_;
};

}

#endif