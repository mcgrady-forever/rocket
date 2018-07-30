// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#include "event.h"

#if defined(linux) || defined(__linux) || defined(__linux__)
#include <epoll_event_poller_impl.h>
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#include <poll_event_poller_impl.h>
#else
#include <select_event_poller_impl.h>
#endif

namespace rocket {

std::shared_ptr<EventPoller> EventPoller::New(size_t id,
                                              size_t max_connections) {
#if defined(linux) || defined(__linux) || defined(__linux__)
    return std::shared_ptr<EventPoller>(
        new EpollEventPollerImpl(id, max_connections));
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
    return std::shared_ptr<EventPoller>(
        new PollEventPollerImpl(id, max_connections));
#else
    return std::shared_ptr<EventPoller>(
        new SelectEventPollerImpl(id, max_connections));
#endif
}

bool EventPoller::Init(std::shared_ptr<EventCenter> event_center) {
    if (!event_center) {
        return false;
    }
    event_center_ = event_center;

    interrupter_ = Interrupter::New();
    if (!interrupter_->Create()) {
        return false;
    }
    if (!DoInit()) {
        interrupter_.reset();
        return false;
    }
    Event ev { interrupter_->get_read_fd(),
                static_cast<int>(Event::Type::READ) };
    return AddPollerEvent(std::move(ev));
}


bool EventPoller::ProcessCommand(const Command& command) {
    if (!command.connection()->socket().IsValid()) {
        // the connection has been closed
        // do nothing
        return false;
    }
    int type = static_cast<int>(Event::Type::kRead);
    if (static_cast<int>(command.type()) &
        static_cast<int>(Command::Type::kWriteable)) {
        type |= static_cast<int>(Event::Type::kWrite);
    }
    if (command.type() & static_cast<int>(Command::Type::kAddConn)) {
        return AddPollerEvent(Event(command.connection()->socket().fd(), type));
    } else if (command.type() & static_cast<int>(Command::Type::kRemoveConnImmediately)) {
        type |= static_cast<int>(Event::Type::kClose);
        return RemovePollerEvent(Event(command.connection()->socket().fd(), type));
    } else if (command.type() & static_cast<int>(Command::Type::kReadable) ||
        command.type() & static_cast<int>(Command::Type::kWriteable)) {
        if (command.type() == command.connection()->cached_event_type()) {
            return true;
        }
        command.connection()->set_cached_event_type(command.type());
        return ModifyPollerEvent(Event(command.connection()->socket().fd(), type));
    } else if (command.type() & static_cast<int>(Command::Type::kRemoveConn)) {
        return true;
    }

    return false;
}

bool EventPoller::ProcessInterrupt() {
    interrupter_->Reset();
    auto event_center = event_center_.lock();
    if (event_center) {
        return event_center->ProcessAllPendingCommands(id_);
    }
    return false;
}

} // namespace rocket