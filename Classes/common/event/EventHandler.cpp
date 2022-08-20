#include "EventHandler.h"

using namespace std;

EventHandler::EventHandler(void* t, const EventID &eId, const EventHandlerFunc &f) :
	evId(eId),
	onEvent(f),
	enabled(true),
	priority(1),
	target(t),
	registered(false)
{
}

EventHandler::~EventHandler() {

}

void EventHandler::operator()(const shared_ptr<MEvent>& ev){
	onEvent(ev);
}

/// Getters and Setters
bool EventHandler::isAvailable() const {
	return !evId.empty() && onEvent && target != nullptr;
}

/// 
std::shared_ptr<EventHandler> EventHandler::shClone() const {
	return shared_ptr< EventHandler>(clone());
}
EventHandler* EventHandler::clone() const {
	EventHandler* ret = new (std::nothrow) EventHandler(target, evId, onEvent);
	return ret;
}
