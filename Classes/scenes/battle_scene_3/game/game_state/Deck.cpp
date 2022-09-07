#include "Deck.h"

#include "define/CommonDefine.h"
#include "data/CardData.h"
#include "card/Card.h"

#include <algorithm>
#include <random>
#include <ctime>

#include "cocos2d.h"

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
		popNum = cardVec.size();

	CardVector ret;
	ret.insert(ret.cend(), cardVec.crbegin() , cardVec.crbegin() + popNum);
	cardVec.erase(cardVec.cend() - popNum, cardVec.cend());
	return ret;
}

Deck::CardVector Deck::popBottom(unsigned int popNum) {
	if (popNum > cardVec.size())
		popNum = cardVec.size();
	
	CardVector ret;
	ret.insert(ret.cend(), cardVec.cbegin(), cardVec.cbegin() + popNum);
	cardVec.erase(cardVec.cbegin(), cardVec.cbegin() + popNum);
	return ret;
}

void Deck::popTop(unsigned int popNum, CardVector &v) {
	if (popNum > cardVec.size())
		popNum = cardVec.size();
	v.insert(v.cend(), cardVec.crbegin(), cardVec.crbegin() + popNum);
	cardVec.erase(cardVec.cend() - popNum, cardVec.cend());
}

void Deck::popTop(unsigned int popNum, vector<std::string> &cidVec) {
	if (popNum > cardVec.size())
		popNum = cardVec.size();

	auto itrBeg = cardVec.crbegin();
	auto itrEnd = cardVec.crbegin() + popNum;
	while (itrBeg != itrEnd) {
		cidVec.emplace_back((*itrBeg)->getId());
		++itrBeg;
	}
	
	cardVec.erase(cardVec.cend() - popNum, cardVec.cend());
}

void Deck::popTopWithABasicPet(unsigned int popNum, std::vector<std::string> &cidVec) {
	if (popNum > cardVec.size())
		popNum = cardVec.size();
	
	CardVector temp;

	auto itrBeg = cardVec.begin();
	auto itrEnd = cardVec.end();
	while (itrBeg != itrEnd) {
		auto card = *itrBeg;
		if (card->isType(Card::Type::Pet)) {
			cidVec.emplace_back(card->getId());
			cardVec.erase(itrBeg);
			popNum -= 1;
			break;
		}
		++itrBeg;
	}

	popTop(popNum, cidVec);
}

void Deck::popTopWithABasicPet(unsigned int popNum, CardVector &cVec) {
	if (popNum > cardVec.size())
		popNum = cardVec.size();

	CardVector temp;

	auto itrBeg = cardVec.begin();
	auto itrEnd = cardVec.end();
	while (itrBeg != itrEnd) {
		auto card = *itrBeg;
		if (card->isType(Card::Type::Pet)) {
			cVec.emplace_back(card);
			cardVec.erase(itrBeg);
			popNum -= 1;
			break;
		}
		++itrBeg;
	}

	popTop(popNum, cVec);
}

void Deck::popBottom(unsigned int popNum, CardVector &v) {
	if (popNum > cardVec.size())
		popNum = cardVec.size();

	v.insert(v.cend(), cardVec.cbegin(), cardVec.cbegin() + popNum);
	cardVec.erase(cardVec.cbegin(), cardVec.cbegin() + popNum);
}

void Deck::popBottom(unsigned int popNum, vector<string> &cidVec) {
	if (popNum > cardVec.size())
		popNum = cardVec.size();

	auto itrBeg = cardVec.cbegin();
	auto itrEnd = cardVec.cbegin() + popNum;
	while (itrBeg != itrEnd) {
		cidVec.emplace_back((*itrBeg)->getId());
		++itrBeg;
	}

	cardVec.erase(cardVec.cbegin(), cardVec.cbegin() + popNum);
}

/// Getters, Setters & Checker
bool Deck::isEmpty() {
	return cardVec.empty();
}


Deck::DeckPtr Deck::clone() const {
	//TODO
	return nullptr;
}

void Deck::shuffleDeck() {
	//default_random_engine dre(time(0));
	default_random_engine dre;
	std::shuffle(std::begin(cardVec), std::end(cardVec), dre);
}


NS_GAME_END