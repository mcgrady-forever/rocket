#ifndef ROCKET_TCP_TCP_CONNECTION_H_
#define ROCKET_TCP_TCP_CONNECTION_H_

#include <atomic>
#include <memory>
#include <string>
#include <list>

#include "tcp/connection_base.h"
#include "container/ring_buffer.h"
#include "tcp/tcp_callbacks.h"
//#include <cnetpp/base/string_piece.h>
//#include "concurrency/spin_lock.h"


namespace rocket {
namespace tcp {

class EventDispatcher;

class TcpConnection : public ConnectionBase {
public:
    friend class ConnectionFactory;

    virtual ~TcpConnection() {};

    void SetSendBufferSize(size_t send_buffer_size) {
        _send_buffer_size = send_buffer_size;
    }

    void SetRecvBufferSize(size_t recv_buffer_size) {
        _receive_buffer_size = recv_buffer_size;
    }

    const rocket::container::RingBuffer& recv_buffer() const {
        return _recv_buffer;
    }
    rocket::container::RingBuffer& mutable_recv_buffer() {
        return _recv_buffer;
    }

    int status() const {
        return _status;
    }

    const std::string& error_message() const {
        return _error_message;
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

    const SentCallbackType& sent_callback() const {
        return _sent_callback;
    }
    SentCallbackType mutable_sent_callback() {
        return _sent_callback;
    }
    void set_sent_callback(const SentCallbackType& sent_callback) {
        _sent_callback = sent_callback;
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

    std::shared_ptr<void> cookie() {
        return _cookie;
    }
    void set_cookie(std::shared_ptr<void> cookie) {
        _cookie = cookie;
    }

    const base::EndPoint& remote_end_point() const {
        return _remote_end_point;
    }
    base::EndPoint& mutable_remote_end_point() {
        return _remote_end_point;
    }
    void set_remote_end_point(const base::EndPoint& remote_end_point) {
        _remote_end_point = remote_end_point;
    }
    void set_remote_end_point(base::EndPoint& remote_end_point) {
        _remote_end_point = remote_end_point;
    }

    bool SendPacket();
    bool SendPacket(const std::string& data);
    bool SendPacket(std::unique_ptr<rocket::container::RingBuffer>& data);

    // These three methods will be called by the event poller thread when a
    // socket fd becomes readable or writable
    // NOTE: user should not care about them
    void HandleReadableEvent(EventDispatcher* event_disp);
    void HandleWriteableEvent(EventDispatcher* event_disp);
    void HandleCloseConnection();

    void MarkAsClosed(bool immediately = true);

 private:
    TcpConnection(std::shared_ptr<EventDispatcher> event_disp, int fd)
        : ConnectionBase(event_disp, fd)
        , _recv_buffer(0)
        , _status(0) {
    }

    base::EndPoint _remote_end_point;

    int           _status; // equal to errno
    std::string   _error_message;

    //concurrency::SpinLock                     _send_lock;
    std::list<std::unique_ptr<rocket::container::RingBuffer>>    _send_buffers;

    rocket::container::RingBuffer  _recv_buffer;
    size_t                         _receive_buffer_size;
    size_t                         _send_buffer_size;

    ClosedCallbackType             _closed_callback;
    SentCallbackType               _sent_callback;
    ReceivedCallbackType           _received_callback;
    std::shared_ptr<void>          _cookie;
};

}  // namespace tcp
}  // namespace rocket

#endif  // ROCKET_TCP_TCP_CONNECTION_H_