#ifndef __TCP_WRAPPER_H__
#define __TCP_WRAPPER_H__

#include <netinet/in.h>
#include <pthread.h>
#include <string>
#include "socket_util.h"
using std::string;

namespace rocket {

class TcpWrapper
{
public:
	TcpWrapper() {};
	virtual ~TcpWrapper() {};


	int receiveData(int fd, char* buf, int buf_len) {
		return recv(fd, buf, buf_len, 0);
	}
	int sendData(int fd, const char* buf, int buf_len) {
		return send(fd, buf, buf_len, 0);
	}

	int AcceptConnection(int fd, 
						 sockaddr_in& sa, 
						 char* p_clientIp, 
						 int& client_port, 
						 bool ipv6supported = false);

	int AcceptConnection(int fd, sockaddr_in& sa);

	bool SetNonBlocking(int fd);
	bool SetReuseAddr(int fd);
	bool SetLinger(int fd, int nLinger);
protected:
private:
	pthread_mutex_t a_mutex;
};
}

#endif //__TCP_WRAPPER_H__ 


