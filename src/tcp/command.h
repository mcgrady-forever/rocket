#ifndef ROCKET_TCP_COMMAND_H_
#define ROCKET_TCP_COMMAND_H_

#include <functional>
#include <memory>

#include "tcp/connection_base.h"


namespace rocket {
namespace tcp {

// Caller thread should use command to add or remove a connection,
// It includes:
//    the command type,
//    the tcp connection,
class Command {
public:
    enum Type {
        DUMMY = 0x0,
        ADD_CONN = 0x1,
        REMOVE_CONN = 0x2,
        REMOVE_CONN_IMMEDIATELY = 0x4,
        READABLE = 0x8,
        WRITEABLE = 0x10,
    };

    Command(int type,
            std::shared_ptr<ConnectionBase> connection)
        : _type(type)
        , _connection(connection) {
    }

    ~Command() {
    }

    // Two commands will handle a same tcp connection handler if one is copied
    // from another
    Command(const Command& c) {
        if (this != &c) {
            _type = c._type;
            _connection = c._connection;
        }
    }
    Command& operator=(const Command& c) {
        if (this != &c) {
            _type = c._type;
            _connection = c._connection;
        }
        return *this;
    }

    int type() const {
      return _type;
    }

    std::shared_ptr<ConnectionBase> connection() const {
      return _connection;
    }

private:
    int                             _type;
    std::shared_ptr<ConnectionBase> _connection;
};

}  // namespace tcp
}  // namespace rocket

#endif  // ROCKET_TCP_COMMAND_H_