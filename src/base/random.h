// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#ifndef ROCKET_BASE_RANDOM_H_
#define ROCKET_BASE_RANDOM_H_

#include <stdint.h>

#include "base/macro.h"

namespace rocket {
namespace base {

// ACM minimal standard random number generator.  (re-entrant.)
class Random {
    ROCKET_DECLARE_UNCOPYABLE(Random);

public:
    explicit Random(int32_t seed) : _seed(seed) {}

    int32_t Next();
    int32_t Uniform(int32_t n);

    void Reset(int32_t seed) {
        _seed = seed;
    }

    // Randomly returns true ~"1/n" of the time, and false otherwise.
    // REQUIRES: n > 0
    bool OneIn(int n) { return (Next() % n) == 0; }

    // Skewed: pick "base" uniformly from range [0,max_log] and then
    // return "base" random bits.  The effect is to pick a number in the
    // range [0,2^max_log-1] with exponential bias towards smaller numbers.
    uint32_t Skewed(int max_log) {
        return Uniform(1 << Uniform(max_log + 1));
    }

private:
    int32_t _seed;
};

}  // namespace base
}  // namespace rocket

#endif  // ROCKET_BASE_RANDOM_H_
