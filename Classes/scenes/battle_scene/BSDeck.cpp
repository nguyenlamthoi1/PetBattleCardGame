#include "BSDeck.h"
#include "BSCard.h"
#include "BattleScene.h"
#include "common/Utilize.h"
#include <new>

USING_NS_CC;
using namespace std;

BATTLE_SCENE_NS_BEG

BSDeck* BSDeck::create(BattleScene *scn, PlayerIdType id) {
	auto deck = new (nothrow) BSDeck(scn, id);
	if (deck && deck->init()) 
		return deck;
	delete deck;
	deck = nullptr;
	return deck;
}

BSDeck::BSDeck(BattleScene *scn, PlayerIdType id) : btlScn(scn), ownerId(id)  {

}

BSDeck::~BSDeck() {
	clear();
}

bool BSDeck::init() {
	if (!btlScn || ownerId == PlayerIdInvalid)
		return false;

	deckIcon = dynamic_cast<Sprite*>(Utilize::getChildByName(btlScn->ui, ""));
	if (!deckIcon)
		return false;

	deckText = dynamic_cast<ui::Text*>(Utilize::getChildByName(btlScn->ui, ""));
	if (!deckText)
		return false;

	auto deck = btlScn->playerData[ownerId]->getCurDeck();
	for (auto &cardData : deck) {
		auto card = BSCard::createWithData(cardData);
		card->retain();
		cards.push_back(nullptr);
	}

	for (size_t i = 0; i < 60; i++) {
		auto card = BSCard::createWithData(nullptr);
		card->retain();
		cards.push_back(nullptr);
	}
}

vector<BSCard*> BSDeck::drawTop(size_t n) {
	vector<BSCard*> ret;

	if (cards.size() < n)
		return ret; // Return empty

	for (auto it = cards.cbegin(); it != cards.cbegin() + n; ++it) {
		auto &card = (*it);
		card->release();
		ret.push_back(card);
	}
	cards.erase(cards.cbegin(), cards.cbegin() + n);
	return ret;
}

std::vector<BSCard*>::iterator BSDeck::drawTop(size_t n, std::vector<BSCard*> &v) {
	auto retIt = v.end();
	if (cards.size() < n)
		return retIt; // Inserting failed

	retIt = cards.begin();
	for (auto it = cards.cbegin(); it != cards.cbegin() + n; ++it) {
		auto &card = (*it);
		card->release();
		v.push_back(card);
	}
	cards.erase(cards.cbegin(), cards.cbegin() + n);
	return retIt;
}


void BSDeck::clear() {
	for (auto &card : cards) {
		card->release();
		card = nullptr;
	}
	cards.clear();
}

BATTLE_SCENE_NS_END