#ifndef HY_HEADER_H_
#define HY_HEADER_H_

#include <stdint.h>
#include <arpa/inet.h>

namespace hy_base {

const char HY_HEADER_SOH = 0x06;
const char HY_HEADER_EOT = 0x08;

#pragma pack(1)
struct HyHeader {
    uint8_t version;
    uint32_t cmd;
    uint16_t checksum;
    uint32_t seq;
    uint32_t header_ext_len; //可变长协议头的长度
    uint8_t response_flag;  // 暂时还没用到
    uint16_t error_code;   //返回码,与业务PB的返回码一致
    char reserved[1];   // 预留
    uint32_t len;       // 协议总长度
};
#pragma pack()
}//namespace
#endif

