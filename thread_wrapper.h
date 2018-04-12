#ifndef _THREAD_WRAPPER_H_
#define _THREAD_WRAPPER_H_

#include <functional>
#include "event_dispatcher.h"

namespace rocket {


class ThreadBase
{
public:
	typedef std::function<void()> ThreadCallback;

	ThreadBase(ThreadCallback cb) : cb_(cb), pthread_id_(0) { }

	bool start() {		
		int ret = pthread_create(&pthread_id_, NULL, TheThread, this);	
		return ret == 0 ? true : false;
	}

	pthread_t thread_id() {
        return pthread_id_;
    }

private:
	static void *TheThread(void *param) {
		pthread_detach(pthread_self());
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	
		((ThreadBase*)param)->run();

		return NULL;
	}

	void run() {
		cb_();
	}

	ThreadCallback cb_;
	pthread_t pthread_id_;
};

class WorkThread : public ThreadBase
{
public:
	WorkThread(ThreadCallback cb) 
		: ThreadBase(cb)
	{};
	~WorkThread() {};

private:
};

class NetworkThread : public ThreadBase
{
public:
	NetworkThread(ThreadCallback cb, 
			      const EventDispatcher& ev_dispatcher) 
		: ThreadBase(cb)
		, ev_dispatcher_(ev_dispatcher)
	{};
	~NetworkThread() {};

private:
	EventDispatcher ev_dispatcher_;
};

}
#endif // _THREAD_WRAPPER_H_


