// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#include "end_point.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>


namespace rocket {

bool TcpServer::Launch(const EndPoint& local_address,
                       const TcpServerOptions& options) {
    event_center_ = EventCenter::New(options.name(), options.worker_count());
    assert(event_center_.get());
    if (!event_center_->Launch()) {
        return false;
    }

    // create listen socket
    base::ListenSocket listen_socket(local_address);
        if (!listen_socket.IsValid()) {
        return false;
    }

    if (!listen_socket.SetCloexec(true) ||
        !listen_socket.SetBlocking(false) ||
        !listen_socket.SetReceiveBufferSize(options.tcp_receive_buffer_size()) ||
        !listen_socket.SetSendBufferSize(options.tcp_send_buffer_size()) ||
        !listen_socket.SetReuseAddress(true) ||
        !listen_socket.Listen()) {
        return false;
    }

    ConnectionFactory cf;
    auto connection =
        cf.CreateConnection(event_center_, listen_socket.fd(), true);
    assert(connection.get());
    connection->set_connected_callback(options.connected_callback());
    auto listener = std::static_pointer_cast<ListenConnection>(connection);
    listener->set_tcp_server_options(options);

    // add the listen fd onto multiplexer
    Command cmd(static_cast<int>(Command::Type::kAddConn),
                std::static_pointer_cast<ConnectionBase>(listener));
    event_center_->AddCommand(cmd, true);

    listen_socket.Detach();
    return true;
}

bool TcpServer::Shutdown() {
    event_center_->Shutdown();
    return true;
}

} // rocket
