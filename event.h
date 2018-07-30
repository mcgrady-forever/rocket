// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#ifndef EVENT_H_
#define EVENT_H_


namespace rocket {

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
        : fd_(fd)
        , mask_(static_cast<int>(Type::DUMMY)) {
    }

    Event(int fd, int mask) 
        : fd_(fd)
        , mask_(mask) {
    }

    int fd() const {
        return fd_;
    }

    int mask() const {
        return mask_;
    }

    int& mutable_mask() {
        return mask_;
    }

private:
    Event(const Event&);
    Event& operator=(const Event&);

    int fd_;
    int mask_;
};

} // rocket


#endif // EVENT_H_