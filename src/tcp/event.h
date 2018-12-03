// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#ifndef ROCKET_TCP_EVENT_H_
#define ROCKET_TCP_EVENT_H_


namespace rocket {
namespace tcp {

// When the EventPoller finds some socket is readable or writable, it will
// return an event
class Event {
public:
    enum Type {
        DUMMY   = 0x0,
        READ    = 0x01,
        WRITE   = 0x02,
        CLOSE   = 0x04,
    };

    explicit Event(int fd) 
        : _fd(fd)
        , _mask(static_cast<int>(Type::DUMMY)) {
    }

    Event(int fd, int mask) 
        : _fd(fd)
        , _mask(mask) {
    }

    int fd() const {
        return _fd;
    }

    int mask() const {
        return _mask;
    }

    int& mutable_mask() {
        return _mask;
    }

private:
    Event(const Event&);
    Event& operator=(const Event&);

    int _fd;
    int _mask;
};

} // namespace tcp
} // namespace rocket


#endif // ROCKET_TCP_EVENT_H_