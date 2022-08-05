#include "PlayerData.h"
#include "CardData.h"
#include "GameManager.h"
#include "common/DataManager.h"

using namespace std;

shared_ptr<PlayerData> PlayerData::createPseudo(bool isPlayer) {
	auto data = new PlayerData();
	data->decks.push_back({});
	return shared_ptr<PlayerData>(data);
}


PlayerData::PlayerData() : curDeck(0) {

}

PlayerData::~PlayerData() {

}

bool PlayerData::loadDataFromFile(const string &file) {
	// TODO
	return true;
}

PlayerData::DeckList PlayerData::getCurDeck() const{
	DeckList ret;
	auto dataMgr = GM_DATA_MGR;
	for (const auto &it : deckList[curDeck]) {
		int i = 0;
		int num = it.second;
		auto cardData = dataMgr->getCardData(it.first);
		while (i < num) {
			ret.push_back(cardData);
			++i;
		}
	}
	return ret;
}




