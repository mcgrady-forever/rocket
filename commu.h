#ifndef _COMMU_H_
#define _COMMU_H_

namespace thunder {

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

    // 向客户端发送响应数据
    virtual int write(char* buf, size_t len) = 0;
};

}  // namespace thunder

#endif
