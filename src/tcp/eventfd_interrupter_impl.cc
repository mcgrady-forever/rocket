#if defined(linux) || defined(__linux) || defined(__linux__)
#include "tcp/eventfd_interrupter_impl.h"

#include <sys/eventfd.h>
#include <unistd.h>

namespace rocket {
namespace tcp {

EventfdInterrupterImpl::~EventfdInterrupterImpl() {
	if (_fd != -1) {
		::close(_fd);
	}
}

bool EventfdInterrupterImpl::Create() {
	_fd = ::eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK);
	return _fd >= 0;
}

// interrupt the epoll_wait call.
bool EventfdInterrupterImpl::Interrupt() {
	return ::eventfd_write(_fd, 1) == 0;
}

// Reset the epoll interrupt.
// Returns true if the epoll_wait call was interrupted.
bool EventfdInterrupterImpl::Reset() {
	eventfd_t data;
	return ::eventfd_read(_fd, &data) == 0;
}

}  // namespace tcp
}  // namespace rocket

#endif  // defined(linux) || defined(__linux) || defined(__linux__)