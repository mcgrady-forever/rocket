#if defined(linux) || defined(__linux) || defined(__linux__)

#ifndef ROCKET_TCP_EVENTFD_INTERRUPTER_IMPL_H_
#define ROCKET_TCP_EVENTFD_INTERRUPTER_IMPL_H_

#include "tcp/interrupter.h"

namespace rocket {
namespace tcp {

class EventfdInterrupterImpl : public Interrupter {
public:
    EventfdInterrupterImpl() 
        : _fd(-1) {
    }

    ~EventfdInterrupterImpl();

    bool Create();

    bool Interrupt();

    bool Reset();

    int get_read_fd() const {
        return _fd;
    }

private:
    int _fd;
};

}  // namespace tcp
}  // namespace rocket

#endif  // ROCKET_TCP_EVENTFD_INTERRUPTER_IMPL_H_
#endif  // defined(linux) || defined(__linux) || defined(__linux__)