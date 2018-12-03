// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>
#include "base/ip_address.h"

#include <sys/socket.h>
#include <cstdlib>
#include <string>

#include "base/string_utils.h"


namespace rocket {
namespace base {

// const size_t IPAddress::IPV4_ADDRESS_SIZE = 4;
// const size_t IPAddress::IPV6_ADDRESS_SIZE = 16;

IPAddress::IPAddress(const std::string& str_addr) {
    if (!LiteralToNumber(str_addr, this)) {
        _address.clear();
    }
}

bool IPAddress::LiteralToNumber(const std::string& str_addr, IPAddress* number) {
    assert(number);

    if (str_addr.find(":") == std::string::npos) {
        return IPv4LiteralToNumber(str_addr, number);
    } else {
        //return IPv6LiteralToNumber(str_addr, number);
        return false;
    }
}

bool IPAddress::NumberToLiteral(const IPAddress& number,
                                std::string* str_addr) {
    assert(str_addr);

    switch (number.Family()) {
    case AF_INET:
        return IPv4NumberToLiteral(number, str_addr);
    case AF_INET6:
        //return IPv6NumberToLiteral(number, str_addr);
        return false;
    default:
        return false;
    }
}

bool IPAddress::IPv4LiteralToNumber(const std::string& str_addr, IPAddress* number) {
    assert(number);

    std::vector<uint8_t>().swap(number->_address);

    if (str_addr.empty()) {
        return false;
    }

    auto pieces = StringUtils::SplitByChars(str_addr, ".");
    if (pieces.size() != 4) {
        return false;
    }
    for (auto& p : pieces) {
        if (p.size() > 3) {
            return false;
        }
        char* pos = nullptr;
        auto int_p = static_cast<uint16_t>(::strtoull(p.c_str(), &pos, 10));
        if (pos == p.data() || int_p > 255) {
            return false;
        }
        (number->_address).push_back(int_p);
    }
    return true;
}

bool IPAddress::IPv4NumberToLiteral(const IPAddress& number,
                                    std::string* str_addr) {
    assert(str_addr);

    if (number.Size() != IPV4_ADDRESS_SIZE) {
        return false;
    }

    for (size_t i = 0; i < IPV4_ADDRESS_SIZE; ++i) {
        str_addr->append(std::to_string(static_cast<unsigned>(number._address[i])));
        if (i != IPV4_ADDRESS_SIZE - 1) {
            str_addr->append(1, '.');
        }
    }
    return true;
}

int IPAddress::Family() const {
    switch (_address.size()) {
        case IPV4_ADDRESS_SIZE:
            return AF_INET;
        case IPV6_ADDRESS_SIZE:
            return AF_INET6;
        default:
            return AF_UNSPEC;
    }
}

std::string IPAddress::ToString() const {
    std::string str_addr;
    if (!NumberToLiteral(*this, &str_addr)) {
        return "";
    }
    return str_addr;
}

}  // namespace base
}  // namespace rocket