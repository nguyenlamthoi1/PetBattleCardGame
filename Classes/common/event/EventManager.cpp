#include "EventManager.h"
#include "EventHandlerVec.h"
#include "EventHandler.h"
#include "MEvent.h"

#include <algorithm>

#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

EventManager::EventManager() {
	waitAdded.reserve(50);
}

EventManager::~EventManager() {

}

void EventManager::dispatchEvent(const shared_ptr<MEvent> &ev) {
	if (!enabled)
		return;

	bool shouldStopPropagation = false;
	auto target = ev->getTarget();
	auto evId = ev->getEventId();
	
	sortHandlersWithPriority(target, evId);

	auto targetMap = handlersMap[target];
	auto iter = targetMap.find(evId);
	if (iter != targetMap.end())
	{
		auto handlersVec = iter->second;

		auto onEvent = [ev](shared_ptr<EventHandler> handler) -> bool{
			auto evHandler = handler.get();
			(*evHandler)(ev);
			return ev->isStopped();
		};
		auto &vec = handlersVec->vec;

		dispatching = true; /// * Dispatching start
		 
		for (const auto &evHandler : vec)
		{
			if (evHandler->isEnabled() && evHandler->isRegistered() && 
				/// Event Handler func duoc call. * Neu event bi stop thi onEvent(evHandler) return false -> se khong break
				onEvent(evHandler)) /// 
			{
				shouldStopPropagation = true;
				break;
			}
		}

		dispatching = false; /// * Dispatching end
		onDispatchingEnded();
	}

}

void EventManager::onDispatchingEnded() {
	for (const auto &handler : waitAdded) {
		addHandler(handler);
	}
	waitAdded.clear();

	for (const auto &handler : waitRemoved) {
		removeHandler(handler);
	}
	waitRemoved.clear();
}

void EventManager::dispatchEvent(void* target, const EventID &evId, void* optData) {
	auto ev = shared_ptr<MEvent>(new MEvent(target, evId));
	ev->setData(optData);
	dispatchEvent(ev);
}

std::shared_ptr<EventHandler> EventManager::addEventHandler(void *target, const EventID &evId, const EventHandlerFunc &f) {
	auto handler = shared_ptr<EventHandler>(new EventHandler(target, evId, f));
	addEventHandlerWithPriority(handler, 1);
	return handler;
}


void EventManager::addEventHandlerWithPriority(const std::shared_ptr<EventHandler> &handler, int p) {
	CCASSERT(handler, "Invalid parameters.");
	CCASSERT(!handler->isRegistered(), "The handler has been registered.");

	if (!handler->isAvailable())
		return;

	handler->setPriority(p);
	handler->setRegistered(true);

	addHandler(handler);
}

void EventManager::addHandler(const shared_ptr<EventHandler> &handler) {
	if (dispatching) {
		waitAdded.push_back(handler);
	}
	else {
		forceAddHandler(handler);
	}
}

void EventManager::forceAddHandler(const shared_ptr<EventHandler> &handler) {
	shared_ptr<EventHandlerVec> tempVec; /// = nullptr
	
	auto evId = handler->getEventID();
	auto target = handler->getTarget();

	auto &targetHanldersMap = handlersMap[target]; /// Du co key == target hay khong, van se can tao 1 (empty) map cho target nay

	auto itr = targetHanldersMap.find(evId);
	if (itr == targetHanldersMap.cend())
	{
		tempVec = shared_ptr<EventHandlerVec>(new EventHandlerVec());
		targetHanldersMap[evId] =  tempVec;
	}
	else
	{
		tempVec = itr->second;
	}

	tempVec->push_back(handler);
	setDirty(target, evId, DirtyFlag::PRIORITY);
}	

void EventManager::sortHandlersWithPriority(void *target, const EventID &evId) {
	auto handlersVec = handlersMap[target][evId];

	if (handlersVec->empty())
		return;

	auto &vec = handlersVec->vec;

	// After sort: priority < 0, > 0
	std::sort(vec.begin(), vec.end(), [](const shared_ptr<EventHandler> &l1, const shared_ptr<EventHandler> &l2) {
		return l1->getPriority() < l2->getPriority();
		});


	// For test
	/*
	for (auto& l : *fixedListeners)
	{
		log("listener priority: node (%p), fixed (%d)", l->_node, l->_fixedPriority);
	}
	*/
}

