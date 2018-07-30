#ifndef TCP_CALLBACKS_H_
#define TCP_CALLBACKS_H_

#include <memory>

#include "connection.h"

namespace rocket {

class Connection;

using ConnectedCallbackType =
    std::function<bool(std::shared_ptr<Connection>)>;
using ClosedCallbackType =
    std::function<bool(std::shared_ptr<Connection>)>;
using ReceivedCallbackType =
    std::function<bool(std::shared_ptr<Connection>)>;
using SentCallbackType =
    std::function<bool(bool, std::shared_ptr<Connection>)>;

}  // namespace chris

#endif  // TCP_CALLBACKS_H_