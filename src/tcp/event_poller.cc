// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#include "tcp/event_poller.h"
#include "tcp/event_dispatcher.h"
#include "tcp/event.h"
#include "base/log.h"


#if defined(linux) || defined(__linux) || defined(__linux__)
#include "tcp/epoll_event_poller_impl.h"
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
//#include "tcp/poll_event_poller_impl.h"
#else
//#include <select_event_poller_impl.h>
#endif


namespace rocket {
namespace tcp {

std::shared_ptr<EventPoller> EventPoller::New(size_t id,
                                              size_t max_connections) {
#if defined(linux) || defined(__linux) || defined(__linux__)
    return std::shared_ptr<EventPoller>(
        new EpollEventPollerImpl(id, max_connections));
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
    return std::shared_ptr<EventPoller>(
        //new PollEventPollerImpl(id, max_connections));
#else
    return std::shared_ptr<EventPoller>(
        //new SelectEventPollerImpl(id, max_connections));
#endif
}

bool EventPoller::Init(std::shared_ptr<EventDispatcher> event_disp) {
    if (!event_disp) {
        return false;
    }
    _event_disp = event_disp;

    _interrupter = Interrupter::New();
    if (!_interrupter->Create()) {
        return false;
    }

    if (!DoInit()) {
        _interrupter.reset();
        return false;
    }

    Event ev { _interrupter->get_read_fd(),
                static_cast<int>(Event::Type::READ) };
    return AddPollerEvent(ev);

    // return true;
}


bool EventPoller::ProcessCommand(const Command& command) {
    Debug("ProcessCommand type:%d", command.type());
    if (!command.connection()->socket().IsValid()) {
        // the connection has been closed
        // do nothing
        return false;
    }
    int type = static_cast<int>(Event::Type::READ);
    if (static_cast<int>(command.type()) &
        static_cast<int>(Command::Type::WRITEABLE)) {
        type |= static_cast<int>(Event::Type::WRITE);
    }
    if (command.type() & static_cast<int>(Command::Type::ADD_CONN)) {
        Event event(command.connection()->socket().fd(), type);
        return AddPollerEvent(event);
    } else if (command.type() & static_cast<int>(Command::Type::REMOVE_CONN_IMMEDIATELY)) {
        type |= static_cast<int>(Event::Type::CLOSE);
        Event event(command.connection()->socket().fd(), type);
        return RemovePollerEvent(event);
    } else if (command.type() & static_cast<int>(Command::Type::READABLE) ||
        command.type() & static_cast<int>(Command::Type::WRITEABLE)) {
        if (command.type() == command.connection()->cached_event_type()) {
            return true;
        }
        command.connection()->set_cached_event_type(command.type());
        Event event(command.connection()->socket().fd(), type);
        return ModifyPollerEvent(event);
    } else if (command.type() & static_cast<int>(Command::Type::REMOVE_CONN)) {
        return true;
    }

    return false;
}

bool EventPoller::ProcessInterrupt() {
    _interrupter->Reset();
    auto event_disp = _event_disp.lock();
    if (event_disp) {
        return event_disp->ProcessAllPendingCommands(_id);
    }
    return false;
}

} // namespace tcp
} // namespace rocket