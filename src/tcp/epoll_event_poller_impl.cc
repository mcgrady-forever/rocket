#if defined(linux) || defined(__linux) || defined(__linux__)
#include "tcp/epoll_event_poller_impl.h"

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include "tcp/event.h"
#include "tcp/event_dispatcher.h"
#include "concurrency/this_thread.h"
#include "base/log.h"

namespace rocket {
namespace tcp {

bool EpollEventPollerImpl::DoInit() {
    _epoll_fd = ::epoll_create1(EPOLL_CLOEXEC);
    if (_epoll_fd < 0) {
        Error("epoll_create1() failed. erro message: %s",
            concurrency::ThisThread::GetErrorString(
            concurrency::ThisThread::GetLastError()).c_str());
        return false;
    }
    return true;
}

bool EpollEventPollerImpl::Poll() {
    // before starting polling, we first process all the pending command events
    //Debug("EpollEventPollerImpl::Poll 1");
    if (!ProcessInterrupt()) {
        return false;
    }

    int count { 0 };
    do {
//        Debug("EpollEventPollerImpl::Poll 0, count=%d, err_msg:%s",
//              count, rocket::concurrency::ThisThread::GetLastErrorString().c_str());
        count = ::epoll_wait(_epoll_fd,
                             &_epoll_events[0],
                             _epoll_events.size(),
                             -1);
//        Debug("EpollEventPollerImpl::Poll 1, count=%d, err_msg:%s",
//              count, rocket::concurrency::ThisThread::GetLastErrorString().c_str());
    } while (count == -1 
            && rocket::concurrency::ThisThread::GetLastError() == EINTR);

//    Debug("EpollEventPollerImpl::Poll 2, count=%d, err_msg:%s",
//          count, rocket::concurrency::ThisThread::GetLastErrorString().c_str());
    if (count < 0) {
        Debug("EpollEventPollerImpl::Poll 3, count=%d", count);
        return false;
    }

    for (auto i = 0; i < count; ++i) {
        auto fd = _epoll_events[i].data.fd;
        if (fd == _interrupter->get_read_fd()) {
            // we have some command events to be processed
            if (!ProcessInterrupt()) {
                return false;
            }
        } else {
            Event event(fd);
            if (_epoll_events[i].events & (EPOLLHUP | EPOLLERR)) {
                event.mutable_mask() |= static_cast<int>(Event::Type::CLOSE);
            } else {
                if (_epoll_events[i].events & (EPOLLIN | EPOLLRDBAND | EPOLLRDNORM)) {
                    event.mutable_mask() |= static_cast<int>(Event::Type::READ);
                }
                if (_epoll_events[i].events & (EPOLLOUT | EPOLLWRNORM | EPOLLWRBAND)) {
                    event.mutable_mask() |= static_cast<int>(Event::Type::WRITE);
                }
            }

            std::shared_ptr<EventDispatcher> event_disp = _event_disp.lock();
            if (!event_disp || !event_disp->ProcessEvent(event, _id)) {
                return false;
            }
        }
    }
    return true;
}

bool EpollEventPollerImpl::AddPollerEvent(Event& ev) {
    struct epoll_event epoll_ev {0u, 0};
    epoll_ev.data.fd = ev.fd();
    epoll_ev.events = EPOLLIN;
    if (ev.mask() & static_cast<int>(Event::Type::WRITE)) {
        epoll_ev.events |= EPOLLOUT;
    }
    Debug("AddPollerEvent _epoll_fd:%d, fd:%d", _epoll_fd, ev.fd());
    return ::epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, ev.fd(), &epoll_ev) == 0;
}

bool EpollEventPollerImpl::ModifyPollerEvent(Event& ev) {
    struct epoll_event epoll_ev {0u, 0};
    epoll_ev.data.fd = ev.fd();
    epoll_ev.events = EPOLLIN;
    if (ev.mask() & static_cast<int>(Event::Type::WRITE)) {
        epoll_ev.events |= EPOLLOUT;
    }
    return ::epoll_ctl(_epoll_fd, EPOLL_CTL_MOD, ev.fd(), &epoll_ev) == 0;
}

bool EpollEventPollerImpl::RemovePollerEvent(Event& ev) {
    if (ev.mask() & static_cast<int>(Event::Type::CLOSE)) {
        return ::epoll_ctl(_epoll_fd, EPOLL_CTL_DEL, ev.fd(), NULL) == 0;
    }
    return false;
}

}  // namespace tcp
}  // namespace rocket

#endif  // defined(linux) || defined(__linux) || defined(__linux__)