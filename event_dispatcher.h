#ifndef ROCKET_EVENT_DISPATCHER_H
#define ROCKET_EVENT_DISPATCHER_H

namespace rocket {


class EventDispatcher
{
public:
	EventDispatcher();
	~EventDispatcher();
	
	int Start();
};

}

#endif // ROCKET_EVENT_DISPATCHER_H_