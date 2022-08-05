#ifndef __MOVE_DATA_H__
#define __MOVE_DATA_H__

#include <string>
#include <unordered_map>
#include <vector>
#include <map>

class MoveData {
public:
	using EnergyType = std::string;
	std::string name;
	std::string damageText;
	std::string content;
	std::map<EnergyType, unsigned int> costMap;

	bool isEmpty() { return name.empty(); }
};

#endif // __MOVE_DATA_H__