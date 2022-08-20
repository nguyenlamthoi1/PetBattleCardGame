#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>

class EventHandlerVec;
class EventHandler;
class MEvent;

/*
	Hint:
	- Singleton instance duoc quan ly boi GameManager, khong nen tu tao EventManager
*/
class EventManager final {
public:
	using EventID = std::string;
	using TargetID = void*;
	using EventHandlerFunc = std::function<void(const std::shared_ptr<MEvent>&)>;

	EventManager();
	virtual ~EventManager();

	void dispatchEvent(const std::shared_ptr<MEvent> &ev);
	void dispatchEvent(void* target, const EventID &evId, void* optData = nullptr);


	std::shared_ptr<EventHandler> addEventHandler(void *target, const EventID &evId, const EventHandlerFunc &f);
	void addEventHandlerWithPriority(const std::shared_ptr<EventHandler> &handler, int p);

	///Remove handlers///
	void removeHandler(const std::shared_ptr<EventHandler> handler);
	void removeHandlerForTarget(void *target);
	void removeHandlersWithEventID(void *target, const EventID& customEventName);
	void removeAllHandlers();

	///Getters and Setters///
	bool isEnabled() const { return enabled; }
	void setEnabled(bool e) { enabled = e; }

private:
	enum class DirtyFlag
	{
		NONE = 0,
		PRIORITY = 1 << 0,
		ALL = PRIORITY
	};
	std::unordered_map<void*, std::unordered_map<EventID, DirtyFlag>> priorityDirtyFlagMap;
	void setDirty(void *target, const EventID& evId, DirtyFlag flag);

	void addHandler(const std::shared_ptr<EventHandler> &handler);
	void forceAddHandler(const std::shared_ptr<EventHandler> &handler);
	void sortHandlersWithPriority(void *target, const EventID &evId);

	std::unordered_map<TargetID, std::unordered_map<EventID, std::shared_ptr<EventHandlerVec>>> handlersMap;
	std::vector<std::shared_ptr<EventHandler>> waitAdded;
	std::vector<std::shared_ptr<EventHandler>> waitRemoved;
	void onDispatchingEnded();

	bool dispatching = false;
	bool enabled = true;
};

#endif // __EVENT_MANAGER_H__