// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#include "random.h"

namespace rocket {
namespace base {

int32_t Random::Next() {
    const int32_t M = 2147483647L;   // 2^31-1
    const int32_t A = 16807;
    // In effect, we are computing seed_ = (seed_ * A) % M, where M = 2^31-1
    uint32_t lo = A * static_cast<int32_t>(_seed & 0xFFFF);
    uint32_t hi = A * static_cast<uint32_t>(static_cast<uint32_t>(_seed) >> 16);
    lo += (hi & 0x7FFF) << 16;
    if (lo > static_cast<uint32_t>(M)) {
        lo &= M;
        ++lo;
    }
    lo += hi >> 15;
    if (lo > static_cast<uint32_t>(M)) {
        lo &= M;
        ++lo;
    }
    return (_seed = static_cast<int32_t>(lo));
}

int32_t Random::Uniform(int32_t n) {
    return Next() % n;
}

}  // namespace base
}  // namespace rocket