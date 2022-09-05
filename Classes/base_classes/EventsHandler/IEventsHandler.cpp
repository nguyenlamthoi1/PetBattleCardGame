#include "IEventsHandler.h"
#include "GameManager.h"

void IEventsHandler::dispatchEvent(const EventID &evId, void* optData) {
	GM_EV_MGR->dispatchEvent(this, evId, optData);
}

std::shared_ptr<EventHandler> IEventsHandler::addEventHandler(const EventID &evId, const EventHandlerFunc &f) {
	return GM_EV_MGR->addEventHandler(this, evId, f);
}
void IEventsHandler::removeHandler(const std::shared_ptr<EventHandler> &handler) {
	GM_EV_MGR->removeHandler(handler);
}
void IEventsHandler::removeHandlersWithEventID(const EventID& eventId) {
	GM_EV_MGR->removeHandlersWithEventID(this, eventId);
}
void IEventsHandler::removeAllHandlers() {
	GM_EV_MGR->removeHandlerForTarget(this);
}