#include "server.h"

#include <limits.h>
#include <vector>

#include "logger.h"
#include "event_dispatcher.h"
#include "socket_util.h"
#include "load_so.h"
#include "unistd.h"

namespace rocket {

Server::Server(const uint32_t port, 
               const int network_threads_num/* = 1*/,
               const int work_threads_num/* = 1*/) 
    : port_(port)
    , listenfd_(0)
    , network_threads_num_(network_threads_num)
    , work_threads_num_(work_threads_num)
    , stop_(false) {
    LOG_INFO("Server::server");
    init();
};

int Server::init() {
	listenfd_ = BaseSocket::socket();
	if (-1 == listenfd_) {
		perror("socket()");
		return -1;
	}

    int ret = -1;
	ret = BaseSocket::setReuseAddr(listenfd_);
    ret = BaseSocket::setNonBlocking(listenfd_);

    ret = BaseSocket::bind(listenfd_, port_);
    ret = BaseSocket::listen(listenfd_, INT_MAX);

    
    int* p = new int[10];

	return ret;
}

void Server::run() {
    LOG_INFO("Server::run()");
    // start network thread
    EventDispatcher* event_dispatcher = new EventDispatcher(this, listenfd_);
    //LOG_INFO("Server::run() 1");
    ThreadBase* thread = new NetworkThread(event_dispatcher);
    network_threads_.push_back(thread);
    thread->start();
    LOG_INFO("network thread start, tid=%d", thread->thread_id());


    // start work thread
    for (int i = 0; i < work_threads_num_; ++i) {
        ThreadBase* thread = new WorkThread(std::bind(&Server::WorkThreadFunc, this));
        work_threads_.push_back(thread);
        thread->start();
        LOG_INFO("work thread start i=%d, tid=%d", i, thread->thread_id());
    }
}

void Server::stop() {
    for (auto it = network_threads_.begin(); it != network_threads_.end(); ++it) {
        delete *it;
        *it = NULL;
    }

    for (auto it = work_threads_.begin(); it != work_threads_.end(); ++it) {
        delete *it;
        *it = NULL;
    }
}

void Server::WorkThreadFunc() {
    LOG_DEBUG("WorkThreadFunc begin");
    while(!stop_) {
        LOG_DEBUG("WorkThreadFunc begin1");
        
        connection_que_ev_.WaitAlways();
        Commu* c  = connection_que_.pop_front();;
        int fd = c->get_fd();

        std::string read_buffer;
        bool end = false;
        BaseSocket::nbRead(fd, read_buffer, end);

        blob_type blob;
        blob.len = read_buffer.size();
        blob.owner = c;
        blob.data = new char[read_buffer.size()];
        memcpy(blob.data, read_buffer.c_str(), read_buffer.size());

        // 检查数据包完整性
        int pktlen = rocket_dll.rocket_handle_input(&blob);
        if (pktlen > 0) {
            LOG_DEBUG("WorkThreadFunc rocket_handle_input, fd=%d read_buffer.size=%d", 
                  fd, read_buffer.size());
            // 处理请求
            rocket_dll.rocket_handle_process(&blob);
        } else {
            std::cout << "rocket_handle_input error" << std::endl;
        }

        delete[] blob.data;
        delete c;
        c = NULL;
        BaseSocket::close(fd);
        LOG_DEBUG("WorkThreadFunc end, fd=%d read_buffer.size=%d", 
                  fd, read_buffer.size());
    }
}

void Server::ReadCallback(int fd) {
    LOG_DEBUG("ReadCallback begin, fd=%d", fd);
    Connection* c = new Connection(fd);
    connection_que_.push_back(c);
    connection_que_ev_.Set();
    LOG_DEBUG("ReadCallback end, fd=%d", fd);
    
    /*
    // handle request
    LOG_DEBUG("ReadCallback READ EVENT, fd=%d", fd);
    std::string read_buffer;
    bool end = false;
    BaseSocket::nbRead(fd, read_buffer, end);

    LOG_DEBUG("ProcessEvents READ EVENT, fd=%d read_buffer.size=%d", 
              fd, read_buffer.size());
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

void Server::WriteCallback(int fd) {

}

} // namespace rocket
