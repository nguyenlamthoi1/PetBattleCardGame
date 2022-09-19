#ifndef __MOVE_DATA_H__
#define __MOVE_DATA_H__

#include <string>
#include <unordered_map>
#include <vector>
#include <map>

struct ActionData {
	std::string id;
	std::unordered_map<std::string , int> ovrValues;
};

class MoveData {
public:
	using EnergyType = std::string;
	std::string name;
	std::string damageText;
	std::string content;
	std::map<EnergyType, unsigned int> costMap;
	std::vector<std::shared_ptr<ActionData>> actions;

	bool isEmpty() { return name.empty(); }
};

#endif // __MOVE_DATA_H__