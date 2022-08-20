#include "EventHandlerVec.h"

using namespace std;

void EventHandlerVec::push_back(const shared_ptr<EventHandler> &handler) {
	if (vec.empty())
		vec.reserve(50);

	vec.push_back(handler);
}

bool EventHandlerVec::pop(const std::shared_ptr<EventHandler> &handler) {
	auto handlerPtr = handler.get();
	for (auto itr = vec.begin(); itr != vec.end(); ++itr) {
		if ((*itr).get() == handlerPtr) { // Tim thay
			vec.erase(itr); // Xoa handler
			return true;
		}
	}
	return false;
}


