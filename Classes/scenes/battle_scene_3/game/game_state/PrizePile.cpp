#include "PrizePile.h"

#include "define/CommonDefine.h"
#include "data/CardData.h"
#include "card/Card.h"

using namespace std;

NS_GAME_BEGIN

PrizePile::PrizePtr PrizePile::createShPtr(const PlayerIdType &id) {
	auto ret = make_shared<PrizePile>(id);
	if (!ret || !ret->init())
		ret.reset(); // nullptr
	return ret;
}

PrizePile::PrizePile(const PlayerIdType &id) : pid(id) {
	cardVec.reserve(GConfig::CARD_NUM_IN_DECK_MAX);
}

PrizePile::~PrizePile() {}

bool PrizePile::init() { return true; }

void PrizePile::pushCards(const CardVector &pushedCards) {
	cardVec.insert(cardVec.cend(), pushedCards.cbegin(), pushedCards.cend());
}

void PrizePile::pushCard(const std::shared_ptr<Card> &pushedCard) {
	cardVec.push_back(pushedCard);;
}

void PrizePile::clear() {
	cardVec.clear();
}

/// Getters, Setters & Checker
bool PrizePile::empty() const {
	return cardVec.empty();
}


PrizePile::PrizePtr PrizePile::clone() const {
	//TODO
	return nullptr;
}

void PrizePile::popCards(std::initializer_list<PrizeIdx> idxs, CardVector &cVec) {

}



NS_GAME_END