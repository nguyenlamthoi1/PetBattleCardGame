#include "Board.h"
#include "Holder.h"

using namespace std;

NS_GAME_BEGIN

Board::BoardPtr Board::createShPtr(const PlayerIdType &id) {
	auto ret = make_shared<Board>(id);
	if (!ret || !ret->init()) 
		ret.reset(); // nullptr
	return ret;
}

Board::Board(const PlayerIdType &id) : pid(id) {}

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



NS_GAME_END