#ifndef ROCKET_TCP_CONNECTION_BASE_H_
#define ROCKET_TCP_CONNECTION_BASE_H_

#include <functional>
#include <memory>
#include <string>
#include <atomic>

#include "tcp/connection_base.h"
#include "tcp/tcp_callbacks.h"
#include "tcp/connection_id.h"
#include "container/ring_buffer.h"
#include "base/socket.h"


namespace rocket {
namespace tcp {

class EventDispatcher;

class ConnectionBase : public std::enable_shared_from_this<ConnectionBase> {
public:
    enum State {
        CONNECTING  = 0,
        CONNECTED   = 1,
        CLOSING     = 2,
        CLOSED      = 3,
    };

    virtual ~ConnectionBase() {};

    const base::TcpSocket& socket() const {
        return _socket;
    }
    base::TcpSocket& mutable_socket()  {
        return _socket;
    }

    rocket::tcp::ConnectionId id() const {
        return _id;
    }

    const std::thread::id& ep_thread_id() const {
        return _ep_thread_id;
    }
    void set_ep_thread_id() {
        _ep_thread_id = std::this_thread::get_id();
    }

    const ConnectedCallbackType& connected_callback() const {
        return _connected_callback;
    }
    ConnectedCallbackType& mutable_connected_callback() {
        return _connected_callback;
    }
    void set_connected_callback(const ConnectedCallbackType& connected_callback) {
        _connected_callback = connected_callback;
    }

    int cached_event_type() const {
        return _cached_event_type;
    }
    void set_cached_event_type(int event_type) {
        _cached_event_type = event_type;
    }

    State state() const {
        return _state;
    }
    void set_state(State state) {
        _state = state;
    }

    // These three methods will be called by the event poller thread when a
    // socket fd becomes readable or writable
    // NOTE: user should not care about them
    virtual void HandleReadableEvent(EventDispatcher* event_disp) = 0;
    virtual void HandleWriteableEvent(EventDispatcher* event_disp) = 0;
    virtual void HandleCloseConnection() = 0;
    virtual void MarkAsClosed(bool immediately = true) = 0;

protected:
    ConnectionBase(std::shared_ptr<EventDispatcher> event_disp, int fd)
        : _event_disp(event_disp)
        , _id(ConnectionIdGenerator::Generate())
        , _connected_callback(NULL)
        , _cached_event_type(0)
        , _state(State::CONNECTING) {
        _socket.Attach(fd);
    }

    std::weak_ptr<EventDispatcher>  _event_disp;
    rocket::tcp::ConnectionId       _id;
    base::TcpSocket                 _socket;

    // the event poller thread id
    std::thread::id                 _ep_thread_id;
    ConnectedCallbackType           _connected_callback;
    int                             _cached_event_type;
    State                           _state;
};

}  // namespace tcp
}  // namespace rocket

#endif  // ROCKET_TCP_CONNECTION_BASE_H_