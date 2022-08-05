#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

#include "define/CommonDefine.h"

#include <string>
#include <unordered_map>
#include <vector>

class CardData;
class PlayerData;

class DataManager final{
public:
	enum class CardType {
		Pokemon,
		Energy,
		Supporter,
		Item
	};
	using CardId = std::string;
	using CardList = std::unordered_map<CardId, CardData>;

	DataManager();
	~DataManager();

	bool loadEnergyCards();
	bool loadPetCards();
	bool loadPlayerData();

	std::shared_ptr<PlayerData> getPlayerData() { return player; }
	std::shared_ptr<const CardData> getCardData(CardId id);
private:
	void clear();

	std::unordered_map<CardId, std::shared_ptr<CardData>> cardMap;
	std::shared_ptr<PlayerData> player;
};

#endif // __DATA_MANAGER_H__