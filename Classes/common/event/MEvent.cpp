#include "MEvent.h"

MEvent::MEvent(void *target, const EventID &eId) :
target(target),
evId(eId),
data(nullptr)
{

}

MEvent::~MEvent() {

}

