#include "tcp/tcp_client.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "tcp/connection_factory.h"
#include "base/end_point.h"
#include "base/socket.h"


namespace rocket {
namespace tcp {

bool TcpClient::Launch(const std::string& name,
                       const TcpClientOptions& options) {
    _event_disp = EventDispatcher::New(name, options.worker_count());
    assert(_event_disp.get());
    return _event_disp->Launch();
}

bool TcpClient::Shutdown() {
    _event_disp->Shutdown();
    _contexts.clear();
    return true;
}

ConnectionId TcpClient::Connect(const base::EndPoint* remote,
                                const TcpClientOptions& options,
                                std::shared_ptr<void> cookie) {
    assert(remote);

    base::TcpSocket socket;
    if (!socket.Create() ||
        !socket.SetCloexec() ||
        !socket.SetBlocking(false) ||
        !socket.SetTcpNoDelay() ||
        !socket.SetKeepAlive() ||
        !socket.SetSendBufferSize(options.tcp_send_buffer_size()) ||
        !socket.SetReceiveBufferSize(options.tcp_receive_buffer_size()) ||
        !socket.Connect(*remote)) {
        return INVALID_CONNECTION_ID;
    }

    InternalConnectionContext cc;
    cc.status = Status::CONNECTING;
    cc.options = options;
    cc.tcp_connection.reset();

    ConnectionFactory cf;
    auto connection = cf.CreateConnection(_event_disp, socket.fd(), false);
    auto tcp_connection = std::static_pointer_cast<TcpConnection>(connection);
    tcp_connection->SetSendBufferSize(options.send_buffer_size());
    tcp_connection->SetRecvBufferSize(options.receive_buffer_size());
    tcp_connection->set_cookie(cookie);
    tcp_connection->set_remote_end_point(*remote);
    cc.tcp_connection = tcp_connection;
    std::unique_lock<std::mutex> guard(_contexts_mutex);
    _contexts[connection->id()] = cc;
    guard.unlock();

    tcp_connection->set_connected_callback(
        [this] (std::shared_ptr<TcpConnection> c) -> bool {
            return this->OnConnected(c);
        }
    );
    tcp_connection->set_closed_callback(
        [this] (std::shared_ptr<TcpConnection> c) -> bool {
            return this->OnClosed(c);
        }
    );
    tcp_connection->set_sent_callback(
        [this] (bool status, std::shared_ptr<TcpConnection> c) -> bool {
            return this->OnSent(status, c);
        }
    );
    tcp_connection->set_received_callback(
        [this] (std::shared_ptr<TcpConnection> c) -> bool {
            return this->OnReceived(c);
        }
    );

    socket.Detach();

    _event_disp->AddCommand(
        Command(static_cast<int>(Command::Type::ADD_CONN), connection), true);
    return connection->id();
}

bool TcpClient::AsyncClosed(ConnectionId connection_id) {
    std::unique_lock<std::mutex> guard(_contexts_mutex);
    auto itr = _contexts.find(connection_id);
    if (itr == _contexts.end()) {
        return false;
    }
    auto connection = itr->second.tcp_connection;
    guard.unlock();
    connection->MarkAsClosed();
    return true;
}

bool TcpClient::OnConnected(std::shared_ptr<TcpConnection> tcp_connection) {
    assert(tcp_connection.get());
    std::unique_lock<std::mutex> guard(_contexts_mutex);
    auto itr = _contexts.find(tcp_connection->id());
    assert(itr != _contexts.end());
    itr->second.status = Status::CONNECTED;
    if (itr->second.options.connected_callback()) {
        auto& cb = itr->second.options.mutable_connected_callback();
        guard.unlock();
        return cb(tcp_connection);
    }
    return true;
}

bool TcpClient::OnClosed(std::shared_ptr<TcpConnection> tcp_connection) {
    assert(tcp_connection.get());
    std::unique_lock<std::mutex> guard(_contexts_mutex);
    auto itr = _contexts.find(tcp_connection->id());
    assert(itr != _contexts.end());
    itr->second.status = Status::CLOSED;
    bool res = true;
    if (itr->second.options.closed_callback()) {
        auto& cb = itr->second.options.mutable_closed_callback();
        guard.unlock();
        res = cb(tcp_connection);
        guard.lock();
        _contexts.erase(itr);
        return res;
    }
    _contexts.erase(itr);
    return res;
}

bool TcpClient::OnSent(bool success,
                       std::shared_ptr<TcpConnection> tcp_connection) {
    assert(tcp_connection.get());
    std::unique_lock<std::mutex> guard(_contexts_mutex);
    auto itr = _contexts.find(tcp_connection->id());
    assert(itr != _contexts.end());
    assert(itr->second.status == Status::CONNECTED);
    if (itr->second.options.sent_callback()) {
        auto& cb = itr->second.options.mutable_sent_callback();
        guard.unlock();
        return cb(success, tcp_connection);
    }
    return true;
}

bool TcpClient::OnReceived(std::shared_ptr<TcpConnection> tcp_connection) {
    assert(tcp_connection.get());
    std::unique_lock<std::mutex> guard(_contexts_mutex);
    auto itr = _contexts.find(tcp_connection->id());
    assert(itr != _contexts.end());
    assert(itr->second.status == Status::CONNECTED);
    if (itr->second.options.received_callback()) {
        auto& cb = itr->second.options.mutable_received_callback();
        guard.unlock();
        return cb(tcp_connection);
    }
    return true;
}

}  // namespace tcp
}  // namespace rocket