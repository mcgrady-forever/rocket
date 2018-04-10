#ifndef _THREAD_WRAPPER_H_
#define _THREAD_WRAPPER_H_

#include <functional>

namespace rocket {

class ThreadWrapper
{
public:
	typedef std::function<void()> ThreadCallback;

	ThreadWrapper(ThreadCallback cb) : cb_(cb), pthread_id_(0) { }

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
	
		((ThreadWrapper*)param)->run();

		return NULL;
	}

	void run() {
		cb_();
	}

	ThreadCallback cb_;
	pthread_t pthread_id_;
};

}
#endif // _THREAD_WRAPPER_H_


