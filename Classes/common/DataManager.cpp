#include "DataManager.h"
#include "data/CardData.h"
#include "data/MoveData.h"
#include "data/PlayerData.h"
#include "GameManager.h"

#include "include/rapidjson/rapidjson.h"
#include "include/rapidjson/document.h"
#include "cocos2d.h"

//#ifdef _MSC_VER
//#undef GetObject
//#endif

using namespace rapidjson;
USING_NS_CC;


DataManager::DataManager() {

}

DataManager::~DataManager() {
	CCLOG("DataManager:: dtor called");
	clear();
}

bool DataManager::loadEnergyCards() {
	Document doc;
	ssize_t size;
	std::string fileName = "data/energy_cards.json";
	auto fileStr = FileUtils::getInstance()->getStringFromFile(fileName);
	doc.Parse(fileStr.c_str());

	CCASSERT(doc.IsObject(), "loadEnergyCards:: doc is not Object");
	for (auto itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr) {
		auto key = itr->name.GetString();
		auto val = itr->value.GetObj();

		CCASSERT(cardMap.find(key) == cardMap.end(), "loadEnergyCards:: duplicate key");

		auto cardId = key;
		auto cardName = val["Name"].GetString();
		auto cardType = val["Type"].GetString();
		auto cardNum = val["Num"].GetUint();
		auto cardImage = val["Image"].GetString();
		
		cardMap.insert({ cardId, std::make_shared<EnergyCardData>(cardId, cardName, cardType, cardNum, cardImage) });
	}

	return true;
}

bool DataManager::loadPetCards() {
	Document doc;
	ssize_t size;
	std::string fileName = "data/pet_cards.json";
	auto fileStr = FileUtils::getInstance()->getStringFromFile(fileName);
	doc.Parse(fileStr.c_str());

	CCASSERT(doc.IsObject(), "loadPetCards:: doc is not Object");
	for (auto itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr) {
		auto key = itr->name.GetString();
		auto val = itr->value.GetObj();

		CCASSERT(cardMap.find(key) == cardMap.end(), "loadPetCards:: duplicate key");

		auto cardId = key;
		auto data = new PetCardData(cardId);

		// Compulsory data
		data->type = val["Type"].GetString();
		data->name = val["Name"].GetString();
		data->hp = val["Hp"].GetUint();
		data->petImg = val["Image"].GetString();
		data->outerImg = val["OuterImage"].GetString();

		// Moves
		for (auto& v : val["Moves"].GetArray()) {
			auto obj = v.GetObj();
			auto moveData = new MoveData();
			moveData->name = obj["Name"].GetString();
			moveData->damageText = obj["DamageText"].GetString();
			moveData->content = obj["ContentText"].GetString();

			// Cost
			for (auto& it : obj["Cost"].GetObj())
				moveData->costMap.insert({ it.name.GetString(), it.value.GetUint() });
			
			data->moveVec.emplace_back(moveData);
		}

		rapidjson::Value::ConstMemberIterator foundItr;

		// Resistance cost(optional)
		foundItr = val.FindMember("Resistance");
		if (foundItr != val.MemberEnd()) 
			for (auto& it : foundItr->value.GetObj())
				data->resistanceMap.insert({ it.name.GetString(), it.value.GetInt() });

		// Weakness(optional)
		foundItr = val.FindMember("Weakness");
		if (foundItr != val.MemberEnd())
			for (auto& v : foundItr->value.GetArray())
				data->weakSet.insert(v.GetString());
	
		// Retreat cost(optional)
		foundItr = val.FindMember("Retreat");
		if (foundItr != val.MemberEnd())
			for (auto& it : foundItr->value.GetObj())
				data->retreatMap.insert({ it.name.GetString(), it.value.GetUint() });

		// Evolve(optional)
		foundItr = val.FindMember("Evolve");
		if (foundItr != val.MemberEnd()) {
			auto evObj = foundItr->value.GetObj();
			data->evolveFrom = evObj["FromName"].GetString();
			data->preEvImg = evObj["Image"].GetString();
			data->evStage = evObj["Stage"].GetUint();
		}


		auto insRet = cardMap.insert({ cardId, std::shared_ptr<PetCardData>(data) });
	}

	return true;
}

bool DataManager::loadPlayerData() {
	player.reset(new PlayerData());

	Document doc;
	ssize_t size;
	std::string fileName = "data/player_data.json";
	auto fileStr = FileUtils::getInstance()->getStringFromFile(fileName);
	doc.Parse(fileStr.c_str());

	CCASSERT(doc.IsObject(), "loadPlayerData:: doc is not Object");

	rapidjson::Value::ConstMemberIterator foundItr;

	auto val = doc.GetObj();

	// Deck List
	foundItr = val.FindMember("DeckList");
	if (foundItr != val.MemberEnd())
		for (auto &v : foundItr->value.GetArray()) {
			player->deckList.push_back(PlayerData::DeckMap());
			auto &deckVec = player->deckList.back();
			for (auto &v : v.GetObj()) 
				deckVec.insert({ v.name.GetString(), v.value.GetUint() });
		}

	foundItr = val.FindMember("CurrentDeckIdx");
	if (foundItr != val.MemberEnd())
		player->curDeck = foundItr->value.GetUint();

	// Owned cards
	foundItr = val.FindMember("OwnedCards");
	if (foundItr != val.MemberEnd()) {
		for (auto &v : foundItr->value.GetObj()) {
			auto cardId = v.name.GetString();
			auto cardNum = v.value.GetUint();
			auto foundIt = player->ownedCards.find(cardId);

			if (foundIt != player->ownedCards.cend())   // Found
				foundIt->second += cardNum;
			else //Not Found
				player->ownedCards.insert({ cardId, cardNum });
		}
	}

	// Coin list
	foundItr = val.FindMember("CoinList");
	if (foundItr != val.MemberEnd())
		for (auto &v : foundItr->value.GetArray()) 
			player->coins.insert(v.GetString());

	foundItr = val.FindMember("UsedCoin");
	if (foundItr != val.MemberEnd())
		player->usedCoin = foundItr->value.GetString();
	
	return true;
}


void DataManager::clear() {
	cardMap.clear();
}

std::shared_ptr<const CardData> DataManager::getCardData(CardId id) {
	auto foundIt = cardMap.find(id);
	if (foundIt != cardMap.cend()) 
		return foundIt->second;
	return nullptr;
}
