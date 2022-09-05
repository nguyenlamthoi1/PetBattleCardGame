#include "Hand.h"
#include "card/Card.h"

using namespace std;

NS_GAME_BEGIN

Hand::HandPtr Hand::createShPtr(const PlayerIdType &id) {
	auto ret = make_shared<Hand>(id);
	if (!ret || !ret->init())
		ret.reset(); // nullptr
	return ret;
}

Hand::Hand(const PlayerIdType &id) : pid(id) {}

Hand::~Hand(){}

bool Hand::init() { return true; }

void Hand::pushCards(const CardVector &pushedCards) {
	cardVec.insert(cardVec.cend(), pushedCards.cbegin(), pushedCards.cend());
}

void Hand::pushCard(const std::shared_ptr<Card> &pushedCard) {
	cardVec.push_back(pushedCard);
}

void Hand::clear() {
	cardVec.clear();
}

bool Hand::isEmpty() const {
	return cardVec.empty();
}



NS_GAME_END