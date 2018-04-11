#include "event_dispatcher.h"
#include <cstdlib>
#include "logger.h"

namespace rocket {

EventDispatcher::EventDispatcher(const uint32_t listenfd) 
	: listenfd_(listenfd) {
	if (!init())
	{
		exit(EXIT_FAILURE);
	}
}

EventDispatcher::~EventDispatcher() {

}

int EventDispatcher::init() {
	int ret = 0;
	ret = epoll_.CreateHandle();
	if (ret != 0) return -1;

	ret = epoll_.AddFdEvent(listenfd_, EpollWrapper::READ_READY);
	if (ret != 0) return -1;

	return ret;
}

int EventDispatcher::loop() {
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