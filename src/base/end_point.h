// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#ifndef ROCKET_BASE_END_POINT_H_
#define ROCKET_BASE_END_POINT_H_

#include <sys/socket.h>

#include <string>

#include "base/ip_address.h"


namespace rocket {
namespace base {

class EndPoint {
public:
    EndPoint() {};
    ~EndPoint() {};

    EndPoint(const IPAddress& address, int port)
        : _address(address)
        , _port(port) {
    }

    EndPoint(const std::string& address, int port)
        : _address(address)
        , _port(port) {
    }

    EndPoint(const struct sockaddr& address, socklen_t address_length) {
        bool res = FromSockAddr(address, address_length);
        assert(res);
    }

    int family() const {
        return _address.Family();
    }

    int port() const {
        return _port;
    }

    IPAddress& mutable_address() {
        return _address;
    }

    const IPAddress& address() const {
        return _address;
    }

    bool ToSockAddr(struct sockaddr* address,
                    socklen_t* address_len) const;

    bool FromSockAddr(const struct sockaddr& address,
                      socklen_t address_len);

    std::string ToString() const {
        std::string res;
        res.reserve(64);
        res.append(_address.ToString());
        res.append(1, ':');
        res.append(std::to_string(static_cast<unsigned>(_port)));
        return res;
    }

    std::string ToStringWithoutPort() const {
        return _address.ToString();
    }

private:
    IPAddress   _address;
    int         _port;
};

} // namespace base
} // namespace rocket

#endif // ROCKET_BASE_END_POINT_H_