#include "msg.h"
#include "debug.h"

using namespace google::protobuf;

Msg::Msg()
{
    memset(&_head, 0, sizeof(HyHeader));
    _msg = NULL;
}

Msg::~Msg()
{
    if (_msg)
    {
        //delete _msg;
        _msg = NULL;
    }
}

int Msg::encode(char* buf, size_t& buf_len)
{
    if (buf_len < sizeof(HyHeader) + 2)
    {
        DEBUGP("invalid buf_len: %zd", buf_len);
        return -1;
    }

    if (NULL == _msg)
    {
        DEBUGP("msg is null");
        return -1;
    }

    string data;
    if (!_msg->SerializeToString(&data))
    {
        DEBUGP("serialize to pb error");
        return -1;
    }

    unsigned packet_size = sizeof(HyHeader) + 2 + (unsigned)data.size();
    if (buf_len < packet_size)
    {
        DEBUGP("buf_len %zd less than packet_size %u", buf_len, packet_size);
        return -1;
    }

    buf_len = packet_size;

    _head.cmd            = htonl(_head.cmd);
    _head.checksum       = htons(_head.checksum);
    _head.seq            = htonl(_head.seq);
    _head.header_ext_len = htonl(_head.header_ext_len);
    _head.error_code     = htons(_head.error_code);
    _head.len            = htonl(packet_size);

    *buf = HY_HEADER_SOH;
    *(buf + buf_len - 1) = HY_HEADER_EOT;

    memcpy(buf + 1, &_head, sizeof(HyHeader));
    memcpy(buf + 1 + sizeof(HyHeader), data.data(), data.size());

    DEBUGP("encode buf:%s buf_len:%d", buf, buf_len);
    return 0;
}

int Msg::encode(char** buf, size_t& buf_len)
{
    if (NULL == buf)
    {
        DEBUGP("buf is null");
        return -1;
    }

    if (NULL == _msg)
    {
        DEBUGP("msg is null");
        return -1;
    }

    string data;
    if (!_msg->SerializeToString(&data))
    {
        DEBUGP("serialize to pb error");
        return -1;
    }

    buf_len = sizeof(HyHeader) + 2 + (unsigned)data.size();

    char *buffer = new char[buf_len];

    _head.cmd            = htonl(_head.cmd);
    _head.checksum       = htons(_head.checksum);
    _head.seq            = htonl(_head.seq);
    _head.header_ext_len = htonl(_head.header_ext_len);
    _head.error_code     = htons(_head.error_code);
    _head.len            = htonl(buf_len);

    *buffer = HY_HEADER_SOH;
    *(buffer + buf_len - 1) = HY_HEADER_EOT;

    memcpy(buffer + 1, &_head, sizeof(HyHeader));
    memcpy(buffer + 1 + sizeof(HyHeader), data.data(), data.size());

    *buf = buffer;
    DEBUGP("encode buf:%s buf_len:%d", buffer, buf_len);
    return 0;
}

int Msg::decode(const char* buf, size_t buf_len)
{
    if (buf_len < sizeof(HyHeader) + 2)
    {
        DEBUGP("invalid buf_len: %zd", buf_len);
        return -1;
    }

    if (*buf != HY_HEADER_SOH)
    {
        DEBUGP("invalid soh: 0x%x", *buf);
        return -1;
    }

    if (*(buf + buf_len - 1) != HY_HEADER_EOT)
    {
        DEBUGP("invalid eot: 0x%x", *(buf + buf_len - 1));
        return -1;
    }

    memcpy(&_head, buf + 1, sizeof(HyHeader));

    _head.cmd            = ntohl(_head.cmd);
    _head.checksum       = ntohs(_head.checksum);
    _head.seq            = ntohl(_head.seq);
    _head.header_ext_len = ntohl(_head.header_ext_len);
    _head.error_code     = ntohs(_head.error_code);
    _head.len            = ntohl(_head.len);

    if (_head.len != buf_len)
    {
        DEBUGP("head.len %u not equal buf_len %zd", _head.len, buf_len);
        return -1;
    }

    DEBUGP("_head.cmd:%d", _head.cmd);
    switch (_head.cmd)
    {
        case CMD_REQ_ECHO:
            _msg = new EchoReq();
            break;
        case CMD_RSP_ECHO:
            _msg = new EchoRsp();
            break;
        default:
            DEBUGP("invalid cmd: %u", _head.cmd);
            return -1;
    }

    int size = int(buf_len - 2 - sizeof(HyHeader));

    if (!_msg->ParseFromArray(buf + 1 + sizeof(HyHeader), size))
    {
        DEBUGP("parse from pb error");
        delete _msg;
        _msg = NULL;
        return -1;
    }

    //DEBUGP("%s: [%s]", _msg->GetDescriptor()->name().c_str(), _msg->ShortDebugString().c_str());

    return 0;
}

