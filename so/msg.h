#ifndef _MSG_H_
#define _MSG_H_

#include <string>
#include <google/protobuf/message.h>

#include "hy_header.h"
#include "echo.pb.h"

using namespace std;
using namespace hy_base;
using namespace google::protobuf;

enum
{
    CMD_REQ_ECHO = 100,
    CMD_RSP_ECHO = 101,
};

class Msg
{
public:
    Msg();
    virtual ~Msg();

    int encode(char* buf, size_t& buf_len);

    // 内部分配buf，需调用者通过delete[]释放
    int encode(char** buf, size_t& buf_len);

    int decode(const char* buf, size_t buf_len);

public:
    HyHeader _head;
    Message* _msg;
};

#endif
