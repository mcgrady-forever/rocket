#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

#include "server.hpp"

namespace thunder
{

Server::Server(const unsigned short port, 
               const std::size_t thread_pool_size)
    : thread_pool_size_(thread_pool_size), 
      local_endpoint_(tcp::v4(), port),
      acceptor_(io_service_, local_endpoint_) 
{
    std::cout << "Server construct" << std::endl;
    start_accept();
}

/*
Server::Server(boost::asio::io_service& io_service,
               const unsigned short port, 
               const std::size_t thread_pool_size)
    : io_service_(io_service),
      local_endpoint_(tcp::v4(), port),
      thread_pool_size_(thread_pool_size), 
      acceptor_(io_service_, local_endpoint_)
{
    std::cout << "Server construct" << std::endl;
    start_accept();
}
*/

void Server::start_accept()
{
    std::cout << "Server start_accept" << std::endl;
    Session* new_session = new Session(io_service_);
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&Server::handle_accept, this, new_session,
            boost::asio::placeholders::error));
}

void Server::handle_accept(Session* new_session,
                           const boost::system::error_code& error)
{
    std::cout << "Server handle_accept" << std::endl;
    if (!error)
    {
        std::cout << "Server handle_accept success" << std::endl;
        new_session->set_keepalive();
        new_session->start();
    }
    else
    {
        std::cout << "Server handle_accept error" << error << std::endl;
        delete new_session;
    }

    start_accept();
}

void Server::run()
{
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
}

void Server::stop()
{
    io_service_.stop();
}

}