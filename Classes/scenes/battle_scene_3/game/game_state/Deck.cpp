#include "Deck.h"

#include "define/CommonDefine.h"
#include "data/CardData.h"

using namespace std;

NS_GAME_BEGIN

Deck::DeckPtr Deck::createShPtr(const PlayerIdType &id) {
	auto ret = make_shared<Deck>(id);
	if (!ret || !ret->init())
		ret.reset(); // nullptr
	return ret;
}


Deck::Deck(const PlayerIdType &id) : pid(id) {
	cardVec.reserve(GConfig::CARD_NUM_IN_DECK_MAX);
}

Deck::~Deck(){}

bool Deck::init() { return true; }

void Deck::pushCards(const CardVector &pushedCards) {
	cardVec.insert(cardVec.cend(), pushedCards.cbegin(), pushedCards.cend());
}

void Deck::pushCard(const std::shared_ptr<Card> &pushedCard) {
	cardVec.push_back(pushedCard);;
}

void Deck::clear() {
	cardVec.clear();
}

Deck::CardVector Deck::popTop(unsigned int popNum) {
	if (popNum > cardVec.size())
		return {};

	CardVector ret;
	ret.insert(ret.cend(), cardVec.crbegin() , cardVec.crbegin() + popNum);
	cardVec.erase(cardVec.cend() - popNum, cardVec.cend());
	return ret;
}

Deck::CardVector Deck::popBottom(unsigned int popNum) {
	if (popNum > cardVec.size())
		return {};
	
	CardVector ret;
	ret.insert(ret.cend(), cardVec.cbegin(), cardVec.cbegin() + popNum);
	cardVec.erase(cardVec.cbegin(), cardVec.cbegin() + popNum);
	return ret;
}

void Deck::popTop(unsigned int popNum, CardVector &v) {
	if (popNum > cardVec.size())
		return;
	v.insert(v.cend(), cardVec.crbegin(), cardVec.crbegin() + popNum);
	cardVec.erase(cardVec.cend() - popNum, cardVec.cend());
}

void Deck::popBottom(unsigned int popNum, CardVector &v) {
	if (popNum > cardVec.size())
		return;

	v.insert(v.cend(), cardVec.cbegin(), cardVec.cbegin() + popNum);
	cardVec.erase(cardVec.cbegin(), cardVec.cbegin() + popNum);
}

/// Getters, Setters & Checker
bool Deck::isEmpty() {
	return cardVec.empty();
}


NS_GAME_END