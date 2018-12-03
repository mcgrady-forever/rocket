#ifndef ROCKET_TCP_TCP_CALLBACKS_H_
#define ROCKET_TCP_TCP_CALLBACKS_H_

#include <memory>

//#include "tcp/tcp_connection.h"

namespace rocket {
namespace tcp {

class TcpConnection;

using ConnectedCallbackType =
    std::function<bool(std::shared_ptr<TcpConnection>)>;
using ClosedCallbackType =
    std::function<bool(std::shared_ptr<TcpConnection>)>;
using ReceivedCallbackType =
    std::function<bool(std::shared_ptr<TcpConnection>)>;
using SentCallbackType =
    std::function<bool(bool, std::shared_ptr<TcpConnection>)>;

} // tcp
} // rocket

#endif  // ROCKET_TCP_TCP_CALLBACKS_H_