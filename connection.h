#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <string>
#include <deque>
#include "commu.h"
#include "mutex.h"

namespace rocket {

// 连接信息
class Connection : public Commu
{
private:
    Connection(int fd = -1, const std::string& ip = "", int port = 0, unsigned flow = 0);
    virtual ~Connection();

public:
    int get_fd() { return fd_; }

    virtual std::string get_ip() { return ip_; }

    virtual int get_port() { return port_; }

    virtual int Write(unsigned flow, const char* buf, size_t len);

private:
    int               fd_;       // client socket fd
    std::string       ip_;       // client ip
    int               port_;     // client port
    unsigned          flow_;     // client connection identifier
};


class ConnectionQueue
{
public:
    ConnectionQueue();
    ~ConnectionQueue();

    void push_front(Connection* c) {
        Lock Lock(mutex_);
        que_.push_front(c);
    }

    void push_back(Connection* c) {
        Lock Lock(mutex_);
        que_.push_back(c);
    }

    Connection* pop_front() {
        Lock Lock(mutex_);
        Connection* c = NULL;
        if (!que_.empty())
        {
            c = que_.front();
            que_.pop_front();
        }
        return c;
    }

    Connection* pop_back() {
        Lock Lock(mutex_);
        Connection* c = NULL;
        if (!que_.empty())
        {
            c = que_.back();
            que_.pop_back();
        }
        return c;
    }

private:
    std::deque<Connection*>      que_;
    Mutex                        mutex_;
};

} //namespace rocket

#endif