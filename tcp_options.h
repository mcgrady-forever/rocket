#ifndef TCP_OPTIONS_H_
#define TCP_OPTIONS_H_

#include <string>
#include <memory>
#include <functional>

#include "tcp_callbacks.h"


namespace rocket {

class TcpOptions {
public:
    TcpOptions()
        : _worker_count(0)
        , max_command_queue_len_(1024)
        , tcp_send_buffer_size_(32 * 1024)
        , tcp_receive_buffer_size_(32 * 1024)
        , send_buffer_size_(0)
        , receive_buffer_size_(0)
        , connected_callback_(NULL)
        , closed_callback_(NULL)
        , received_callback_(NULL)
        , sent_callback_(NULL) {
    }

    virtual ~TcpOptions() {};

    size_t worker_count() const {
        return worker_count_;
    }
    void set_worker_count(size_t worker_count) {
        worker_count_ = worker_count;
    }

    size_t max_command_queue_len() const {
        return max_command_queue_len_;
    }
    void set_max_command_queue_len(size_t len) {
        max_command_queue_len_ = len;
    }

    size_t tcp_send_buffer_size() const {
        return tcp_send_buffer_size_;
    }
    void set_tcp_send_buffer_size(size_t size) {
        tcp_send_buffer_size_ = size;
    }

    size_t tcp_receive_buffer_size() const {
        return tcp_receive_buffer_size_;
    }
    void set_tcp_receive_buffer_size(size_t size) {
        tcp_receive_buffer_size_ = size;
    }

    size_t send_buffer_size() const {
        return send_buffer_size_;
    }
    void set_send_buffer_size(size_t size) {
        send_buffer_size_ = size;
    }

    size_t receive_buffer_size() const {
        return receive_buffer_size_;
    }
    void set_receive_buffer_size(size_t size) {
        receive_buffer_size_ = size;
    }

    const ConnectedCallbackType& connected_callback() const {
        return connected_callback_;
    }
    ConnectedCallbackType& mutable_connected_callback() {
        return connected_callback_;
    }
    void set_connected_callback(const ConnectedCallbackType& connected_callback) {
        connected_callback_ = connected_callback;
    }

    const ClosedCallbackType& closed_callback() const {
        return closed_callback_;
    }
    ClosedCallbackType& mutable_closed_callback() {
        return closed_callback_;
    }
    void set_closed_callback(const ClosedCallbackType& closed_callback) {
        closed_callback_ = closed_callback;
    }

    const ReceivedCallbackType& received_callback() const {
        return received_callback_;
    }
    ReceivedCallbackType& mutable_received_callback() {
        return received_callback_;
    }
    void set_received_callback(const ReceivedCallbackType& received_callback) {
        received_callback_ = received_callback;
    }

    const SentCallbackType& sent_callback() const {
        return sent_callback_;
    }
    SentCallbackType& mutable_sent_callback() {
        return sent_callback_;
    }
    void set_sent_callback(const SentCallbackType& sent_callback) {
        sent_callback_ = sent_callback;
    }

private:
    size_t worker_count_;
    size_t max_command_queue_len_;
    size_t tcp_send_buffer_size_;
    size_t tcp_receive_buffer_size_;
    size_t send_buffer_size_;
    size_t receive_buffer_size_;
    ConnectedCallbackType connected_callback_;
    ClosedCallbackType closed_callback_;
    ReceivedCallbackType received_callback_;
    SentCallbackType sent_callback_;
};

class TcpServerOptions : public TcpOptions {
public:
    TcpServerOptions() 
        : _name("default") {
    };
    virtual ~TcpServerOptions() {};

    const std::string& name() const {
        return name_;
    }
    void set_name(const std::string& name) {
        name_ = name;
    }

private:
    std::string name_;
};

class TcpClientOptions : public TcpOptions {
public:
    TcpClientOptions() {};
    virtual ~TcpClientOptions() {};
};

} // rocket

#endif // TCP_OPTIONS_H_