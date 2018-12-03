#ifndef ROCKET_TCP_EVENT_DISPATCHER_H_
#define ROCKET_TCP_EVENT_DISPATCHER_H_

#include <memory>
#include <vector>
#include <unordered_map>
#include <mutex>

#include "concurrency/task.h"
#include "concurrency/thread.h"
#include "tcp/command.h"
#include "tcp/event.h"
#include "tcp/event_poller.h"


namespace rocket {
namespace tcp {

class Server;

class EventDispatcher : public std::enable_shared_from_this<EventDispatcher> {
public:
 	static std::shared_ptr<EventDispatcher> New(const std::string& name,
      											size_t thread_num = 0);
	~EventDispatcher() {};
	
	bool Launch();

	void Shutdown();

	void AddCommand(const Command& command, bool async = true);

	bool ProcessAllPendingCommands(size_t id);

  	bool ProcessEvent(const Event& event, size_t id);

private:
	EventDispatcher(const std::string& name, size_t thread_num = 0);

	class InternalEventTask : public concurrency::Task {
	public:
		InternalEventTask(std::weak_ptr<EventDispatcher> event_center,
						  std::weak_ptr<EventPoller> event_poller);
		~InternalEventTask();

	protected:
		bool operator()(void* arg = NULL);
		void Stop();

	private:
		std::weak_ptr<EventDispatcher> 	_event_disp;
		std::weak_ptr<EventPoller> 		_event_poller;
	};

	// just for short typing
	using ConnectionPtr = std::shared_ptr<ConnectionBase>;

	struct InternalEventPollerInfo {
		std::shared_ptr<concurrency::Thread> 	_event_poller_thread;
		std::shared_ptr<EventPoller> 			_event_poller;
		std::vector<Command> 					_pending_commands;
		std::mutex 								_pending_commands_mutex;

		// all of closures
		// When some event arrives, the EventPoller will call the EventCallback.
		// No need to be protected by lock, because only the corresponding
		// EventPoller thread can access this structure
		std::unordered_map<int, ConnectionPtr>  _connections;
	};

	using InternalEventPollerInfoPtr = std::shared_ptr<InternalEventPollerInfo>;
	std::vector<InternalEventPollerInfoPtr> _internal_event_poller_infos;
	std::string 							_name;

	void ProcessPendingCommand(InternalEventPollerInfoPtr info,
							   const Command& command);
};

} // namespace tcp
} // namespace rocket

#endif // ROCKET_TCP_EVENT_DISPATCHER_H_