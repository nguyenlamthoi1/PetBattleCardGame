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

PrizePile::PrizePile(const PlayerIdType &id) : pid(id){
	
	selected.reserve(GConfig::PRIZE_CARDS_NUM_PER_PLAYER);
}

PrizePile::~PrizePile() {}

bool PrizePile::init() { return true; }

void PrizePile::pushCards(const vector<shared_ptr<Card>> &pushedCards) {
	for (size_t i = 0; i < cardVec.size(); ++i) {
		if (i < pushedCards.size())
			cardVec[i] = pushedCards[i];
		else
			break;
	}
}

void PrizePile::pushCard(const shared_ptr<Card> &pushedCard) {
	//cardVec.push_back(pushedCard);;
}

void PrizePile::clear() {
	//cardVec.clear();
}

/// Getters, Setters & Checker
bool PrizePile::empty() const {
	for (const auto &card : cardVec) {
		if (card)
			return false;
	}
	return true;
}

unsigned int PrizePile::getCurCardsInPile() const {
	unsigned int ret = 0;
	for (const auto &card : cardVec) {
		if (card)
			ret += 1;
	}
	return ret;
}



PrizePile::PrizePtr PrizePile::clone() const {
	auto ret = make_shared<PrizePile>(pid);
	ret->cardVec = cardVec;
	ret->selected = selected;
	return ret;
}


bool PrizePile::canGetCardsAt(const vector<unsigned int> &idxVec) const {
	for (const auto &idx : idxVec) {
		if (idx < 0 && idx >= cardVec.size() && !cardVec[idx])
			return false;
	}
	return true;
}

vector<unsigned int> PrizePile::getFirstSelectables(unsigned int num) const {
	vector<unsigned int> ret;
	unsigned int selectedNum = 0;
	for (unsigned int i = 0; i < cardVec.size(); ++i) {
		if (cardVec[i]) {
			ret.push_back(i);
			++selectedNum;
			if (selectedNum >= num)
				break;
		}
	}
	return ret;
}

void PrizePile::saveSelectedCardsAt(const vector<unsigned int> &idxVec) {
	selected.clear();
	for (const auto &idx : idxVec) {
		if (0 <= idx && idx < cardVec.size() && cardVec[idx])
			selected.push_back(idx);
	}
}

void PrizePile::popCards(const vector<PrizeIdx> &idxVec, const vector<shared_ptr<Card>> &retVec) {
	//TODO
}

vector<shared_ptr<Card>> PrizePile::popCards(const vector<PrizeIdx> &idxVec) {
	vector<shared_ptr<Card>> ret;
	//TODO
	return ret;
}

void PrizePile::popSelectedCards(vector<shared_ptr<Card>> &retVec) {
	for (const auto &idx : selected) {
		if (0 <= idx && idx < cardVec.size() && cardVec[idx]) {
			retVec.push_back(cardVec[idx]);
			cardVec[idx].reset(); // reset to nullptr
		}
	}
}
vector<shared_ptr<Card>> PrizePile::popSelectedCards() {
	vector<shared_ptr<Card>> retVec;
	for (const auto &idx : selected) {
		if (0 <= idx && idx < cardVec.size() && cardVec[idx]) {
			retVec.push_back(cardVec[idx]);
			cardVec[idx].reset(); // reset to nullptr
		}
	}
	return retVec;
}

NS_GAME_END