#if defined(linux) || defined(__linux) || defined(__linux__)
#ifndef ROCKET_TCP_EPOLL_EVENT_POLLER_IMPL_H_
#define ROCKET_TCP_EPOLL_EVENT_POLLER_IMPL_H_

#include <sys/epoll.h>
#include <unistd.h>

#include <vector>

#include "tcp/event_poller.h"
#include "tcp/event.h"

namespace rocket {
namespace tcp {

class EpollEventPollerImpl : public EventPoller {
public:
    explicit EpollEventPollerImpl(int id, size_t max_connections)
        : EventPoller(id, max_connections)
        , _epoll_fd(-1)
        , _epoll_events(max_connections) {
    }

    ~EpollEventPollerImpl() {};

protected:
    bool DoInit();

    void DoShutdown() {
        if (_epoll_fd >= 0) {
            ::close(_epoll_fd);
        }
    }

    bool Poll();

private:
    int                        _epoll_fd;
    std::vector<epoll_event>   _epoll_events;

    bool AddPollerEvent(Event& ev);
    bool ModifyPollerEvent(Event& ev);
    bool RemovePollerEvent(Event& ev);
};

}  // namespace tcp
}  // namespace rocket

#endif  // ROCKET_TCP_EPOLL_EVENT_POLLER_IMPL_H_
#endif  // define(linux) || defined(__linux) || defined(__linux__)