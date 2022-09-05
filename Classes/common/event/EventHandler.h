#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <functional>

class MEvent;

class EventHandler final { /// * Co the loi khi ke thua, can nghien cuu them dieu nay
public:
	using EventHandlerFunc = std::function<void(const std::shared_ptr<MEvent>&)>;
	using EventID = std::string;

	EventHandler(void* t, const EventID &eId, const EventHandlerFunc &f);
	virtual ~EventHandler();

	/// Getters and Setters
	void *getTarget() const { return target; }

	EventID getEventID() const { return evId; }

	bool isEnabled() const { return enabled; }
	void setEnabled(bool  e) { enabled = e; }

	int getPriority() const { return priority; }
	void setPriority(int  p) { priority = p; }

	bool isAvailable() const;

	bool isRegistered() const { return registered; }
	void setRegistered(bool r) { registered = r; }
	
	std::shared_ptr<EventHandler> shClone() const;
	EventHandler* clone() const;

	void operator()(const std::shared_ptr<MEvent>&);

protected:
	EventHandlerFunc onEvent;
	EventID evId;
	bool enabled;
	int priority;
	void *target;
	bool registered;
	bool callOnce = false;
};

