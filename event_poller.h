// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#ifndef EVENT_POLLER_H_
#define EVENT_POLLER_H_

#include <memory>

namespace rocket {

class EventPoller {
 public:
    virtual ~EventPoller() {
    }

    /**
     * factory function. Create a EventPoller instance.
     * @param id              the identifier of the EventPoller
     * @param max_connections the maximum numbers of connections this event poller
     *                        supports
     * @return the EventPoller instance
     */
    static std::shared_ptr<EventPoller> New(size_t id,
                                            size_t max_connections = 1024);

    /**
     * Initialize the EventPoller.
     * It includes,
     * - initilizing the interruptor,
     * - create an epoll file descriptor(if using epoll), or
     * - create an event handler(if using MsgWaitForMultipleObjectsEx for windows).
     * @param iEventCenter the EventCenter pointer
     * @return true if initialization is succeeded, else false.
     */
    virtual bool Init(std::shared_ptr<EventCenter> event_center);

    /**
     * @breif Poll avaliable IO event & command.
     * dispatch events to connection callback via EventCenter::ProcessEvent()
     * - if interruptor is readable, then process Commands.
     * - if connection is readable/writeable process Events.
     * - dispatch events to Connections and invoke event callbacks
     * @note this function is called within worker thread.
     * @return false if error occured, else true.
     */
    virtual bool Poll() = 0;
  
    /**
     * @brief Shutdown the EventPoller.
     * It includes,
     * - destruct the interruptor,
     * - cancel all waiting requests,
     * - close the epoll file descriptor(if using epoll), or
     * - close the event handler(if using MsgWaitForMultipleObjectsEx).
     */
    virtual void Shutdown() {
        //interrupter_.reset();
        DoShutdown();
    }

    /**
     * @return the identifier of EventPoller instance
     */
    size_t id() const {
        return id_;
    }

    /**
     * Process Command from user thread or Connection callbacks.
     * @param command Command
     * @return true on success, false on failed
     * @note this function is called by EventCenter::ProcessAllPendingCommands
     */
    virtual bool ProcessCommand(const Command& command);

protected:
    EventPoller(int id, size_t max_connections)
        : id_(id)
        , max_connections_(max_connections) {
    }

    // child classes should implement this method if it want to do some extra
    // intializations.
    virtual bool DoInit() {
        return true;
    }

    // child classes should implement this method if it want to do some extra
    // finalizations.
    virtual void DoShutdown() {
    }

    virtual bool ProcessInterrupt();

    virtual bool AddPollerEvent(Event&& event) = 0;
    virtual bool ModifyPollerEvent(Event&& event) = 0;
    virtual bool RemovePollerEvent(Event&& event) = 0;

    int id_; // the id
    size_t max_connections_;
    std::weak_ptr<EventCenter> event_center_;
};

} // namespace

#endif // EVENT_POLLER_H_