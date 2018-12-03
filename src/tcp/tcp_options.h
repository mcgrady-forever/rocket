#ifndef ROCKET_TCP_TCP_OPTIONS_H_
#define ROCKET_TCP_TCP_OPTIONS_H_

#include <string>
#include <memory>
#include <functional>

#include "tcp/tcp_callbacks.h"


namespace rocket {
namespace tcp {

class TcpOptions {
public:
    TcpOptions()
        : _worker_count(0)
        , _max_command_queue_len(1024)
        , _tcp_send_buffer_size(32 * 1024)
        , _tcp_receive_buffer_size(32 * 1024)
        , _send_buffer_size(0)
        , _receive_buffer_size(0)
        , _connected_callback(NULL)
        , _closed_callback(NULL)
        , _received_callback(NULL)
        , _sent_callback(NULL) {
    }

    virtual ~TcpOptions() {};

    size_t worker_count() const {
        return _worker_count;
    }
    void set_worker_count(size_t worker_count) {
        _worker_count = worker_count;
    }

    size_t max_command_queue_len() const {
        return _max_command_queue_len;
    }
    void set_max_command_queue_len(size_t len) {
        _max_command_queue_len = len;
    }

    size_t tcp_send_buffer_size() const {
        return _tcp_send_buffer_size;
    }
    void set_tcp_send_buffer_size(size_t size) {
        _tcp_send_buffer_size = size;
    }

    size_t tcp_receive_buffer_size() const {
        return _tcp_receive_buffer_size;
    }
    void set_tcp_receive_buffer_size(size_t size) {
        _tcp_receive_buffer_size = size;
    }

    size_t send_buffer_size() const {
        return _send_buffer_size;
    }
    void set_send_buffer_size(size_t size) {
        _send_buffer_size = size;
    }

    size_t receive_buffer_size() const {
        return _receive_buffer_size;
    }
    void set_receive_buffer_size(size_t size) {
        _receive_buffer_size = size;
    }

    const ConnectedCallbackType& connected_callback() const {
        return _connected_callback;
    }
    ConnectedCallbackType& mutable_connected_callback() {
        return _connected_callback;
    }
    void set_connected_callback(const ConnectedCallbackType& connected_callback) {
        _connected_callback = connected_callback;
    }

    const ClosedCallbackType& closed_callback() const {
        return _closed_callback;
    }
    ClosedCallbackType& mutable_closed_callback() {
        return _closed_callback;
    }
    void set_closed_callback(const ClosedCallbackType& closed_callback) {
        _closed_callback = closed_callback;
    }

    const ReceivedCallbackType& received_callback() const {
        return _received_callback;
    }
    ReceivedCallbackType& mutable_received_callback() {
        return _received_callback;
    }
    void set_received_callback(const ReceivedCallbackType& received_callback) {
        _received_callback = received_callback;
    }

    const SentCallbackType& sent_callback() const {
        return _sent_callback;
    }
    SentCallbackType& mutable_sent_callback() {
        return _sent_callback;
    }
    void set_sent_callback(const SentCallbackType& sent_callback) {
        _sent_callback = sent_callback;
    }

private:
    size_t                  _worker_count;
    size_t                  _max_command_queue_len;
    size_t                  _tcp_send_buffer_size;
    size_t                  _tcp_receive_buffer_size;
    size_t                  _send_buffer_size;
    size_t                  _receive_buffer_size;
    ConnectedCallbackType   _connected_callback;
    ClosedCallbackType      _closed_callback;
    ReceivedCallbackType    _received_callback;
    SentCallbackType        _sent_callback;
};

class TcpServerOptions : public TcpOptions {
public:
    TcpServerOptions() 
        : _name("default") {
    };
    virtual ~TcpServerOptions() {};

    const std::string& name() const {
        return _name;
    }
    void set_name(const std::string& name) {
        _name = name;
    }

private:
    std::string _name;
};

class TcpClientOptions : public TcpOptions {
public:
    TcpClientOptions() {};
    virtual ~TcpClientOptions() {};
};

} // tcp
} // rocket

#endif // ROCKET_TCP_TCP_OPTIONS_H_