#include <stdlib.h>
#include <errno.h>
#include <string>
#include <iostream>

#include "hy_header.h"
#include "commu.h"
#include "msg.h"
#include "fileconfig.h"

using namespace hy_base;
using namespace std;
using namespace rocket;

//////////////////// app 自启线程实现定时器示例 ////////////////////

void timer_proc(int id, short events, void* arg)
{
    //DEBUG("timer_proc");
}

/*
void* thread_fun(void* arg)
{
    // app 线程设定名称，最长15个字符
    pthread_setname_np(pthread_self(), "ff_app");

    struct event_base* base = event_base_new();
    int flag = 0; // 0 表示只执行一次，EV_PERSIST表示永久

    // 新建定时器事件
    struct event* ev = event_new(base, -1, flag, timer_proc, NULL);
    if (!ev)
    {
        DEBUG("Could not create a timer event!");
        return NULL;
    }

    timeval tv = { 5, 0 };

    if (event_add(ev, &tv) < 0)
    {
        DEBUG("Could not add a timer event!");
        event_free(ev);
        return NULL;
    }

    event_base_dispatch(base);
    event_base_free(base);

    return NULL;
}
*/

//////////////////// app 自启线程实现定时器示例 ////////////////////

/**
 * app 初始化
 *
 * @param arg app 配置文件路径
 *
 * @return 0 成功, 非 0 失败
 */
extern "C" int rocket_handle_init(void* arg)
{
    // app 自身的配置文件
    const char* etc = (const char*) arg;

    // 读 app 配置
    hy_base::FileConfig fconf;
    if (fconf.ParseFile(etc))
    {
        fprintf(stderr, "parse app config file \"%s\" error: %s\n", etc, strerror(errno));
        return -1;
    }

    // 读 ring log 配置
    string path = fconf["log.path"];
    string program = fconf["log.program"];
    int level = atoi(fconf["log.level"].c_str());

    cout << "log.path=" << path << endl
         << "log.program=" << path << endl
         << endl;

    return 0;
}

int complete(const char* data, size_t len)
{
    if (data == NULL)
    {
        return -1;
    }

    if (data[0] != HY_HEADER_SOH)
    {
        return -2;
    }

    if (len < 1 + sizeof(hy_base::HyHeader))
    {
        return 0;
    }

    HyHeader *header = (HyHeader*) (data + 1);

    uint32_t pack_len = ntohl(header->len);

    if (pack_len < 2 + sizeof(hy_base::HyHeader))
    {
        return -3;
    }

    if (pack_len > len)
    {
        return 0;
    }

    if (data[pack_len - 1] != HY_HEADER_EOT)
    {
        return -4;
    }

    return (int)pack_len;
}

/**
 * 输入数据完整性检查
 *
 * @param arg 输入数据, blob 类型
 *
 * @return < 0 表示非法, 0 表示不完整, > 0 表示完整数据包长度
 */
extern "C" int rocket_handle_input(void* arg)
{
    blob_type* blob = (blob_type*) arg;
    return complete(blob->data, blob->len);
}

/**
 * 数据包处理
 *
 * @param arg 数据包, blob类型
 *
 * @return 0 成功， 非 0 失败
 */
extern "C" int rocket_handle_process(void* arg)
{
    std::cout << "rocket_handle_process" << std::endl;
    blob_type* blob = (blob_type*) arg;
    Commu* c = (Commu*) blob->owner; // 通讯组件, 目前支持获取ip和port, 及发送回应

    Msg in_msg;

    int ret = in_msg.decode(blob->data, blob->len);
    if (ret)
    {
        //DEBUG("decode request msg error, ret: %d", ret);
        return -1;
    }

    //DEBUG("recv req from %s:%d, cmd: %d",
    //      c->GetIp().c_str(), c->GetPort(), in_msg._head.cmd);

    switch (in_msg._head.cmd)
    {
        case CMD_REQ_ECHO:
        {
            EchoReq* req = (EchoReq*)in_msg._msg;
            EchoRsp* rsp = new EchoRsp;
            rsp->set_echo(req->echo());

            Msg out_msg;
            out_msg._head = in_msg._head;
            out_msg._head.cmd = CMD_RSP_ECHO;
            out_msg._msg = rsp;

            char *buf = NULL;
            size_t len = 0;

            ret = out_msg.encode(&buf, len);
            if (ret)
            {
                //DEBUG("encode response msg error, ret: %d", ret);
                return -1;
            }

            // 写回应
            c->Write(buf, len);

            delete[] buf;

            break;
        }
        default:
            //DEBUG("invalid cmd: %u", in_msg._head.cmd);
            break;
    }

    return 0;
}

extern "C" void rocket_handle_fini(void* arg)
{
}

extern "C" int rocket_handle_route(unsigned flow, void* arg1, void* arg2)
{
    return 1;
}
