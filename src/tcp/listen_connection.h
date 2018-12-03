#ifndef ROCKET_TCP_LISTEN_CONNECTION_H_
#define ROCKET_TCP_LISTEN_CONNECTION_H_

#include "tcp/connection_base.h"
#include "tcp/tcp_options.h"

#include <memory>

namespace rocket {
namespace tcp {

class EventDispatcher;

class ListenConnection : public ConnectionBase {
public:
    friend class ConnectionFactory;
    virtual ~ListenConnection() {};

    const TcpServerOptions& tcp_server_options() const {
        return _options;
    }
    TcpServerOptions& mutable_tcp_server_options() {
        return _options;
    }
    void set_tcp_server_options(const TcpServerOptions& options) {
        _options = options;
    }

    virtual void HandleReadableEvent(EventDispatcher* event_disp);
    virtual void HandleWriteableEvent(EventDispatcher* event_disp);
    virtual void HandleCloseConnection() {
    }
    virtual void MarkAsClosed(bool immediately = true) {
    }

private:
    ListenConnection(std::shared_ptr<EventDispatcher> event_disp, int fd)
        : ConnectionBase(event_disp, fd) {
    }

    TcpServerOptions _options;
};

}  // namespace tcp
}  // namespace rocket

#endif  // ROCKET_TCP_LISTEN_CONNECTION_H_