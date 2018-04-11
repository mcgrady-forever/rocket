
#ifndef __EPOLL_INTERFACE_H__
#define __EPOLL_INTERFACE_H__


#include <sys/epoll.h>
#include <sys/types.h>

namespace rocket {

#define EPOLL_EVENTS_NUM	10000
#define MAX_EVENTS_NUM 		10000


class EpollWrapper
{
public:
	enum EventType {
		READ_READY 		= 1, // data available to read
		WRITE_READY 	= 2, // connected/data can be written without blocking
		EXCEPTION    	= 4  // uh oh
	};

public:
	EpollWrapper();
	virtual ~EpollWrapper();

    int CreateHandle();
	int DestroyHandle();	
    int AddFdEvent(int fd, u_int events);	
	int DelFdEvent(int fd, u_int events);	
	int ModifyFdEvent(int fd, u_int events);

	int Poll(int timeout = 5000);			
	int GetFdEvents(int index, int& fd, u_int& events);

	bool is_read(u_int events) const { return events & EPOLLIN; }
	bool is_write(u_int events) const { return events & EPOLLOUT; }

private:
	int	 ctl(int op, int fd, u_int events);

private:
    int					epollfd_;
	int					can_operation_fd_num_;  
	epoll_event* 		events_;
	int 				count_;
};
}

#endif


