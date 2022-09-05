#pragma once

#include <string>
#include <unordered_map>
#include <functional>

#include "common/event/EventManager.h"

///*
//	- Abstract Class
//	* Nhung Class can handle nhung event do object phat ra thi ke thua Class nay
//	* Chua ho tro lang nghe event lien tuc -> Callback ton tai cho den khi Event duoc phat ra
//*/
//class IEventsHandler {
//	struct EventData;
//	using EventType = std::string;
//	using EventCallback = std::function<void(const EventType& ev, EventData* data)>;
//
//protected:
//	IEventsHandler() = default;
//	virtual ~IEventsHandler() = default;
//
//	std::unordered_map<EventType, EventCallback> evMap;
//	
//	void dispatchEvent(const EventType &evType, Event *evData);
//	void dispatchEvent(const EventType &evType);
//
//	void addEventCallback(const EventType &ev, const EventCallback &cb);
//
//};
//
//struct IEventsHandler::EventData {
//	EventType evType;
//};

/*
	- Abstract Class
	* Nhung Class can handle nhung event do object phat ra thi ke thua Class nay
*/
class IEventsHandler {
public:
	using EventHandlerFunc = EventManager::EventHandlerFunc;
	using EventID = EventManager::EventID;

	std::shared_ptr<EventHandler> addEventHandler(const EventID &evId, const EventHandlerFunc &f);
	void removeHandler(const std::shared_ptr<EventHandler> &handler);
	void removeHandlersWithEventID( const EventID& eventId);
	void removeAllHandlers();

protected:
	IEventsHandler() = default;
	virtual ~IEventsHandler() = default;
	void dispatchEvent(const EventID &evId, void* optData = nullptr);
};

