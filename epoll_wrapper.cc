
#include "epoll_wrapper.h"

#include <cstdlib>
#include <cstring>
#include <string>
#include <unistd.h>
#include "logger.h"

namespace rocket {

EpollWrapper::EpollWrapper() {
	epollfd_ = -1;
	can_operation_fd_num_ = 0;
    count_ = 0;
    events_ = (epoll_event*)malloc(sizeof(epoll_event)*MAX_EVENTS_NUM);
    if (NULL == events_)
    {
        LOG_ERROR("EpollWrapper malloc error");
        exit(EXIT_FAILURE);
    }
	memset(events_, 0, sizeof(epoll_event)*MAX_EVENTS_NUM);
	
}

EpollWrapper::~EpollWrapper() {
	DestroyHandle();
    free(events_);
    events_ = NULL;
}

bool EpollWrapper::CreateHandle() {	
    epollfd_ = epoll_create(MAX_EVENTS_NUM); //Since Linux 2.6.8, the size argument is ignored
    if (-1 == epollfd_) {    
        LOG_ERROR("CreateHandle error");
        return false;
    }

    return true; 
}	

int EpollWrapper::DestroyHandle() {	
	if (epollfd_ > -1) {
		close(epollfd_);
		epollfd_ = -1;
		return true;
	}
	
	return false;
}

bool EpollWrapper::AddFdEvent(int fd, u_int events) {
	if (ctl(EPOLL_CTL_ADD, fd, events) == -1) {
		LOG_ERROR("AddFdEvent error, fd=%d", fd);
        return false;
	}
	
	return true;
}

int EpollWrapper::DelFdEvent(int fd, u_int events) {
	int ret =  -1;
	if (ctl(EPOLL_CTL_DEL, fd, events) == 0) {
		ret = 0;
	}

	return ret;
}

int EpollWrapper::ModifyFdEvent(int fd, u_int events) {
	return ctl(EPOLL_CTL_MOD, fd, events) == 0;
}

int	 EpollWrapper::ctl(int op, int fd, u_int events) {
	epoll_event e_event;  
	
	e_event.data.fd = fd; 
	
	if ( op != EPOLL_CTL_DEL)
		e_event.events = EPOLLET;

	if ( events & READ_READY )
		e_event.events |= EPOLLIN;
	if ( events & WRITE_READY )
		e_event.events |= EPOLLOUT;

	return epoll_ctl(epollfd_, op, fd, &e_event); 
}


int EpollWrapper::Poll(int timeout/*= 5000*/) {
	can_operation_fd_num_ = epoll_wait(epollfd_, events_, EPOLL_EVENTS_NUM, timeout);
	return can_operation_fd_num_;
}

int EpollWrapper::GetFdEvents(int index, int& fd, u_int& events) {
	if (index > can_operation_fd_num_ || index >= EPOLL_EVENTS_NUM)
		return -1;

	fd = events_[index].data.fd;
	events = events_[index].events;

	return 0;
}


} // namespace net
