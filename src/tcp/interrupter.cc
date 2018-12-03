#include "tcp/interrupter.h"
#if defined(linux) || defined(__linux) || defined(__linux__)
#include "tcp/eventfd_interrupter_impl.h"
#else
//#include <cnetpp/tcp/pipe_interrupter_impl.h>
#endif

namespace rocket {
namespace tcp {

std::shared_ptr<Interrupter> Interrupter::New() {
#if defined(linux) || defined(__linux) || defined(__linux__)
  	return std::shared_ptr<Interrupter>(new EventfdInterrupterImpl());
#else
  	//return std::make_shared<Interrupter>(new PipeInterrupterImpl());
#endif
}

}  // namespace tcp
}  // namespace rocket