#include "PlayerData.h"
#include "CardData.h"
using namespace std;

PlayerData* PlayerData::createPseudo(bool isPlayer) {
	auto data = new PlayerData();
	data->decks.push_back({});
	return data;
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
	return !decks.empty() ? decks[curDeck] : DeckList();
}




