#include "tcp/connection_id.h"

namespace rocket {
namespace tcp {

std::atomic<ConnectionId> ConnectionIdGenerator::_cur_id(-1);

} // namespace tcp
} // namespace rocket