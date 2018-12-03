#include "tcp/listen_connection.h"

#include <assert.h>

#include "tcp/connection_factory.h"
#include "tcp/command.h"
#include "tcp/event_dispatcher.h"
#include "base/socket.h"
#include "base/log.h"


namespace rocket {
namespace tcp {

// This method will be called when a socket fd becomes readable
void ListenConnection::HandleReadableEvent(EventDispatcher* event_disp) {
    assert(event_disp);

    base::ListenSocket listen_socket;
    listen_socket.Attach(_socket.fd());

    base::TcpSocket new_socket;
    base::EndPoint remote_end_point;
    if (!listen_socket.Accept(&new_socket, &remote_end_point)) {
        listen_socket.Detach();
        return;
    }
    listen_socket.Detach();

    new_socket.SetCloexec(true);
    new_socket.SetBlocking(false);
    new_socket.SetTcpNoDelay(true);
    new_socket.SetKeepAlive(true);
    new_socket.SetLinger(false);
    new_socket.SetSendBufferSize(_options.tcp_send_buffer_size());
    new_socket.SetReceiveBufferSize(_options.tcp_receive_buffer_size());

    ConnectionFactory cf;
    Debug("Accept fd:%d", new_socket.fd());
    auto new_connection = cf.CreateConnection(_event_disp.lock(),
                                              new_socket.fd(), 
                                              false);
    auto new_tcp_connection = std::static_pointer_cast<TcpConnection>(new_connection);
    new_tcp_connection->set_closed_callback(_options.closed_callback());
    new_tcp_connection->set_sent_callback(_options.sent_callback());
    new_tcp_connection->set_received_callback(_options.received_callback());
    new_tcp_connection->set_state(TcpConnection::State::CONNECTED);
    new_tcp_connection->SetSendBufferSize(_options.send_buffer_size());
    new_tcp_connection->SetRecvBufferSize(_options.receive_buffer_size());
    new_tcp_connection->set_remote_end_point(remote_end_point);
    Debug("HandleReadableEvent Accept remote_end_point:%s", remote_end_point.ToString().c_str());

    new_socket.Detach();

    if (_connected_callback) {
        // call callback user defined
        _connected_callback(new_tcp_connection);
    }

    event_disp->AddCommand(
        Command(static_cast<int>(Command::Type::ADD_CONN), new_connection), true);
    return;
}

void ListenConnection::HandleWriteableEvent(EventDispatcher* event_disp) {
    assert(event_disp);
    event_disp->AddCommand(
        Command(static_cast<int>(Command::Type::READABLE), shared_from_this()),
        true);
}

}  // namespace tcp
}  // namespace rocket