#include "CoinFlipper.h"

#include <algorithm>
#include <random>
#include <ctime>

#include "cocos2d.h"

using namespace std;

NS_GAME_BEGIN

CoinFlipper::CFlipperPtr CoinFlipper::createShPtr(const PlayerIdType &id) {
	auto ret = make_shared<CoinFlipper>(id);
	if (!ret || !ret->init())
		ret.reset(); // nullptr
	return ret;
}

CoinFlipper::CoinFlipper(const PlayerIdType &id) : pid(id) {
	resVec.reserve(10);
}

CoinFlipper::~CoinFlipper() {}

bool CoinFlipper::init() { return true; }

void CoinFlipper::clear() {
	resVec.clear();
}

CoinFlipper::CFlipperPtr CoinFlipper::clone() const {
	auto ret = make_shared<CoinFlipper>(pid);
	ret->resVec = resVec;
	return ret;
}

vector<CoinFlipper::SideType> CoinFlipper::doFlip(unsigned int flipCount) {
	vector<CoinFlipper::SideType> ret;
	
	int num = 0;
	while (0 < flipCount) {
		auto randomSide = cocos2d::RandomHelper::random_int(0, 1);
		resVec.push_back(randomSide);
		++num;
	}

	return ret;
}


NS_GAME_END