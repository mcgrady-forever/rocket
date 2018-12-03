// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#include "string_utils.h"

namespace rocket {
namespace base {

std::vector<std::string> StringUtils::SplitByChars(const std::string &str, const std::string &separators) {
    std::vector<std::string> result;
    if (str.empty() || separators.empty()) {
        return result;
    }

    std::string::size_type i = 0;
    while (i != str.size()) {
        //找到字符串中首个不等于分隔符的字母；
        int flag = 0;
        while (i != str.size() && flag == 0) {
            flag = 1;
            for (std::string::size_type x = 0; x < separators.size(); ++x) {
                if (str[i] == separators[x]) {
                    ++i;
                    flag = 0;
                    break;
                }
            }
        }

        //找到又一个分隔符，将两个分隔符之间的字符串取出；
        flag = 0;
        std::string::size_type j = i;
        while (j != str.size() && flag == 0) {
            for (std::string::size_type x = 0; x < separators.size(); ++x) {
                if (str[j] == separators[x]) {
                    flag = 1;
                    break;
                }
            }
            if (flag == 0)
                ++j;
        }
        if (i != j) {
            result.push_back(str.substr(i, j - i));
            i = j;
        }
    }
    return result;
}

} // namespace base
} // namespace rocket