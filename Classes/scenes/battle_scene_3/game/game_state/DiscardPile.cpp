#include "DiscardPile.h"

#include "define/CommonDefine.h"
#include "data/CardData.h"
#include "card/Card.h"

using namespace std;

NS_GAME_BEGIN

DiscardPile::DiscardPtr DiscardPile::createShPtr(const PlayerIdType &id) {
	auto ret = make_shared<DiscardPile>(id);
	if (!ret || !ret->init())
		ret.reset(); // nullptr
	return ret;
}

DiscardPile::DiscardPile(const PlayerIdType &id) : pid(id) {
	cardVec.reserve(GConfig::CARD_NUM_IN_DECK_MAX);
}

DiscardPile::~DiscardPile() {}

bool DiscardPile::init() { return true; }

void DiscardPile::pushCards(const CardVector &pushedCards) {
	cardVec.insert(cardVec.cend(), pushedCards.cbegin(), pushedCards.cend());
}

void DiscardPile::pushCard(const std::shared_ptr<Card> &pushedCard) {
	cardVec.push_back(pushedCard);;
}

void DiscardPile::clear() {
	cardVec.clear();
}

/// Getters, Setters & Checker
bool DiscardPile::empty() const {
	return cardVec.empty();
}


DiscardPile::DiscardPtr DiscardPile::clone() const {
	//TODO
	return nullptr;
}

void DiscardPile::popCards(std::initializer_list<DiscardIdx> idxs, CardVector &cVec) {
	
}



NS_GAME_END