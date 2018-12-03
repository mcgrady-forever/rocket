// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#ifndef ROCKET_TCP_END_POINT_H_
#define ROCKET_TCP_END_POINT_H_

#include <sys/socket.h>

#include <string>

#include "tcp/ip_address.h"


namespace rocket {
namespace tcp {

class EndPoint {
public:
    EndPoint() {};
    ~EndPoint() {};

    EndPoint(const IPAddress& address, int port)
        : address_(address)
        , port_(port) {
    }

    EndPoint(const std::string& address, int port)
        : address_(address)
        , port_(port) {
    }

    EndPoint(const struct sockaddr& address, socklen_t address_length) {
        bool res = FromSockAddr(address, address_length);
        assert(res);
    }

    int Family() const {
        return address_.Family();
    }

    int port() const {
        return port_;
    }

    IPAddress& mutable_address() {
        return address_;
    }

    const IPAddress& address() const {
        return address_;
    }

    bool ToSockAddr(struct sockaddr* address,
                    socklen_t* address_len) const;

    bool FromSockAddr(const struct sockaddr& address,
                    socklen_t address_len);

    std::string ToString() const {
        std::string res;
        res.reserve(64);
        res.append(address_.ToString());
        res.append(1, ':');
        res.append(std::to_string(static_cast<unsigned>(port_)));
        return res;
    }

    std::string ToStringWithoutPort() const {
        return address_.ToString();
    }

private:
    IPAddress   address_;
    int         port_;
};

} // namespace tcp
} // namespace rocket

#endif // ROCKET_TCP_END_POINT_H_