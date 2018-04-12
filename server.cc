#include "server.h"

#include <vector>
#include "logger.h"
#include "event_dispatcher.h"

namespace rocket {

Server::Server(const uint32_t port, 
               const int thread_pool_size/* = 1*/) 
    : port_(port),
      thread_pool_size_(thread_pool_size) {
    LOG_INFO("Server::server");
};

void Server::run() {
    LOG_INFO("Server::run()");

    for (int i = 0; i < thread_pool_size_; ++i) {
        EventDispatcher event_dispatcher(port_);
        NetworkThread* thread = new NetworkThread(std::bind(&EventDispatcher::loop, &event_dispatcher),
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