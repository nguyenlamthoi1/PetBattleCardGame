#include "BSDeck.h"
#include "BSCard.h"
#include "data/CardData.h"
#include "data/PlayerData.h"
#include "BattleScene.h"
#include "BSResources.h"

#include "common/Utilize.h"
#include <new>

USING_NS_CC;
using namespace std;

BATTLE_SCENE_NS_BEG

BSDeck::DeckPtr BSDeck::createShPtr(Node* deck, const PlayerIdType &id) {
	auto ret = make_shared<BSDeck>(deck, id);
	if (ret && ret->init())
	return ret;
	return nullptr;
}

BSDeck::BSDeck(Node* deck, const PlayerIdType &id) : deckNode(deck), pid(id)  {}

BSDeck::~BSDeck() {
	clear();
}

bool BSDeck::init() {
	if (!deckNode || pid == PlayerIdInvalid)
		return false;

	auto btlScn = BattleScene::getScn();

	deckIcon = dynamic_cast<Sprite*>(deckNode->getChildByName("Deck_Icon"));
	if (!deckIcon)
		return false;

	deckText = dynamic_cast<ui::Text*>(deckNode->getChildByName("Deck_Text"));
	if (!deckText)
		return false;

	/*auto playerData = btlScn->getPlayerData(pid);
	auto deck = playerData->getCurDeck();
	for (auto &cardData : deck) {
		auto card = BSCard::createWithData(cardData);
		card->retain();
		cards.push_back(card);
		allCards.push_back(card);
	}*/

	auto playerData = btlScn->getPlayerData(pid);
	auto deck = playerData->getCurDeck();
	orgTotal = deck.size();
	curNum = orgTotal;
	setDeckText(orgTotal, orgTotal);
}

vector<BSCard*> BSDeck::drawTop(const std::vector<CardId> &cardVec) {
	vector<BSCard*> ret;
	auto bsres = BattleScene::getResources();
	for (const auto &cardId : cardVec)
		ret.push_back(bsres->getBSCard(cardId));

	if (curNum >= cardVec.size())
		curNum -= cardVec.size();
	else
		curNum = 0;
	setDeckText(curNum, orgTotal);
	return ret;
}

void BSDeck::clear() {
	for (auto &card : allCards) {
		card->release();
		card = nullptr;
	}
	cards.clear();
}

///Utilize functions

void BSDeck::setDeckText(unsigned int curNum, unsigned int total) {
	deckText->setString(to_string(curNum) + "/" + to_string(total));
}


BATTLE_SCENE_NS_END