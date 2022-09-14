#include "CardSelector.h"

#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/LangSys.h"
#include "components/WidgetTouchComponent.h"
#include "define/CommonDefine.h"
#include "scenes/battle_scene_3/BattleScene.h"
#include "ui/UIHelper.h"

#include <new>
#include <algorithm>

USING_NS_CC;
WIDGET_TOUCH_USE_NS;
//BATTLE_SCENE_USE_NS;
using namespace std;

shared_ptr<CardSelector> CardSelector::create(Node *rootnode) {
	auto ret = make_shared<CardSelector>(rootnode);
	if (ret && ret->init())
		return ret;
	return nullptr;
}

CardSelector::CardSelector(cocos2d::Node *rootnode) : root(rootnode) {}

CardSelector::~CardSelector(){}

bool CardSelector::init() {
	auto director = Director::getInstance();
	auto winSize = director->getWinSize();
	auto gm = GameManager::getInstance();
	auto resPool = gm->getPool();
	
	if (!root)
		return false;
	
	unselectedPanel = dynamic_cast<ui::Layout*>(root->getChildByName("Unselected_Panel"));
	if (!unselectedPanel)
		return false;

	unselectedBtnNext = dynamic_cast<ui::Button*>(unselectedPanel->getChildByName("Next_Button"));
	unselectedBtnPrev = dynamic_cast<ui::Button*>(unselectedPanel->getChildByName("Prev_Button"));
	unselectedContainer = dynamic_cast<ui::Layout*>(unselectedPanel->getChildByName("Container"));
	unselectedTouchPanel = dynamic_cast<ui::Layout*>(unselectedPanel->getChildByName("Touch_Panel"));

	// *CACH 2

	auto handlerFunc = [this](Ref *sender, ui::Widget::TouchEventType eventType)->void {
		switch (eventType) {
		case ui::Widget::TouchEventType::BEGAN:
		{
			CCLOG("Touch began");
			break;
		}
		case ui::Widget::TouchEventType::ENDED:
		case ui::Widget::TouchEventType::CANCELED:
		{
			CCLOG("Touch ended");
			break;
		}
		case ui::Widget::TouchEventType::MOVED:
		{
			break;
		}
		};
	};
	unselectedTouchPanel->addTouchEventListener(handlerFunc);

	return true;
}

bool CardSelector::onUnselectedTouchBegan(Touch * touch, Event* event) {
	CCLOG("Touch Began");
	return true;
}

void CardSelector::onUnselectedTouchEnded(Touch * touch, Event* event) {
	CCLOG("Touch Ended");

}

void CardSelector::onUnselectedTouchMoved(Touch * touch, Event* event) {
	CCLOG("Touch Moved");
}

void CardSelector::onUnselectedTouchCancelled(Touch * touch, Event* event) {
	CCLOG("Touch Cancelled");
}