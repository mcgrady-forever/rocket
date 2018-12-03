#include <iostream>
#include <string>

#include "sock.h"
#include "hy_header.h"
#include "msg.h"
#include "debug.h"

using namespace std;

#define BUF_SIZE (1024 * 1024)

string host = "127.0.0.1";
short port = 5555;
TcpCltSocket client;

int ip_s2u(const string& sip, unsigned& uip)
{
    struct in_addr addr;

    int ret = inet_pton(AF_INET, sip.c_str(), &addr);
    if (ret <= 0)
    {
        return -1;
    }

    uip = (unsigned)addr.s_addr;

    return 0;
}

int recv_msg(char *buf, unsigned buf_len)
{
    int ret;
    unsigned recv = 0;

    // 读取hy_header
    while (recv < 1 + sizeof(hy_base::HyHeader))
    {
        ret = client.TcpRead(buf + recv, buf_len);
        if (ret < 0)
        {
            DEBUGP("recv msg error, ret: %d", ret);
            return -1;
        }
        recv += ret;
    }

    hy_base::HyHeader* head = (hy_base::HyHeader *)(buf + 1);

    // 得到消息整体长度
    unsigned len = ntohl(head->len);

    //DEBUGP("msg length: %d", len);

    // 继续读取消息剩余部分
    while (recv < len)
    {
        ret = client.TcpRead(buf + recv, buf_len);
        if (ret < 0)
        {
            DEBUGP("recv error: %s", client.getErrMsg());
            return -1;
        }
        recv += ret;
    }

    return recv;
}

int send_and_recv_msg(Msg& out_msg, Msg& in_msg)
{
    char buf[BUF_SIZE];
    size_t buf_len = BUF_SIZE;

    int ret = out_msg.encode(buf, buf_len);
    if (ret != 0)
    {
        DEBUGP("encode msg error");
        return -1;
    }

    ret = client.TcpWrite(buf, buf_len);

    //ret = client.TcpWrite(buf, 10);
    //usleep(1000);
    //ret = client.TcpWrite(buf + 10, buf_len - 10);

    //memcpy(buf + buf_len, buf, buf_len);
    //ret = client.TcpWrite(buf, buf_len * 2);

    //memcpy(buf + buf_len, buf, buf_len);
    //ret = client.TcpWrite(buf, buf_len + 10);
    //ret = client.TcpWrite(buf + buf_len + 10, buf_len - 10);

    if (ret < 0)
    {
        DEBUGP("send msg error");
        return -1;
    }

    ret = recv_msg(buf, buf_len);
    if (ret <= 0)
    {
        DEBUGP("recv msg error");
        return -1;
    }

    ret = in_msg.decode(buf, ret);
    if (ret != 0)
    {
        DEBUGP("decode msg error: %d", ret);
        return -1;
    }

    return 0;
}

int echo()
{
    EchoReq* req = new EchoReq();
    req->set_echo("haha");

    Msg out_msg;
    out_msg._head.cmd = CMD_REQ_ECHO;
    out_msg._msg = req;


    Msg in_msg;
    int ret = send_and_recv_msg(out_msg, in_msg);
    if (ret)
    {
        return -1;
    }

    EchoRsp* rsp = (EchoRsp*)in_msg._msg;

    DEBUGP("%s: [%s]", rsp->GetDescriptor()->name().c_str(), rsp->ShortDebugString().c_str());

    return 0;
}

int main(int argc, char **argv)
{
    if (argc == 3) {
        host = argv[1];
        port = atoi(argv[2]);
    }

    unsigned uip;
    ip_s2u(host, uip);

    /*
    if (!client.ConnectServer2(uip, port))
    {
        DEBUGP("connect to %s:%u error: %s", host.c_str(), port, client.getErrMsg());
        return -1;
    }
    */

    //connect with timeout
    if (!client.ConnectServer2(uip, port, 1000000))
    {
        DEBUGP("connect to %s:%u error: %s", host.c_str(), port, client.getErrMsg());
        return -1;
    }

    echo();
}
