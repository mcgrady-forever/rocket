#ifndef ROCKET_TCP_TCP_CLIENT_H_
#define ROCKET_TCP_TCP_CLIENT_H_

#include <mutex>
#include <unordered_map>

#include "tcp/connection_id.h"
#include "tcp/event_dispatcher.h"
#include "tcp/tcp_callbacks.h"
#include "tcp/tcp_options.h"
#include "base/end_point.h"


namespace rocket {
namespace tcp {

class TcpClient {
public:
    TcpClient() {};
    ~TcpClient() {};

    bool Launch(const std::string& name,
        const TcpClientOptions& options = TcpClientOptions());
    bool Shutdown();

    // connect with remote server
    // if keep local as nullptr, it will use one of local ip.
    ConnectionId Connect(const base::EndPoint* remote,
                        const TcpClientOptions& options = TcpClientOptions(),
                        std::shared_ptr<void> cookie = nullptr);

    bool AsyncClosed(ConnectionId connection_id);

private:
    enum class Status {
        INITIALIZED, 
        CONNECTING,
        CONNECTED,
        CLOSED
    };

    struct InternalConnectionContext {
        Status status;
        TcpClientOptions options;
        std::shared_ptr<TcpConnection> tcp_connection;
    };

    bool OnConnected(std::shared_ptr<TcpConnection> tcp_connection);

    bool OnClosed(std::shared_ptr<TcpConnection> tcp_connection);

    bool OnSent(bool success, std::shared_ptr<TcpConnection> tcp_connection);

    bool OnReceived(std::shared_ptr<TcpConnection> tcp_connection);

    std::unordered_map<ConnectionId, InternalConnectionContext>     _contexts;
    std::mutex                                                      _contexts_mutex;
    std::shared_ptr<EventDispatcher>                                _event_disp;

private:
    ROCKET_DECLARE_UNCOPYABLE(TcpClient);
};

}  // namespace tcp
}  // namespace rocket

#endif  // ROCKET_TCP_TCP_CLIENT_H_