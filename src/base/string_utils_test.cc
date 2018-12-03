#include <iostream>
#include <vector>

#include "string_utils.h"

using namespace std;
using namespace rocket::base;

void test1() {
    string str = "123:456:789";
    vector<string> res;
    res = StringUtils::SplitByChars(str, ":");
    for (auto i : res) {
        cout << i << endl;
    }
}

void test2() {
    string str1 = "123456789";
    vector<string> res;
    res = StringUtils::SplitByChars(str1, ":");
    for (auto i : res) {
        cout << i << endl;
    }
}

int main() {
    test1();
    test2();
}