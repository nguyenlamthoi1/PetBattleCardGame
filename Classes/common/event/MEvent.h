#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

class MEvent { /// * Co the loi khi ke thua, can nghien cuu them dieu nay
public:
	using EventID = std::string;
	MEvent(void *target, const EventID &evId);
	virtual ~MEvent();

	// Getters and Setters
	void * getTarget() const { return target; }
	std::string getEventId() const { return evId; }

	void setData(void *dta) { data = dta; }
	void* getData() const { return data; }

	void stopPropagation() { stopped = true; }
	bool isStopped() const { return stopped; };
protected:
	void *target;
	void *data;
	std::string evId;

	bool stopped = false;
};

