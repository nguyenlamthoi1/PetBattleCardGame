#include "Board.h"
#include "Holder.h"
#include "card/Card.h"
#include "data/CardData.h"

using namespace std;

NS_GAME_BEGIN

Board::BoardPtr Board::createShPtr(GameState *gameState, const PlayerIdType &id) {
	auto ret = make_shared<Board>(id);
	if (!ret) 
		ret.reset(); // nullptr
	ret->setGameState(gameState);
	return ret;
}

Board::Board(const PlayerIdType &id) : pid(id) {
	init();
}

Board::~Board(){}

bool Board::init() {
	active = Holder::createShPtr(pid);

	benchCapacity = GConfig::DEFAULT_BENCH_CAPACITY;
	for (int i = 0; i < benchCapacity; ++i) 
		bench.push_back(Holder::createShPtr(pid));

	return true;
}

Board::BoardPtr Board::clone() const {
	//TODO
	return nullptr;
}

void Board::setGameState(GameState* gameState) {
	active->setGameState(gameState);
	for (auto &holder : bench) 
		holder->setGameState(gameState);
}


bool Board::addBasicPetCardToActive(const std::shared_ptr<PetCard> &petCard) {
	return active->addBasicPetCard(petCard);
}

bool Board::evolvePetAtActive(const shared_ptr<PetCard> &petCard) {
	//TODO
	return false;
}

bool Board::addBasicPetCardToBench(const shared_ptr<PetCard> &petCard) {
	for (const auto &holder : bench) {
		if (!holder->hasPet()) {
			auto suc = holder->addBasicPetCard(petCard);
			if (suc) {
				//curBench += 1;
				return true;
			}
		}
	}
	return false;
}

bool Board::isBenchFull() const {
	//return curBench >= benchCapacity;
	for (const auto &holder : bench) {
		if (!holder->hasPet()) {
			return false;
		}
	}
	return true;
}

bool Board::evolvePetAtBench(const std::shared_ptr<PetCard> &petCard, unsigned int benchIdx) {
	//TODO
	return false;
}

bool Board::hasActivePet() const {
	return active->hasPet();
}

bool Board::benchHasPet() const {
	bool ret = false;
	for (const auto &holder : bench) {
		if (holder->hasPet())
		{
			ret = true;
			break;
		}
	}
	return ret;
}


vector<Board::HolderPtr> Board::getAllHolders() const {
	vector<HolderPtr> ret;
	ret.push_back(active);
	ret.insert(ret.cend(), bench.cbegin(), bench.cend());
	return ret;
}

Board::HolderPtr Board::getBenchHolder(unsigned int idx) const {
	if (idx >= bench.size())
		return nullptr;
	return bench.at(idx);
}


NS_GAME_END