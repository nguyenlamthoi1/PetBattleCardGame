#ifndef __CARD_DATA_H__
#define __CARD_DATA_H__

#include "define/CommonDefine.h"
#include <string>
#include <unordered_map>
#include <map>
#include <set>
#include <vector>
#include <memory>

class MoveData;

class CardData{
public:
	static bool checkSelectType(GConfig::SelectType type, const std::shared_ptr<const CardData> &data);
	
	using CardId = std::string;
	enum class Type {
		None,
		Pet,
		Energy,
		Supporter,
		Item
	};
	virtual ~CardData();
	bool isValid() const { return !id.empty(); }

	CardId id;
	Type type = Type::None;
protected:
	CardData(CardId id, Type t);
};

class PetCardData final : public CardData{
public:
	using PetType = std::string;
	using EnergyType = std::string;

	PetCardData(CardId id = "");
	~PetCardData();

	PetType type;
	std::string name;
	size_t hp = 0;
	std::string petImg;
	std::string outerImg;
	std::map<EnergyType, unsigned int> retreatMap;
	std::set<PetType> weakSet;
	std::map<EnergyType, int> resistanceMap;
	std::vector<std::shared_ptr<MoveData>> moveVec;
	
	bool isEvolutionCard() const { return evStage > 0; }
	bool isBasicCard() const { return evStage < 1; }
	
	std::string evolveFrom;

	using EVStage = unsigned int;
	using PetName = unsigned int;
	std::unordered_map<EVStage, PetName> evolutionLine;
	
	std::string preEvImg;
	unsigned char evStage = 0;
	unsigned int prizeNum = 1;
};

class EnergyCardData final : public CardData {
public:
	using EnergyType = std::string;
	using EnergyNumType = unsigned char;
	EnergyCardData(CardId id, std::string name, std::string type, unsigned char num, std::string img);
	~EnergyCardData();

	std::string eName;
	EnergyType eType = CardDefine::Energy::None;
	EnergyNumType eNum = 1;
	std::string image = "";
};

#endif // __CARD_DATA_H__