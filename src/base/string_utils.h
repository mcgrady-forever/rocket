// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#ifndef ROCKET_BASE_STRING_UTILS_H_
#define ROCKET_BASE_STRING_UTILS_H_

#include <string>
#include <vector>

namespace rocket {
namespace base {

class StringUtils {
public:
    // split the source string by the charactors
    // this method scans the str, once it finds the charactor which it exsits
    // in the separators, it will generate a new substring,
    // and put it into a container, finally returns the container
    static std::vector<std::string> SplitByChars(const std::string& str,
                                                 const std::string& separators);
};

} // namespace base
} // namespace rocket

#endif // ROCKET_BASE_STRING_UTILS_H_