///Remove Handlers///
void EventManager::removeHandler(const shared_ptr<EventHandler> handler) {
	if (!handler)
		return;

	auto handlerPtr = handler.get();
	bool isFound = false;
	auto target = handler->getTarget();
	auto evId = handler->getEventID();

	auto &targetMap = handlersMap[target];
	auto itr = targetMap.find(evId);
	bool removedSuc = false;
	auto &handlerVec = itr->second;

	if (itr != targetMap.end()) {
		if (!dispatching)
			removedSuc = handlerVec->pop(handler);
		else
			waitRemoved.push_back(handler); // dang dispatching, se duoc xoa sau(khi dispatching xong)
	}

	if (handlerVec->empty()) { // * Sau khi xoa thi vec empty tai EventID
		targetMap.erase(itr);
	}

	if (removedSuc) { // Remove thanh cong
		setDirty(target, evId, DirtyFlag::PRIORITY);
	}
	else
	{
		for (auto iter = waitAdded.begin(); iter != waitAdded.end(); ++iter)
		{
			if ((*iter).get() == handlerPtr)
			{
				handler->setRegistered(false);
				waitAdded.erase(iter);
				break;
			}
		}
	}

	//#if CC_NODE_DEBUG_VERIFY_EVENT_LISTENERS
//		CCASSERT(_inDispatch != 0 ||
//			!sceneGraphPriorityListeners ||
//			std::count(sceneGraphPriorityListeners->begin(), sceneGraphPriorityListeners->end(), listener) == 0,
//			"Listener should be in no lists after this is done if we're not currently in dispatch mode.");
//
//		CCASSERT(_inDispatch != 0 ||
//			!fixedPriorityListeners ||
//			std::count(fixedPriorityListeners->begin(), fixedPriorityListeners->end(), listener) == 0,
//			"Listener should be in no lists after this is done if we're not currently in dispatch mode.");
//#endif
}
void EventManager::removeHandlerForTarget(void *target) {
	auto itr = handlersMap.find(target);
	if (itr != handlersMap.end())
	{
		auto &targetMap = itr->second;
		if (targetMap.empty() || dispatching)
			return;

		handlersMap.erase(target); // * Xoa EventHandlerVec theo EventID
		priorityDirtyFlagMap.erase(target); // Remove the dirty flag  
	}

	/// Xoa handler trong danh sach waitAdded
	for (auto iter = waitAdded.begin(); iter != waitAdded.end();)
	{
		if ((*iter)->getTarget() == target) // 
		{
			(*iter)->setRegistered(false);
			iter = waitAdded.erase(iter);
		}
		else
			++iter;
	}
}

void EventManager::removeHandlersWithEventID(void *target, const EventID& evId) {
	auto &targetMap = handlersMap[target];
	auto itr = targetMap.find(evId);
	if (itr != targetMap.end())
	{
		auto handlersVec = itr->second;
		if (handlersVec->empty() || dispatching)
			return;

		targetMap.erase(itr); // * Xoa EventHandlerVec theo EventID
		priorityDirtyFlagMap[target].erase(evId);
	}

	/// Xoa handler trong danh sach waitAdded
	for (auto iter = waitAdded.begin(); iter != waitAdded.end();)
	{
		if ((*iter)->getEventID() == evId) // 
		{
			(*iter)->setRegistered(false);
			iter = waitAdded.erase(iter);
		}
		else
			++iter;
	}
}

void EventManager::removeAllHandlers() {
	if (!dispatching)
		handlersMap.clear();
	
}

///Dirty Flag///
void EventManager::setDirty(void *target, const EventID& evId, DirtyFlag flag) {

	auto targetMap = priorityDirtyFlagMap[target]; /// Du co key == target hay khong, van se can tao 1 (empty) map cho target nay

	auto itr = targetMap.find(evId);
	if (itr == targetMap.cend())
	{
		targetMap.insert(std::make_pair(evId, flag));
	}
	else {
		int ret = (int)flag | (int)itr->second;
		itr->second = (DirtyFlag)ret;
	}
}