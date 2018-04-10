#ifndef _ROCKET_EVENT_DISPATCHER_H_
#define _ROCKET_EVENT_DISPATCHER_H_

namespace rocket {


class EventDispatcher
{
public:
	EventDispatcher();
	~EventDispatcher();
	
	int Start();
};

}

#endif // _ROCKET_EVENT_DISPATCHER_H_