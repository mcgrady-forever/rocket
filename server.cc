#include "server.h"

#include <limits.h>
#include <vector>
#include "logger.h"
#include "event_dispatcher.h"
#include "socket_util.h"

namespace rocket {

Server::Server(const uint32_t port, 
               const int thread_pool_size/* = 1*/) 
    : port_(port)
    , listenfd_(0)
    , thread_pool_size_(thread_pool_size) {
    LOG_INFO("Server::server");
    init();
};

int Server::init() {
	listenfd_ = BaseSocket::socket();
	if (-1 == listenfd_) {
		perror("socket()");
		return -1;
	}

    int ret = -1;
	ret = BaseSocket::setReuseAddr(listenfd_);
    ret = BaseSocket::setNonBlocking(listenfd_);

    ret = BaseSocket::bind(listenfd_, port_);
    ret = BaseSocket::listen(listenfd_, INT_MAX);

	return ret;
}

void Server::run() {
    LOG_INFO("Server::run()");

    for (int i = 0; i < thread_pool_size_; ++i) {
        EventDispatcher* event_dispatcher = new EventDispatcher(listenfd_);
        LOG_INFO("Server::run() 1");
        NetworkThread* thread = new NetworkThread(std::bind(&EventDispatcher::loop, event_dispatcher),
                                                  event_dispatcher);
        threads_.push_back(thread);
        thread->start();
        LOG_INFO("thread start i=%d, tid=%d", i, thread->thread_id());
    }
}

void Server::stop()
{
    for (auto it = threads_.begin(); it != threads_.end(); ++it) {
        delete *it;
        *it = NULL;
    }
}

} // namespace rocket