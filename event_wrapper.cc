#include "event_wrapper.h"
#include <errno.h>

namespace rocket {

EventWrapper::EventWrapper() {
	InitEvent();
}

EventWrapper::~EventWrapper() {
	DestEvent();	
}

void EventWrapper::InitEvent() {
	pthread_mutex_init(&mutex_, NULL);
	pthread_cond_init(&cond_, (pthread_condattr_t *)PTHREAD_PROCESS_PRIVATE);
	is_wait_event_ = false;
	set_event_count_ = 0;
}

void EventWrapper::DestEvent() {
	pthread_cond_destroy(&cond_);
	pthread_mutex_destroy(&mutex_);
}

void EventWrapper::Set() {
	pthread_mutex_lock(&mutex_);   
	pthread_cond_signal(&cond_);
	set_event_count_++;
	pthread_mutex_unlock(&mutex_);
}

//unit:ms
int EventWrapper::Wait(uint32_t dw_timeout) {
	int res = 0;
	is_wait_event_ = true;
	pthread_mutex_lock(&mutex_);

	if (set_event_count_ == 0) {
		if (dw_timeout > 0) {
			struct timespec m_timv;

			m_timv.tv_sec = time(NULL) + dw_timeout / 1000;
			m_timv.tv_nsec = (dw_timeout % 1000) * 1000000;

			res = pthread_cond_timedwait(&cond_, &mutex_, &m_timv);		
			if (res == ETIMEDOUT) {
				res = EVENT_TIMEOUT;
			} else if (res == 0) {
				res = EVENT_HAVEEVENT;
				if (set_event_count_ > 0)
					set_event_count_--;
			} else {
				res = EVENT_ERROR;
			}
		} else {
			res = EVENT_TIMEOUT;
		}
	} else	{
		if(set_event_count_ > 0)
			set_event_count_--;
		res = EVENT_HAVEEVENT;
	}
	pthread_mutex_unlock(&mutex_);

	is_wait_event_ = false;
	return res;
}

int EventWrapper::WaitAlways() {
	int res = 0;
	is_wait_event_ = true;
	pthread_mutex_lock(&mutex_);

	if (set_event_count_== 0) {
		res = pthread_cond_wait(&cond_, &mutex_); 	   
		if (res == 0) {
			if (set_event_count_> 0)
				set_event_count_--;
			res = 1;
		} else {
			res = -1;
		}
	} else {
		if(set_event_count_ > 0)
			set_event_count_--;
	}
	pthread_mutex_unlock(&mutex_);

	is_wait_event_ = false;
	return res;
}

}
