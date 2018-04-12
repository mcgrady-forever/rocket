#include "event_dispatcher.h"
#include <cstdlib>
#include "logger.h"

namespace rocket {

EventDispatcher::EventDispatcher(const int listenfd) 
	: listenfd_(listenfd)
    , stop_(false) {
    LOG_INFO("EventDispatcher");
	if (init() != 0)
	{
		exit(EXIT_FAILURE);
	}
}

EventDispatcher::~EventDispatcher() {

}

int EventDispatcher::init() {
    LOG_INFO("EventDispatcher init 1");
	int ret = 0;
	if (!epoll_.CreateHandle()) {
        perror("epoll_.CreateHandle()");
        return -1;
    }
        
	if(!epoll_.AddFdEvent(listenfd_, EpollWrapper::READ_READY)) {
        perror("epoll_.AddFdEvent()");
        return -1;
    }

    LOG_INFO("EventDispatcher init 2");  
	return ret;
}

int EventDispatcher::loop() {
    LOG_INFO("EventDispatcher LOOP");
	while (!stop_) {
		int events_num = epoll_.Poll();
		for (int i = 0; i < events_num; ++i)
		{
            int fd = -1;
            u_int events = 0;
            if (!epoll_.GetFdEvents(i, fd, events))
            {
                LOG_ERROR("loop getFdEvents error, i=%d", i);
                continue;
            }

            ProcessEvents(fd, events);
		}
	}
}

void EventDispatcher::ProcessEvents(int fd, u_int events) {
    if(epoll_.is_read(events)) {
        LOG_DEBUG("ProcessEvents READ EVENT, fd=%d", fd);
        //TODO: add to task queue
    }
}

}