#ifndef __EVENT_WRAPPER_H_20120509__
#define __EVENT_WRAPPER_H_20120509__

#include <pthread.h>
#include <stdint.h>

namespace rocket {

class EventWrapper {
public: 
	enum {
		EVENT_HAVEEVENT = 1,
		EVENT_TIMEOUT = 2,
		EVENT_ERROR = -1
	};

public:
	EventWrapper();
	~EventWrapper();

public:
	void Set();
	//unit:ms
	int  Wait(uint32_t dw_timeout);
	int  WaitAlways();
	bool is_wait() { return is_wait_event_; }
	int  get_set_event_count() { return set_event_count_; }
	
private:
	void InitEvent();
	void DestEvent();

private:
	bool 			is_wait_event_;
	uint32_t 		set_event_count_;
	pthread_mutex_t mutex_;
	pthread_cond_t  cond_;

};

} // rocket

#endif 