#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>

#include <event.h>

#include "msg.h"
#include "debug.h"

#define BUF_SIZE 4096

static const char* host = "127.0.0.1";
static int port = 5555;
static int nclients = 1;

class Client
{
public:
    Client() : _fd(-1), _event(NULL)
    {
    }

    ~Client()
    {
        if (_fd > 0)
        {
            close(_fd);
        }

        if (_event)
        {
            event_free(_event);
            _event = NULL;
        }
    }

    int Init(struct sockaddr_in* addr, struct event_base* base)
    {
        int ret;

        ret = socket(AF_INET, SOCK_STREAM, 0);
        if (ret == -1)
        {
            DEBUGP("socket error, %d, %s", errno, strerror(errno));
            return -1;
        }

        _fd = ret;

        ret = connect(_fd, (struct sockaddr *)addr, sizeof(struct sockaddr));
        if (ret == -1)
        {
            DEBUGP("connect error, %d, %s", errno, strerror(errno));
            return -1;
        }

        ret = evutil_make_socket_nonblocking(_fd);
        if (ret == -1)
        {
            DEBUGP("fcntl error, %d, %s", errno, strerror(errno));
            return -1;
        }

        _event = event_new(base, _fd, EV_READ|EV_PERSIST, readcb, this);
        if (_event == NULL)
        {
            DEBUGP("event_new failed");
            return -1;

        }

        ret = event_add(_event, NULL);
        if (ret == -1)
        {
            DEBUGP("event_add failed");
            return -1;
        }

        return 0;
    }

    ssize_t Send()
    {
        char buf[BUF_SIZE];

        EchoReq* req = new EchoReq();
        req->set_echo("hello");

        Msg msg;
        msg._head.cmd = CMD_REQ_ECHO;
        msg._msg = req;

        size_t len = BUF_SIZE;

        if (msg.encode(buf, len))
        {
            DEBUGP("msg encode failed");
            return -1;
        }

        ssize_t ret = write(_fd, buf, len);
        if (ret == -1)
        {
            DEBUGP("write error: %d, %s", errno, strerror(errno));
            return -1;
        }
        else
        {
            DEBUGP("write success: %d, %d", _fd, ret);
        }

        return 0;
    }

    static void readcb(int fd, short eventis, void* arg)
    {
        DEBUGP("readcb");
        Client* client = (Client *)arg;
        char buf[BUF_SIZE];
        time_t cur;
        static time_t last = time(NULL);
        static int count = 0;

        ssize_t ret = read(fd, buf, BUF_SIZE);

        if (ret == -1)
        {
            close(fd);
            DEBUGP("read error: %d, %s", errno, strerror(errno));
            return;
        } 
        else if (0 == ret)
        {
            close(fd);
            DEBUGP("read end: %d, %s", errno, strerror(errno));
            return;
        }

        Msg msg;

        if (msg.decode(buf, ret))
        {
            DEBUGP("decode msg error");
            close(fd);
            return;
        }
        else
        {
            DEBUGP("rsp:%s", ((EchoRsp*)msg._msg)->echo().c_str());
        }

        count++;
        cur = time(NULL);

        if (cur > last)
        {
            printf("%d\n", count);
            count = 0;
            last = cur;
        }

        //client->Send();
    }

private:
    int _fd;
    struct event* _event;
};

int main(int argc, char **argv)
{
    if (argc > 3)
    {
        host = argv[1];
        port = atoi(argv[2]);
        nclients = atoi(argv[3]);
    }

    struct sockaddr_in addr;

    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(host);
    addr.sin_port = htons(port);

    struct event_base *base = event_base_new();

    if (base == NULL)
    {
        DEBUGP("Could not initialize libevent!");
        return 1;
    }

    Client* client = new Client[nclients];

    for (int i = 0; i < nclients; i++)
    {
        client[i].Init(&addr, base);
        usleep(10);
        client[i].Send();
    }

    event_base_dispatch(base);
    event_base_free(base);

    delete[] client;

    return 0;
}
