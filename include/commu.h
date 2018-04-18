#ifndef _TCOMMU_H_
#define _TCOMMU_H_

namespace rocket {

// 数据blob
struct blob_type
{
    unsigned len;   // 数据长度
    void*    owner; // 组件指针
    char*    data;  // 数据缓冲区
};

const unsigned BLOB_HEAD_SIZE = 12;

// 通讯类抽象接口
class Commu
{
public:
    Commu() {}
    virtual ~Commu() {}

    // 获取客户端ip
    virtual std::string get_ip() = 0;

    // 获取客户端port
    virtual int get_port() = 0;

    // get fd
    virtual int get_fd() = 0;

    // 向客户端发送响应数据
    virtual int Write(unsigned flow, const char* buf, size_t len) = 0;
};

}  // namespace ff

#endif
