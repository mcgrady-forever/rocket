#include "connection.h"
#include "socket_util.h"
#include "logger.h"


namespace rocket {

Connection::Connection(int fd, 
                       const std::string& ip, 
                       int port, 
                       unsigned flow)
	: fd_(fd)
	, ip_(ip)
	, port_(port)
	, flow_(flow)
{ }

Connection::~Connection() {
}

int Connection::Write(unsigned flow, const char* buf, size_t len) {
    LOG_DEBUG("Connection::Write flow=%u, len=%d", flow, len);
    int ret = -1;
    int write_size = 0;
    ret = BaseSocket::nbWrite(fd_, buf, len, &write_size);
    if (ret) {
    	LOG_DEBUG("Write nbWrite success");
    } else {
    	LOG_ERROR("Write nbWrite error");
    }
    return ret;
}	

}