#ifndef ROCKET_TCP_CONNECTION_FACTORY_H_
#define ROCKET_TCP_CONNECTION_FACTORY_H_

#include "tcp/connection_base.h"
#include "tcp/listen_connection.h"
#include "tcp/tcp_connection.h"

namespace rocket {
namespace tcp {

class EventDispatcher;

class ConnectionFactory {
public:
    ConnectionFactory() {};
    virtual ~ConnectionFactory() {};

    std::shared_ptr<ConnectionBase> CreateConnection(std::shared_ptr<EventDispatcher> event_disp,
                                                     int fd,
                                                     bool is_listener) {
        if (is_listener) {
            return std::shared_ptr<ConnectionBase>(new ListenConnection(event_disp, fd));
        } else {
            return std::shared_ptr<ConnectionBase>(new TcpConnection(event_disp, fd));
        }
    }
};

}  // namespace tcp
}  // namespace rocket

#endif  // ROCKET_TCP_CONNECTION_FACTORY_H_