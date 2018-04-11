#include "server.h"

#include <vector>

namespace rocket {

Server::Server(const uint32_t port, 
               const int thread_pool_size/* = 1*/) 
    : port_(port),
      thread_pool_size_(thread_pool_size) {

};

void Server::run() {
    /*
    std::vector<boost::shared_ptr<boost::thread> > threads; 
    for(std::size_t i = 0; i < thread_pool_size_; ++i)
    {
        boost::shared_ptr<boost::thread> thread(new boost::thread(
            boost::bind(&boost::asio::io_service::run, &io_service_)));
        threads.push_back(thread);
    }

    for(std::size_t i = 0; i < threads.size(); ++i)
    {
        threads[i]->join();
    }
    */
}

void Server::stop()
{
    //io_service_.stop();
}

} // namespace rocket