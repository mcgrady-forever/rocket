// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#ifndef TCP_SERVER_H_
#define TCP_SERVER_H_

#include <memory>
#include <functional>

#include "tcp_callbacks.h"
#include "tcp_options.h"

namespace rocket {

class TcpServer {
public:
    // you must first call this method before you do any requests
    bool Launch(const base::EndPoint& local_address,
                const TcpServerOptions& options = TcpServerOptions());
    bool Shutdown();

private:
    std::shared_ptr<EventDispatcher> event_disp_;

    // all callbacks
    ConnectedCallbackType connected_callback_;
    ClosedCallbackType closed_callback_;
    ReceivedCallbackType received_callback_;
    SentCallbackType sent_callback_;
};

}

#endif // TCP_SERVER_H_