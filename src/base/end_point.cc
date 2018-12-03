// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#include "base/end_point.h"

#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <string.h>

#include "base/string_utils.h"


namespace rocket {
namespace base {

bool EndPoint::ToSockAddr(struct sockaddr* address,
                          socklen_t* address_len) const {
    assert(address);
    assert(address_len);

    if (_port <= 0) {
        return false;
    }

    switch (_address.Family()) {
    case AF_INET: {
        //*address_len = IPAddress::IPV4_ADDRESS_SIZE;
        *address_len = sizeof(struct sockaddr_in);
        auto in_address = reinterpret_cast<struct sockaddr_in*>(address);
        in_address->sin_family = AF_INET;
        in_address->sin_port = htons(_port);
        ::memcpy(&(in_address->sin_addr),
                &((_address.address())[0]),
                IPAddress::IPV4_ADDRESS_SIZE);
        break;
    }
    case AF_INET6: {
        //*address_len = IPAddress::IPV6_ADDRESS_SIZE;
        *address_len = sizeof(struct sockaddr_in6);
        auto in6_address = reinterpret_cast<struct sockaddr_in6*>(address);
        in6_address->sin6_family = AF_INET6;
        in6_address->sin6_port = htons(_port);
        memcpy(&(in6_address->sin6_addr),
                &((_address.address())[0]),
                IPAddress::IPV6_ADDRESS_SIZE);
        break;
    }
    default:
        return false;
    }
    return true;
}

bool EndPoint::FromSockAddr(const struct sockaddr& address,
                            socklen_t address_len) {
    switch (address_len) {
        case sizeof(struct sockaddr_in): {
            if (address_len < static_cast<socklen_t>(sizeof(struct sockaddr_in))) {
                goto error;
            }
            auto in_address = reinterpret_cast<const struct sockaddr_in*>(&address);
            if (in_address->sin_family != AF_INET) {
                goto error;
            }
            _port = ntohs(in_address->sin_port);
            _address.mutable_address().resize(IPAddress::IPV4_ADDRESS_SIZE);
            memcpy(&((_address.mutable_address())[0]),
                   &(in_address->sin_addr),
                   IPAddress::IPV4_ADDRESS_SIZE);
            break;
        }
        case sizeof(struct sockaddr_in6): {
            if (address_len < static_cast<socklen_t>(sizeof(struct sockaddr_in6))) {
                goto error;
            }
            auto in6_address = reinterpret_cast<const struct sockaddr_in6*>(&address);
            if (in6_address->sin6_family != AF_INET6) {
                goto error;
            }
            _port = ntohs(in6_address->sin6_port);
            _address.mutable_address().resize(IPAddress::IPV6_ADDRESS_SIZE);
            memcpy(&((_address.mutable_address())[0]),
                   &(in6_address->sin6_addr),
                   IPAddress::IPV6_ADDRESS_SIZE);
            break;
        }
        default:
            goto error;
    }
    return true;

error:
    _address.mutable_address().clear();
    _port = 0;
    return false;
}

}  // namespace base
}  // namespace rocket