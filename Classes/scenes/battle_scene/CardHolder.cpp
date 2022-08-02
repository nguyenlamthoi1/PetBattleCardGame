#include "CardHolder.h"
#include "GameManager.h"
#include "common/ResourcePool.h"
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

	this->setAnchorPoint(Vec2::ZERO);
	this->setContentSize(Size(95, 137));
	this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	this->setBackGroundColor(Color3B::BLUE);
	this->setBackGroundColorOpacity(255 / 2);

	auto dir = Director::getInstance();
	auto pool = GM_POOL;
	
	node = pool->tryGetNodeCsb("ccstd_csb/battle_scenes/card_holder.csb");
	if (!node)
		return false;
	this->addChild(node);
	node->setPosition(Vec2::ZERO);
	

	return true;
}

BATTLE_SCENE_NS_END