#ifndef _THREAD_WRAPPER_H_
#define _THREAD_WRAPPER_H_

#include <functional>
#include <iostream>
#include "event_dispatcher.h"

namespace rocket {


class ThreadBase
{
public:
	typedef std::function<void()> ThreadCallback;

	ThreadBase(ThreadCallback cb) : cb_(cb), pthread_id_(0) { }

	bool start() {	
		std::cout << "base.run" << std::endl;	
		int ret = pthread_create(&pthread_id_, NULL, TheThread, this);	
		return ret == 0 ? true : false;
	}

	pthread_t thread_id() {
        return pthread_id_;
    }

protected:
	ThreadCallback cb_;

private:
	static void *TheThread(void *param) {
		pthread_detach(pthread_self());
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
		pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	
		((ThreadBase*)param)->run();

		return NULL;
	}

	virtual void run() {
		std::cout << "base.run" << std::endl;
		cb_();
	}

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
	NetworkThread(/*ThreadCallback cb,*/ 
			      EventDispatcher* ev_dispatcher) 
		: ThreadBase(std::bind(&EventDispatcher::loop, ev_dispatcher))
		, ev_dispatcher_(ev_dispatcher) {
	};
	~NetworkThread() {
		if (ev_dispatcher_ != NULL) {
			delete ev_dispatcher_;
			ev_dispatcher_ = NULL;
		}
	};

	/*
	bool start() {
		_thread.start();
	}
	*/

	virtual void run() {
		std::cout << "Derived.run" << std::endl;
		cb_();
	}
private:
	EventDispatcher* ev_dispatcher_;
	//ThreadBase _thread;
};

}
#endif // _THREAD_WRAPPER_H_


