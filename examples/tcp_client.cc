#include <unistd.h>

#include <iostream>
#include <string>
#include <unordered_map>

#include "tcp/tcp_callbacks.h"
#include "tcp/tcp_client.h"
#include "tcp/tcp_connection.h"
#include "base/end_point.h"
#include "base/ip_address.h"
#include "base/string_utils.h"


int main(int argc, const char **argv) {
    if(argc != 3) {
        std::cout << "Usage: " << argv[0] << " <ip> <port>" << std::endl;
        return 1;
    }

    std::unordered_map<rocket::tcp::ConnectionId, int> counts;
    using TcpConnectionPtr = std::shared_ptr<rocket::tcp::TcpConnection>;

    rocket::tcp::TcpClientOptions tcp_options;
    tcp_options.set_send_buffer_size(1024 * 1024);
    tcp_options.set_receive_buffer_size(1024 * 1024);

    auto send_func = [] (TcpConnectionPtr c) -> bool {
        return c->SendPacket("client connect");
    };

    tcp_options.set_connected_callback([&send_func] (TcpConnectionPtr c) -> bool {
        std::cout << "Connected to the server" << std::endl;
        return send_func(c);
    });

    tcp_options.set_closed_callback([] (TcpConnectionPtr c) -> bool {
        //Info("Connection closed");
        std::cout << "Connection closed" << std::endl;
        (void) c;
        return true;
    });

    tcp_options.set_received_callback([&send_func, &counts] (
            TcpConnectionPtr c) -> bool {
        //Info("receive");
        std::cout << "receive" << std::endl;
        if (counts[c->id()]++ == 10) {
            c->MarkAsClosed();
            //Info("MarkAsClosed");
            std::cout << "MarkAsClosed" << std::endl;
            return true;
        } else {
            return send_func(c);
        }
    });

    tcp_options.set_sent_callback([] (bool success, TcpConnectionPtr c) -> bool {
        (void) c;
        //Info("Send packet successfully");
        std::cout << "Send packet successfully" << std::endl;
        return success;
    });

    rocket::tcp::TcpClient tcp_client;
    rocket::tcp::TcpClientOptions options;
    options.set_worker_count(1);
    if (!tcp_client.Launch("tcpcli", options)) {
        //Fatal("Failed to launch tcp client, exiting...");
        return 1;
    }

    std::string ip(argv[1]);
    std::string str_port(argv[2]);
    //int port = rocket::base::StringUtils::ToUint32(str_port);
    int port = 12346;
    rocket::base::EndPoint server_end_point(ip, 12346);
    //Info("server_end_point: %s  port:%d  str_port:%s", server_end_point.ToString().c_str(), port, str_port.c_str());
    std::cout << "server_end_point:" << server_end_point.ToString().c_str()
              << std::endl;
    for (auto i = 0; i < 8; ++i) {
        auto connection_id = tcp_client.Connect(&server_end_point, tcp_options);
        if (connection_id == rocket::tcp::INVALID_CONNECTION_ID) {
        //Info("Failed to connect to server");
        std::cout << "Failed to connect to server" << std::endl;
        return 1;
        }
    }

    sleep(2);

    tcp_client.Shutdown();

    return 0;
}