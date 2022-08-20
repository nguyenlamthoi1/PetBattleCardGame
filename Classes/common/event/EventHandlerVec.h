#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

class EventManager;
class EventHandler;

class EventHandlerVec final {
public:
	friend class EventManager;

	EventHandlerVec() = default;
	virtual ~EventHandlerVec() = default;

	size_t size() const { return vec.size(); };
	bool empty() const { return vec.empty(); }

	void push_back(const std::shared_ptr<EventHandler> &handler);
	bool pop(const std::shared_ptr<EventHandler> &handler);
	void clear() { vec.clear(); }

private:
	std::vector<std::shared_ptr<EventHandler>> vec;
};

