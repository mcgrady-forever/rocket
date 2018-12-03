// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#include "tcp/end_point.h"

#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <string.h>

#include "base/string_utils.h"


namespace rocket {
namespace tcp {

bool EndPoint::ToSockAddr(struct sockaddr* address,
                          socklen_t* address_len) const {
    assert(address);
    assert(address_len);

    if (port_ <= 0) {
        return false;
    }

    switch (address_.Family()) {
    case AF_INET: {
        //*address_len = IPAddress::IPV4_ADDRESS_SIZE;
        *address_len = sizeof(struct sockaddr_in);
        auto in_address = reinterpret_cast<struct sockaddr_in*>(address);
        in_address->sin_family = AF_INET;
        in_address->sin_port = htons(port_);
        ::memcpy(&(in_address->sin_addr),
                &((address_.address())[0]),
                IPAddress::IPV4_ADDRESS_SIZE);
        break;
    }
    case AF_INET6: {
        //*address_len = IPAddress::IPV6_ADDRESS_SIZE;
        *address_len = sizeof(struct sockaddr_in6);
        auto in6_address = reinterpret_cast<struct sockaddr_in6*>(address);
        in6_address->sin6_family = AF_INET6;
        in6_address->sin6_port = htons(port_);
        memcpy(&(in6_address->sin6_addr),
                &((address_.address())[0]),
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
    return false;
}

}  // namespace tcp
}  // namespace rocket