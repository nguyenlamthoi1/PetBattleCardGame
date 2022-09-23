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

std::shared_ptr<Card> Hand::removeCard(unsigned int idx) {
	if (idx >= cardVec.size())
		return nullptr;

	auto ret = *(cardVec.cbegin() + idx);
	cardVec.erase(cardVec.cbegin() + idx);
	return ret;
}

std::shared_ptr<Card> Hand::getCardAt(unsigned int idx) const {
	if (idx >= cardVec.size())
		return nullptr;

	return cardVec[idx];
}


void Hand::clear() {
	cardVec.clear();
}

bool Hand::isEmpty() const {
	return cardVec.empty();
}

Hand::HandPtr Hand::clone() const {
	auto ret = make_shared<Hand>(pid);
	ret->cardVec = cardVec;
	return ret;
}

bool Hand::checkPetCard(unsigned int idx) const {
	if (idx < 0 || idx >= cardVec.size())
		return false;
	
	return cardVec[idx]->isType(Card::Type::Pet);
}



NS_GAME_END