#ifndef SESSION_HPP_
#define SESSION_HPP_ 

#include <cstdlib>
#include <iostream>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include "commu.h"
#include "load_so.h"

using boost::asio::ip::tcp;

namespace thunder
{
class Session : public Commu
{
public:
    Session(boost::asio::io_service& io_service)
      : socket_(io_service)
    {
        std::cout << "Session construct" << std::endl;
    }

    virtual std::string get_ip() 
    { 
        return _ip; 
    }

    virtual int get_port() 
    { 
        return _port;
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        std::cout << "Session start" << std::endl;
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
            boost::bind(&Session::handle_read, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    virtual int write(char* buf, size_t len)
    {
        std::cout << "write start" << std::endl;
        socket_.async_write_some(boost::asio::buffer(buf, len),
            boost::bind(&Session::handle_write, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    void set_keepalive()
    {
        std::cout<<"t1";
        //socket_.open();
        boost::asio::socket_base::keep_alive option;
        socket_.get_option(option);
        std::cout<<"t2";
        bool is_set = option.value();
        std::cout << "is_set:" << is_set << std::endl;

        option = true;
        socket_.set_option(option);
    }

private:
    void handle_read(const boost::system::error_code& error,
                     size_t bytes_transferred)
    {
        std::cout << "handle_read begin:" << std::endl;
        if (!error)
        {
            std::cout << "handle_read success" << std::endl;
            std::cout << "bytes_transferred:" << bytes_transferred << std::endl;
            std::cout << "data_:" << data_ << std::endl;

            
            blob_type blob;
            blob.len = bytes_transferred;
            blob.owner = this;
            blob.data = new char[bytes_transferred];
            memcpy(blob.data, data_, bytes_transferred);

            // 检查数据包完整性
            int pktlen = thunder_dll.thunder_handle_input(&blob);
            
            if (pktlen > 0)
            {
                // 处理请求
                thunder_dll.thunder_handle_process(&blob);
            }
            else
            {
                std::cout << "thunder_handle_input error" << std::endl;
            }


            delete[] blob.data ;
            /*
            boost::asio::async_write(socket_,
            boost::asio::buffer(data_, bytes_transferred),
            boost::bind(&Session::handle_write, this,
            boost::asio::placeholders::error));
            */
        }
        else
        {
            std::cout << "handle_read failed" << std::endl;
            delete this;
        }

        //close();
    }

    void handle_write(const boost::system::error_code& error,
                      size_t bytes_transferred)
    {
        std::cout << "handle_write begin:" << std::endl;
        if (!error)
        {

        }
        else
        {
            delete this;
        }
    }

    void close()
    {
        socket_.close();
    }

private:
    enum { max_length = 1024 };
    char data_[max_length];
    tcp::socket  socket_;
    std::string       _ip;       // client ip
    int               _port;     // client port
};
}

#endif