#include "CardHolder.h"
#include <new>

using namespace std;
USING_NS_CC;
BATTLE_SCENE_NS_BEG

CardHolder::CardHolder() {

}

CardHolder::~CardHolder() {

}

CardHolder* CardHolder::create() {
	auto holder = new (nothrow) CardHolder();
	if (holder && holder->init()) {
		holder->autorelease();
		return holder;
	}
	else {
		delete holder;
		holder = nullptr;
	}
	return holder;
}

bool CardHolder::init() {
	if (!ui::Layout::init())
		return false;

	

	return true;
}

BATTLE_SCENE_NS_END