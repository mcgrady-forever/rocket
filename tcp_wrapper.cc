#include "tcp_wrapper.h"

#include <arpa/inet.h>
#include <fcntl.h>

#include <string>

namespace rocket {

int TcpWrapper::AcceptConnection(int fd, 
								 sockaddr_in& sa, 
								 char* p_clientIp, 
								 int& clientPort, 
								 bool ipv6supported /*=false*/) {
	int clientFd = -1;
	if (ipv6supported) {
		struct sockaddr_in6 clientAddrIPV6; //ipv6
		socklen_t length = sizeof(clientAddrIPV6);

		clientFd = accept(fd, (struct sockaddr*) &clientAddrIPV6, &length);
		if ( clientFd < 0 )
			return clientFd;

		inet_ntop(AF_INET6, &clientAddrIPV6.sin6_addr, p_clientIp, (socklen_t)256); // IPv6
		clientPort = ntohs(clientAddrIPV6.sin6_port);
	} else {	
		memset(&sa, 0, sizeof(sa));   
		socklen_t salen = sizeof(sa);   

		clientFd = accept(fd, (struct sockaddr *)&sa, &salen);
		if ( clientFd < 0 )
			return clientFd;

		inet_ntop(AF_INET,&sa.sin_addr,p_clientIp,(socklen_t)256);
		clientPort = ntohs(sa.sin_port);
	}

	return clientFd;
}

int TcpWrapper::acceptConnection(int fd, sockaddr_in& sa)
{
	memset(&sa, 0, sizeof(sa));   
	socklen_t salen = sizeof(sa);   

	return accept(fd, (struct sockaddr *)&sa, &salen);
}

bool TcpWrapper::setNonBlocking(int fd)
{
	return (fcntl(fd, F_SETFL, O_NONBLOCK) == 0);
}


bool TcpWrapper::setReuseAddr(int fd)
{
	// Allow this port to be re-bound immediately so server re-starts are not delayed
	int sflag = 1;
	return (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (const char *)&sflag, sizeof(sflag)) == 0);
}

bool TcpWrapper::setLinger(int fd, int nLinger)
{
	return true;
	struct   linger   ling; 
	ling.l_onoff   =   1; 
	ling.l_linger   =   0; 
	return (::setsockopt(fd, SOL_SOCKET, SO_LINGER, (const char*) &ling,
		sizeof(ling)) == 0);
}

} // namespace rocket

