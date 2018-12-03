#ifndef ROCKET_TCP_INTERRUPTER_H_
#define ROCKET_TCP_INTERRUPTER_H_

#include <memory>

namespace rocket {
namespace tcp {

// Currently, we only implements a interrupter based on pipe,
// we will introduce eventfd interrupter in the future.
class Interrupter {
public:
    static std::shared_ptr<Interrupter> New();

    // Destructor.
    virtual ~Interrupter() {}

    // Do initialing work
    virtual bool Create() = 0;

    // Interrupt the poll thread from waiting.
    virtual bool Interrupt() = 0;

    // Poll thread calls this function to reset interrupt state
    virtual bool Reset() = 0;

    // Get the read descriptor to be passed to epoll.
    virtual int get_read_fd() const = 0;

protected:
    Interrupter() {};
};

}  // namespace tcp
}  // namespace rocket

#endif  // ROCKET_TCP_INTERRUPTER_H_