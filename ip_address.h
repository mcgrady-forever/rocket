// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#ifndef NET_TCP_IP_ADDRESS_H_
#define NET_TCP_IP_ADDRESS_H_

#include <stdint.h>
#include <cstdlib>
#include <limits>
#include <cassert>

#include <vector>
#include <string>

namespace rocket {

class IPAddress {
public:
    static const size_t IPV4_ADDRESS_SIZE = 4; // 4 bytes
    static const size_t IPV6_ADDRESS_SIZE = 16; // 16 bytes

    IPAddress() {};
    ~IPAddress() {};
    explicit IPAddress(const std::string& str_addr);

    // convert IPv4 address like '192.168.1.1', or
    // IPv6 address like 
    // '2001:0DB8:02de:0000:0000:0000:0000:0e13', 
    // or '2001:DB8:2de:0000:0000:0000:0000:e13', 
    // or '2001:0DB8:02de::0e13', 
    // or '::e13:2001', 
    // or 'abcd:0001::', 
    // or '::ffff:192.168.1.1' 
    // to number, in network byte order
    // return false if the str_addr is invalid
    static bool LiteralToNumber(const std::string& str_addr, IPAddress* number);

    // convert IPv4 or IPv6 address with binary format to string format
    static bool NumberToLiteral(const IPAddress& number, std::string* str_addr);

    int Family() const;

    std::vector<uint8_t>& mutable_address() {
        return address_;
    }

    const std::vector<uint8_t>& address() const {
        return address_;
    }

    size_t Size() const {
        return address_.size();
    }

    std::string ToString() const;

    void Swap(IPAddress& ip_address) {
        std::swap(address_, ip_address.address_);
    }

private:
    static bool IPv4LiteralToNumber(const std::string& str_addr,
                                    IPAddress* number);
    /*
    static bool IPv6LiteralToNumber(const std::string& str_addr,
                                    IPAddress* number);
    */
    static bool IPv4NumberToLiteral(const IPAddress& number,
                                    std::string* str_addr);
    /*
    static bool IPv6NumberToLiteral(const IPAddress& number,
                                    std::string* str_addr);
    */
    std::vector<uint8_t> address_;
};

class IPAddressList {
public:
    std::vector<IPAddress> addresses_;
};

}  // namespace chris


#endif  // NET_TCP_IP_ADDRESS_H_