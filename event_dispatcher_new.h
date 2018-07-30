#ifndef EVENT_DISPATCHER_H_
#define EVENT_DISPATCHER_H_

#include "epoll_wrapper.h"
#include "connection.h"

namespace rocket {

class Server;

class EventDispatcher
{
public:
	EventDispatcher(Server* s, const int listenfd);
	~EventDispatcher();
	
	int init();

	int loop();

private:
	void ProcessEvents(int fd, u_int events);

private:
	Server*       		server_;
	uint32_t    		listenfd_;
	EpollWrapper 		epoll_;
	int 				epollfd_;
	bool            	stop_;
};

}

#endif // EVENT_DISPATCHER_H_