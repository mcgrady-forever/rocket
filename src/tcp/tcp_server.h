// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#ifndef ROCKET_TCP_TCP_SERVER_H_
#define ROCKET_TCP_TCP_SERVER_H_

#include <memory>
#include <functional>

#include "tcp/tcp_callbacks.h"
#include "tcp/tcp_options.h"
#include "base/end_point.h"

namespace rocket {
namespace tcp {

class EventDispatcher;

class TcpServer {
public:
    // you must first call this method before you do any requests
    bool Launch(const rocket::base::EndPoint& local_address,
                const TcpServerOptions& options = TcpServerOptions());
    bool Shutdown();

private:
    std::shared_ptr<EventDispatcher> _event_disp;

    // all callbacks
    ConnectedCallbackType            _connected_callback;
    ClosedCallbackType               _closed_callback;
    ReceivedCallbackType             _received_callback;
    SentCallbackType                 _sent_callback;
};

} // tcp
} // rocket

#endif // ROCKET_TCP_TCP_SERVER_H_