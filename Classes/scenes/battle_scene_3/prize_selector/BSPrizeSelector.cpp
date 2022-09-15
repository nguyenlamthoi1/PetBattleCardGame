#include "BSPrizeSelector.h"

#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/LangSys.h"
#include "ui/UIHelper.h"

#include "scenes/battle_scene_3/BattleScene.h"
#include "scenes/battle_scene_3/BSCard.h"
#include "scenes/battle_scene_3/BSResources.h"
#include "scenes/battle_scene_3/BSPrizePile.h"

#include "scenes/battle_scene_3/game/player_actions/PlayerAction.h"


USING_NS_CC;
//BATTLE_SCENE_USE_NS;
using namespace std;

BATTLE_SCENE_NS_BEG

// BSPrizeHolder Class //

shared_ptr<BSPrizeHolder> BSPrizeHolder::create(Node * rootnode) {
	auto ret = make_shared<BSPrizeHolder>(rootnode);
	if (ret && ret->init())
		return ret;
	return nullptr;
}
BSPrizeHolder::BSPrizeHolder(cocos2d::Node *rootnode) : root(rootnode) {}
BSPrizeHolder::~BSPrizeHolder() {

}

bool BSPrizeHolder::init() {
	if (!root)
		return false;

	container = dynamic_cast<ui::Layout*>(root->getChildByName("Container"));
	cardMarker = container->getChildByName("Card_Marker");

	selectBorder = dynamic_cast<ui::ImageView*>(container->getChildByName("Select_Border"));
	selectBorder->setVisible(false);

	auto func = [this](Ref *sender, ui::Widget::TouchEventType eventType)->void {
		switch (eventType) {
		case ui::Widget::TouchEventType::ENDED:
			selected = !selected;
			selectBorder->setVisible(selected);
			break;
		}
	};

	
	touchPanel = dynamic_cast<ui::Layout*>(container->getChildByName("Touch_Panel"));
	touchPanel->addTouchEventListener(func);

	return true;
}

void BSPrizeHolder::addCard(BSCard *card) {
	bsCard = card;
	bsCard->stopAllActions();
	bsCard->removeFromParent();
	cardMarker->addChild(bsCard);
	bsCard->setScale(1.0f);
	bsCard->setPosition(Vec2(0, 0));
}

BSCard* BSPrizeHolder::removeCard() {
	auto ret = bsCard;
	if (bsCard) {
		bsCard->removeFromParent();
		selected = false;
		selectBorder->setVisible(selected);
		bsCard = nullptr;
	}
	return ret;
}


//---------------------//


shared_ptr<BSPrizeSelector> BSPrizeSelector::create(BattleScene *scn) {
	auto ret = make_shared<BSPrizeSelector>(scn);
	if (ret && ret->init())
		return ret;
	return nullptr;
}

BSPrizeSelector::BSPrizeSelector(BattleScene *scn) : btlScn(scn) {
	holderVec.reserve(GConfig::PRIZE_CARDS_NUM_PER_PLAYER);
}

BSPrizeSelector::~BSPrizeSelector(){}

bool BSPrizeSelector::init() {
	auto director = Director::getInstance();
	auto winSize = director->getWinSize();
	
	if (!btlScn)
		return false;

	auto scnroot = btlScn->getScnRoot();
	root = scnroot->getChildByName("Prize_Selector");
	//root = dynamic_cast<ui::Layout*>(scnroot->getChildByName("Prize_Selector")); // P1_Panel or P2_Panel
	if (!root)
		return false;

	container = dynamic_cast<ui::Layout*>(root->getChildByName("Container"));
	auto idx = 0;
	for (const auto &holderNode : container->getChildren()) {
		auto holder = BSPrizeHolder::create(holderNode);
		holder->idx = idx;
		holderVec.push_back(holder);
		++idx;
	}

	doneBtn = dynamic_cast<ui::Button*>(root->getChildByName("DoneButton"));
	doneBtn->addClickEventListener(bind(&BSPrizeSelector::onDoneBtnTouched, this, std::placeholders::_1));
	doneBtn->setVisible(false);

	showMatchBtn = dynamic_cast<ui::Button*>(root->getChildByName("ShowMatchBtn"));
	showMatchBtn->addClickEventListener(bind(&BSPrizeSelector::onShowMatchBtnTouched, this, std::placeholders::_1));
	showMatchBtn->setVisible(false);

	return true;
}

void BSPrizeSelector::showPrizeCards(const PlayerIdType &pid) {
	root->setVisible(true);
	curPid = pid;
	auto prizePile = btlScn->getPrizePile(curPid);
	auto &prizeCards = prizePile->cardMap;

	for (unsigned int idx = 0; idx < prizeCards.size(); ++idx) {
		auto card = prizeCards[idx];
		if (!card) // card == nullptr
			continue;

		auto holder = holderVec[idx];
		holder->addCard(card);
		prizeCards[idx] = nullptr; // Clear vector trong prizePile
	}

	doneBtn->setVisible(true);
}

void BSPrizeSelector::hidePrizeCard() {
	auto prizePile = btlScn->getPrizePile(curPid);
	for (unsigned int idx = 0; idx < holderVec.size(); ++idx) {
		auto holder = holderVec[idx];
		if (holder->hasCard()) {
			auto card = holder->removeCard();
			//prizePile->addCard(card, idx, 0.5f + idx * 0.2f);
			prizePile->addCard(card, idx, 0.0f);
		}
	}
	root->setVisible(false);
}

const string BSPrizeSelector::ON_DONE_SELECT = "ON_DONE_SELECT";

void BSPrizeSelector::onDoneBtnTouched(cocos2d::Ref* sender) {
	// Tao moi ket qua lua chon
	auto prizePile = btlScn->getPrizePile(curPid);
	idxVec.clear(); // Clear ket qua truoc do

	for (unsigned int idx = 0; idx < holderVec.size(); ++idx) {
		auto holder = holderVec[idx];
		if (holder->hasCard() &&holder->isSelected())
				idxVec.push_back(idx);
	}

	// Kiem tra ket qua lua chon
	auto pAction = make_shared<MGame::PA_SelectPrizeCards>(curPid, idxVec);
	bool checkAction = btlScn->onPlayerDoAction(pAction);

	if (!checkAction)
		return;
	else {
		hidePrizeCard();
		dispatchEvent(ON_DONE_SELECT);
	}
}
void BSPrizeSelector::onShowMatchBtnTouched(cocos2d::Ref* sender) {
	CCLOG("Show match touched");

}

BATTLE_SCENE_NS_END