#include "CardHolder.h"
#include "GameManager.h"
#include "common/ResourcePool.h"
#include <new>

using namespace std;
USING_NS_CC;
BATTLE_SCENE_NS_BEG

Size CardHolder::HOLDER_SIZE = Size(95, 137);

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
	this->setBackGroundColorOpacity(255 / 3);
	auto dir = Director::getInstance();
	auto pool = GM_POOL;
	
	node = pool->tryGetNodeCsb("ccstd_csb/battle_scene/card_holder.csb");
	if (!node)
		return false;
	this->addChild(node);
	node->setPosition(Vec2::ZERO);
	
	auto layout = node->getChildByName("PanelHolder");
	this->setContentSize(layout->getContentSize());

	return true;
}

void CardHolder::setHolderSizeH(float h) {
	auto layout = node->getChildByName("PanelHolder");
	auto layoutSize = layout->getContentSize();
	auto thisSize = getContentSize();
	auto s = h / thisSize.height;

	auto newThisSize = Size(thisSize.width * s, thisSize.height * s);
	setContentSize(newThisSize);
	node->setScale(s);
}
void CardHolder::setHolderSizeW(float w) {
	auto layout = node->getChildByName("PanelHolder");
	auto layoutSize = layout->getContentSize();
	auto thisSize = getContentSize();
	auto s = w / thisSize.width;

	auto newThisSize = Size(thisSize.width * s, thisSize.height * s);
	setContentSize(newThisSize);
	node->setScale(s);
}


BATTLE_SCENE_NS_END