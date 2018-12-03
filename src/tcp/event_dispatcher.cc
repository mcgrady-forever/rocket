#include "event_dispatcher.h"

#include <cstdlib>
#include <cstdio>

#include <string>
#include <iostream>
#include <thread>

#include "base/log.h"


namespace rocket {
namespace tcp {

namespace {
    const size_t DEFAULT_THREAD_NUM = 5;
}

std::shared_ptr<EventDispatcher> EventDispatcher::New(const std::string &name, size_t thread_num) {
    if (thread_num <= 0) {
        thread_num = std::thread::hardware_concurrency();
    }
    if (thread_num <= 0) {
        thread_num = DEFAULT_THREAD_NUM;
    }

    return std::shared_ptr<EventDispatcher>(new EventDispatcher(name, thread_num));
}

EventDispatcher::EventDispatcher(const std::string &name, size_t thread_num)
        : _internal_event_poller_infos(thread_num)
        , _name(name) {
    for (size_t i = 0; i < thread_num; ++i) {
        _internal_event_poller_infos[i] = std::make_shared<InternalEventPollerInfo>();
        _internal_event_poller_infos[i]->_event_poller = EventPoller::New(i);
        assert((_internal_event_poller_infos[i]->_event_poller).get());
    }
}

bool EventDispatcher::Launch() {
    for (size_t i = 0; i < _internal_event_poller_infos.size(); ++i) {
        if (!_internal_event_poller_infos[i]->_event_poller->Init(shared_from_this())) {
            Fatal("Failed to initialize EventPoller.");
        }
        Info("EventPoller %d initialized.", i);
        std::shared_ptr<concurrency::Task> task(
                new InternalEventTask(shared_from_this(), _internal_event_poller_infos[i]->_event_poller));
        // Can not start the thread until put it into vector
        auto t = std::make_shared<concurrency::Thread>(task, _name + "-poller-" + std::to_string(i));
        _internal_event_poller_infos[i]->_event_poller_thread = t;
        t->Start();
    }
    return true;
}

void EventDispatcher::Shutdown() {
    for (auto &poller_info : _internal_event_poller_infos) {
        poller_info->_event_poller_thread->Stop();
    }
    _internal_event_poller_infos.clear();
}

void EventDispatcher::AddCommand(const Command &command, bool async) {
    int id = command.connection()->id() % _internal_event_poller_infos.size();

    auto &info = _internal_event_poller_infos[id];
    if (async) {
        Debug("AddCommand 0 %d", id);
        {
            std::lock_guard<std::mutex> guard(info->_pending_commands_mutex);
            (info->_pending_commands).push_back(command);
        }
        Debug("AddCommand 1 %d", id);
        info->_event_poller->Interrupt();
    } else {
        assert(command.connection()->ep_thread_id() == std::this_thread::get_id());
        ProcessPendingCommand(info, command);
    }
}

bool EventDispatcher::ProcessAllPendingCommands(size_t id) {
    if (id >= _internal_event_poller_infos.size()) {
        return false;
    }

    auto &info = _internal_event_poller_infos[id];
    if (info->_event_poller_thread->GetStatus() == concurrency::Thread::Status::STOP) {
        return false;
    }

    std::vector<Command> pending_commands;
    {
        std::lock_guard<std::mutex> guard(info->_pending_commands_mutex);
        pending_commands.swap(info->_pending_commands);
    }

    for (auto &command : pending_commands) {
        ProcessPendingCommand(info, command);
    }
    return true;
}

void EventDispatcher::ProcessPendingCommand(InternalEventPollerInfoPtr info, const Command &command) {
    if (info->_event_poller->ProcessCommand(command)) {
        Debug("ProcessPendingCommand ProcessCommand success cmd:%d", command.type());
        if (command.type() & static_cast<int>(Command::Type::ADD_CONN)) {
            info->_connections[command.connection()->socket().fd()] = command.connection();
            command.connection()->set_ep_thread_id();
            command.connection()->HandleReadableEvent(this);
        } else if (command.type() & static_cast<int>(Command::Type::REMOVE_CONN_IMMEDIATELY)) {
            (info->_connections).erase(command.connection()->socket().fd());
            command.connection()->HandleCloseConnection();
        } else if (command.type() & static_cast<int>(Command::Type::REMOVE_CONN)) {
            command.connection()->set_state(ConnectionBase::State::CLOSING);
            command.connection()->HandleWriteableEvent(this);
        }
    } else {
        Error("ProcessCommand Error");
    }
}

bool EventDispatcher::ProcessEvent(const Event &event, size_t id) {
    if (id >= _internal_event_poller_infos.size()) {
        return false;
    }

    int fd = event.fd();
    if (fd < 0) {
        return false;
    }

    auto &connections = _internal_event_poller_infos[id]->_connections;
    auto itr = connections.find(event.fd());
    if (itr != connections.end()) {
        auto connection = itr->second;
        if (event.mask() & static_cast<int>(Event::Type::CLOSE)) {
            connection->MarkAsClosed(true);
        } else {
            if (event.mask() & static_cast<int>(Event::Type::READ)) {
                connection->HandleReadableEvent(this);
            }
            if (event.mask() & static_cast<int>(Event::Type::WRITE)) {
                connection->HandleWriteableEvent(this);
            }
        }
    }

    return true;
}

EventDispatcher::InternalEventTask::InternalEventTask(std::weak_ptr<EventDispatcher> event_center,
                                                      std::weak_ptr<EventPoller> event_poller)
        : _event_disp(event_center)
        , _event_poller(event_poller) {
}

EventDispatcher::InternalEventTask::~InternalEventTask() {
}

bool EventDispatcher::InternalEventTask::operator()(void *arg) {
    (void) arg;
    auto event_poller = _event_poller.lock();
    auto event_disp = _event_disp.lock();
    if (!event_poller || !event_disp) {
        return false;
    }

    //Debug("EventDispatcher::InternalEventTask::operator 1");
    while (!IsStopped()) {
        //Debug("EventDispatcher::InternalEventTask::operator 2");
        if (!event_poller->Poll()) {
            event_poller->Shutdown();
            Debug("EventDispatcher::InternalEventTask::operator end");
            return false;
        }
    }
    event_poller->Shutdown();
    return true;
}

void EventDispatcher::InternalEventTask::Stop() {
    _stop.store(true, std::memory_order_release);
    auto event_poller = _event_poller.lock();
    if (event_poller) {
        event_poller->Interrupt();
    }
}

} // namespace tcp
} // namspace rocket
