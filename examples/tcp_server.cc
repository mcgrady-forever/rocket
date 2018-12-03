#include <unistd.h>

#include <iostream>
#include <memory>

#include "base/end_point.h"
#include "base/ip_address.h"
#include "base/string_utils.h"
#include "tcp/tcp_connection.h"
#include "tcp/tcp_server.h"
#include "base/log.h"


class TcpServerHandler {
public:
    TcpServerHandler() {};
    ~TcpServerHandler() {};

    bool OnConnected(std::shared_ptr<rocket::tcp::TcpConnection> c) {
        Info("A new connection arrived");
        assert(c.get());
        (void) c;
        return true;
    }

    bool OnReceived(std::shared_ptr<rocket::tcp::TcpConnection> c) {
        Info("Data received");
        assert(c.get());

        std::string rsp("echo rsp");
        c->SendPacket(rsp);
        //c->MarkAsClosed(false);
        return true;
    }

    bool OnClosed(std::shared_ptr<rocket::tcp::TcpConnection> c) {
        assert(c.get());
        (void) c;
        Info("Connection closed");
        return true;
    }

    bool OnSent(bool success, std::shared_ptr<rocket::tcp::TcpConnection> c) {
        (void) c;
        Info("Message has been sent");
        return success;
    }
};

int main() {
    rocket::base::IPAddress listen_ip("127.0.0.1");
    rocket::base::EndPoint listen_end_point(listen_ip, 12346);

    TcpServerHandler tcp_handler;
    rocket::tcp::TcpServerOptions options;
    options.set_connected_callback(
        [&tcp_handler] (
            std::shared_ptr<rocket::tcp::TcpConnection> c) -> bool {
        return tcp_handler.OnConnected(c);
    });
    options.set_closed_callback(
        [&tcp_handler] (
            std::shared_ptr<rocket::tcp::TcpConnection> c) -> bool {
        return tcp_handler.OnClosed(c);
    });
    options.set_received_callback(
        [&tcp_handler] (
            std::shared_ptr<rocket::tcp::TcpConnection> c) -> bool {
        return tcp_handler.OnReceived(c);
    });
    options.set_sent_callback(
        [&tcp_handler] (
            bool success,
            std::shared_ptr<rocket::tcp::TcpConnection> c) -> bool {
        return tcp_handler.OnSent(success, c);
    });

    rocket::tcp::TcpServer tcp_server;
    if (!tcp_server.Launch(listen_end_point, options)) {
        Fatal("Failed to launch http server, exiting...");
    } else {
        Info("server started, listen_end_point:%s",
             listen_end_point.ToString().c_str());
    }

    ::sleep(1000);

    return 0;
}

