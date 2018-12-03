// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#include "tcp/tcp_server.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>

#include "base/end_point.h"
#include "base/log.h"
#include "tcp/event_dispatcher.h"
#include "tcp/connection_factory.h"

namespace rocket {
namespace tcp {

bool TcpServer::Launch(const rocket::base::EndPoint& local_address,
                       const TcpServerOptions& options) {
    _event_disp = EventDispatcher::New(options.name(), options.worker_count());
    assert(_event_disp.get());
    if (!_event_disp->Launch()) {
        return false;
    }

    // create listen socket
    rocket::base::ListenSocket listen_socket(local_address);
        if (!listen_socket.IsValid()) {
        return false;
    }

    if (!listen_socket.SetCloexec(true) 
        || !listen_socket.SetBlocking(false) 
        || !listen_socket.SetReceiveBufferSize(options.tcp_receive_buffer_size()) 
        || !listen_socket.SetSendBufferSize(options.tcp_send_buffer_size())
        || !listen_socket.SetReuseAddress(true)
        || !listen_socket.Listen()) {
        return false;
    }

    Debug("listen_socket.fd() = %d", listen_socket.fd());
    ConnectionFactory cf;
    auto connection = cf.CreateConnection(_event_disp, listen_socket.fd(), true);
    assert(connection.get());
    connection->set_connected_callback(options.connected_callback());
    auto listener = std::static_pointer_cast<ListenConnection>(connection);
    listener->set_tcp_server_options(options);

    // add the listen fd onto multiplexer
    Command cmd(static_cast<int>(Command::Type::ADD_CONN),
                std::static_pointer_cast<ConnectionBase>(listener));
    _event_disp->AddCommand(cmd, true);

    listen_socket.Detach();
    return true;
}

bool TcpServer::Shutdown() {
    _event_disp->Shutdown();
    return true;
}

} // namespace tcp
} // namespace rocket
