#ifndef _SOCKET_UTIL_H_
#define _SOCKET_UTIL_H_

#include <string>
#include <net/if.h>
#include <net/if_arp.h>
#include <arpa/inet.h>
#include <sys/resource.h>

namespace rocket {

#define SOCKET_ERROR            (-1)
#define INVALID_SOCKET  (unsigned int)(~0)
#define SOCKET_ERROR_OK				0
#define SOCKET_ERROR_PARAM			-1
#define SOCKET_ERROR_TIMEOUT		-2
#define SOCKET_ERROR_DISCONNECTED	-3
#define SOCKET_TIMEOUT_SELECT		5

#ifdef _DEBUG
#define _MAKE_DEBUG		struct rlimit sLimit;	\
	sLimit.rlim_cur = -1; \
	sLimit.rlim_max = -1; \
	setrlimit(RLIMIT_CORE, &sLimit); 
#else
#define _MAKE_DEBUG
#endif


class BaseSocket {
public:

static void initSock();
static void uninitSock();
//! Creates a stream (TCP) socket. Returns -1 on failure.
static int socket(int type = 1/*SOCK_STREAM*/);

//! Closes a socket.
static void close(int socket);

//! Sets a stream (TCP) socket to perform non-blocking IO. Returns false on failure.
static bool setNonBlocking(int socket);

//! Read text from the specified socket. Returns false on error.
static bool nbRead(int socket, std::string& s, bool *eof);
static int nbRead(int s, char* pData, unsigned &nLen, unsigned nTimeout = 0);

//! Write text to the specified socket. Returns false on error.
static bool nbWrite(int socket, std::string& s, int *bytesSoFar);
static int nbWrite(int s, const char* pData, unsigned &nLen, unsigned nTimeout = 0);


// The next four methods are appropriate for servers.

//! Allow the port the specified socket is bound to to be re-bound immediately so 
//! server re-starts are not delayed. Returns false on failure.
static bool setReuseAddr(int socket);

//! Bind to a specified port
static bool bind(int socket, int port, const char* pIp = NULL);

//! Set socket in listen mode
static bool listen(int socket, int backlog);

//! Accept a client connection request
static int accept(int socket, std::string& ip, int& port);

//! Connect a socket to a server (from a client)
static bool connectByHostName(int socket, int port, std::string& host);
static bool connectByIP(int socket, int port, const char* pIP);


//! Returns last errno
static int getError();

//! Returns message corresponding to last error
static std::string getErrorMsg();

//! Returns message corresponding to error
static std::string getErrorMsg(int error);
};

} // namespace BaseSocket

#endif
