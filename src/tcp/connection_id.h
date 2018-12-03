#ifndef ROCKET_TCP_CONNECTION_ID_H_
#define ROCKET_TCP_CONNECTION_ID_H_

#include <atomic>

namespace rocket {
namespace tcp {

using ConnectionId = int64_t;
const ConnectionId INVALID_CONNECTION_ID = -1; // -1

class ConnectionIdGenerator {
public:
    static ConnectionId Generate() {
        return ++_cur_id;
    }

private:
    static std::atomic<ConnectionId> _cur_id;
};

} // namespace tcp
} // namespace rocket

#endif // ROCKET_TCP_CONNECTION_ID_H_