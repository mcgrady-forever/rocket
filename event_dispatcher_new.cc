#include "event_dispatcher_new.h"

#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>

#include "logger.h"
#include "socket_util.h"
#include "commu.h"
#include "load_so.h"
#include "connection.h"
#include "server.h"

namespace rocket {

EventDispatcher::EventDispatcher(Server* s, const int listenfd) 
	: listenfd_(listenfd)
    , server_(s)
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
    printf("EventDispatcher LOOP\n");
	while (!stop_) {
        //printf("EventDispatcher LOOP1\n");
		int events_num = epoll_.Poll();
		for (int i = 0; i < events_num; ++i) {
            int fd = -1;
            u_int events = 0;
            if (epoll_.GetFdEvents(i, fd, events) != 0) {
                LOG_ERROR("loop getFdEvents error, i=%d", i);
                continue;
            }

            ProcessEvents(fd, events);
		}
	}
}

void EventDispatcher::ProcessEvents(int fd, u_int events) {
    // handle read event
    if(epoll_.is_read(events)) {
        if (fd == listenfd_) {
            int accept_cnt_tcp = 50;

            // 每次poll 最多建立50个连接
            /*while (accept_cnt_tcp --)*/ {
                int port;
                std::string ip;
                int infd;
                infd = BaseSocket::accept(fd, ip, port);
                if (infd == -1 && (errno == EAGAIN)) {
                    //break;
                } else if (infd == -1 && (errno == EINTR)) {
                    //continue;
                } else if (infd == -1) {
                    perror("accept error");
                    LOG_DEBUG("ProcessEvents READ EVENT, listenfd is readable, infd=%d, fd=%d",
                              infd, fd);
                } else {
                    LOG_DEBUG("ProcessEvents READ EVENT, infd=%d, fd=%d, port=%d, ip=%s",
                              infd, fd, port, ip.c_str());
                    BaseSocket::setNonBlocking(infd);
                    if(!epoll_.AddFdEvent(infd, EpollWrapper::READ_READY)) {
                        LOG_ERROR("ProcessEvents READ EVENT, infd=%d, fd=%d, port=%d, ip=%s",
                                  infd, fd, port, ip.c_str());
                        return;
                    }
                } 
            } 
        } else {
            server_->ReadCallback(fd);

            /*
            LOG_DEBUG("ProcessEvents READ EVENT, fd=%d", fd);
            std::string read_buffer;
            bool end = false;
            BaseSocket::nbRead(fd, read_buffer, end);

            LOG_DEBUG("ProcessEvents READ EVENT, fd=%d read_buffer.size=%d", 
                      fd, read_buffer.size());
            
            Commu* c = new Connection(fd);
            blob_type blob;
            blob.len = read_buffer.size();
            blob.owner = c;
            blob.data = new char[read_buffer.size()];
            memcpy(blob.data, read_buffer.c_str(), read_buffer.size());

            // 检查数据包完整性
            int pktlen = rocket_dll.rocket_handle_input(&blob);
            if (pktlen > 0) {
                LOG_DEBUG("ProcessEvents READ EVENT 1, fd=%d read_buffer.size=%d", 
                      fd, read_buffer.size());
                // 处理请求
                rocket_dll.rocket_handle_process(&blob);
            } else {
                std::cout << "rocket_handle_input error" << std::endl;
            }

            delete[] blob.data;
            delete c;
            c = NULL;
            */
        }
    }

    // handle write event
    if (epoll_.is_write(events)) {
        LOG_ERROR("ProcessEvents WRITE EVENT, fd=%d", fd);
    }
}

}