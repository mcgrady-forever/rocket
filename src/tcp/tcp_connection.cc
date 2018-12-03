#include "tcp/tcp_connection.h"

#include <assert.h>

#include <memory>

#include "tcp/command.h"
#include "tcp/event_dispatcher.h"
#include "base/socket.h"


namespace rocket {
namespace tcp {

bool TcpConnection::SendPacket() {
    Command command(static_cast<int>(Command::Type::READABLE) |
                    static_cast<int>(Command::Type::WRITEABLE),
                    shared_from_this());
    std::shared_ptr<EventDispatcher> event_disp = _event_disp.lock();
    if (!event_disp.get()) {
        return false;
    }
    event_disp->AddCommand(command,
        _ep_thread_id != std::this_thread::get_id());
    return true;
}

bool TcpConnection::SendPacket(const std::string& data) {
    /*
    auto send_buffer = std::make_unique<RingBuffer>(data.size());
    bool r = send_buffer->Write(data);
    assert(r);
    return SendPacket(std::move(send_buffer));
    */
    return false;
}

bool TcpConnection::SendPacket(std::unique_ptr<rocket::container::RingBuffer>& data) {
    /*
    {
        concurrency::SpinLock::ScopeGuard guard(send_lock_);
        send_buffers_.emplace_back(std::move(data));
    }
    return SendPacket();
    */
    return false;
}

// This method will be called when a socket fd becomes readable
void TcpConnection::HandleReadableEvent(EventDispatcher* event_disp) {
    bool closed = false;

    if (_state == State::CONNECTING) {
        int error = 0;
        socklen_t error_length = sizeof(error);
        if (!_socket.GetOption(SOL_SOCKET, SO_ERROR, &error, &error_length) 
            || (error && error != EINPROGRESS)) {
            closed = true;
        } else if (error == EINPROGRESS) {
            return;
        } else {
            _state = State::CONNECTED;
            if (_connected_callback) {
                // call callback user defined
                _connected_callback(std::static_pointer_cast<TcpConnection>(shared_from_this()));
            }
        }
    }

    if (_state == State::CONNECTED) {
        // handle new arrival data
        // while (true) {
            
        //     if (_recv_buffer.capacity() - _recv_buffer.size() < 512) {
        //         //_recv_buffer.Resize(_recv_buffer.capacity() + 4096);
        //     }
        //     struct iovec buffers[2];
        //     _recv_buffer.GetWritePositions(buffers, 2);
        //     size_t received_length = 0;
        //     bool ret = _socket.Receive(buffers, 2, &received_length, true);
        //     _status = rocket::concurrency::ThisThread::GetLastError();
        //     //error_message_ = cnetpp::concurrency::ThisThread::GetLastErrorString();
        //     if (!ret && (_status == EAGAIN || _status == EWOULDBLOCK)) {
        //         break;
        //     } else if (!ret || received_length == 0) {
        //         closed = true;
        //         break;
        //     } else {
        //         // really received data
        //         _recv_buffer.CommitWrite(received_length);
        //         if (_received_callback) {
        //             if (!_received_callback(
        //                 std::static_pointer_cast<TcpConnection>(shared_from_this()))) {
        //                 closed = true;
        //                 break;
        //             }
        //         }
        //     }
        // }
    }

    bool tmp = false;
    if (closed && _state != State::CLOSED) {
        // remove this connection from event center
        Command command(static_cast<int>(Command::Type::REMOVE_CONN_IMMEDIATELY),
                        shared_from_this());
        event_disp->AddCommand(command, false/* only ep thread could be here */);
    }
}

void TcpConnection::HandleWriteableEvent(EventDispatcher* event_disp) {
    //_send_lock.Lock();
    if (_send_buffers.empty()) {
        //_send_lock.Unlock();
        if (_state == State::CONNECTED) {
            Command command(static_cast<int>(Command::Type::READABLE),
                shared_from_this());
            event_disp->AddCommand(command, false);
        } else if (_state == State::CLOSING) {
            Command command(static_cast<int>(Command::Type::REMOVE_CONN_IMMEDIATELY),
                shared_from_this());
            event_disp->AddCommand(command, false);
        }
        // do nothing
        return;
    }
    //_send_lock.Unlock();

    bool closed = false;
    if (_state == State::CONNECTING) {
        int error = 0;
        socklen_t error_length = sizeof(error);
        if (!_socket.GetOption(SOL_SOCKET, SO_ERROR, &error, &error_length) 
            || (error && error != EINPROGRESS)) {
            closed = true;
        } else if (error == EINPROGRESS) {
            return;
        } else {
            _state = State::CONNECTED;
            if (_connected_callback) {
                // call callback user defined
                _connected_callback(std::static_pointer_cast<TcpConnection>(shared_from_this()));
            }
        }
    }

    if (_state == State::CONNECTED || _state == State::CLOSING) {
        /*
        while (true) {
            _send_lock.Lock();
            auto& send_buffer = _send_buffers_.front();
            send_lock_.Unlock();
            size_t sent_length = 0;
            struct iovec buffers[2];
            send_buffer->GetReadPositions(buffers, 2);
            bool ret = socket_.Send(buffers, 2, &sent_length, true);
            status_ = cnetpp::concurrency::ThisThread::GetLastError();
            //error_message_ = cnetpp::concurrency::ThisThread::GetLastErrorString();
            if (!ret && status_ == EAGAIN) {
                return;
            } else if (!ret) {
                closed = true;
                break;
            } else {
                if (sent_length > 0) {
                if (sent_length != send_buffer->Size()) {
                    send_buffer->CommitRead(sent_length);
                    int type = static_cast<int>(Command::Type::kReadable) |
                    static_cast<int>(Command::Type::kWriteable);
                    event_center->AddCommand(Command(type, shared_from_this()), false);
                    return;
                } else {
                    bool all_sent = false;
                    send_lock_.Lock();
                    send_buffers_.pop_front();
                    if (send_buffers_.empty()) {
                    all_sent = true;
                    }
                    send_lock_.Unlock();
                    if (all_sent && state_ != State::kClosing) {
                    int type = static_cast<int>(Command::Type::kReadable);
                    event_center->AddCommand(Command(type, shared_from_this()),
                        false);
                    }
                    if (sent_callback_) {
                    sent_callback_(true,
                        std::static_pointer_cast<TcpConnection>(shared_from_this()));
                    }
                    if (all_sent) {
                    if (state_ == State::kClosing) {
                        closed = true;
                    }
                    break;
                    }
                }
                }
            }
        }
        */
    }

    if (closed && _state != State::CLOSED) {
        Command command(static_cast<int>(Command::Type::REMOVE_CONN_IMMEDIATELY),
                        shared_from_this());
        event_disp->AddCommand(command, false);
    }
}

void TcpConnection::HandleCloseConnection() {
    if (_state == State::CLOSED) {
        return;
    }
    _state = State::CLOSED;
    if (_closed_callback) {
        _closed_callback(std::static_pointer_cast<TcpConnection>(shared_from_this()));
    }
    _socket.Close();
}

void TcpConnection::MarkAsClosed(bool immediately) {
    int type = 0;
    if (immediately) {
        type = static_cast<int>(Command::Type::REMOVE_CONN_IMMEDIATELY);
    } else {
        type = static_cast<int>(Command::Type::REMOVE_CONN);
    }
    Command command(type, shared_from_this());
    auto event_disp = _event_disp.lock();
    if (event_disp.get()) {
        event_disp->AddCommand(command, _ep_thread_id != std::this_thread::get_id());
    }
}

}  // namespace tcp
}  // namespace rocket