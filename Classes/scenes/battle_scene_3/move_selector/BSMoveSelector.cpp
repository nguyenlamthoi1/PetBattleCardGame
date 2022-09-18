#include "BSMoveSelector.h"

#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/LangSys.h"
#include "ui/UIHelper.h"

#include "scenes/battle_scene_3/BattleScene.h"
#include "scenes/battle_scene_3/BSCard.h"
#include "scenes/battle_scene_3/BSResources.h"
#include "scenes/battle_scene_3/BSPrizePile.h"
#include "scenes/battle_scene_3/CardHolder.h"
#include "scenes/battle_scene_3/prefabs/BSPrefabs.h"


#include "data/CardData.h"


#include "scenes/battle_scene_3/game/player_actions/PlayerAction.h"


USING_NS_CC;
using namespace std;
BS_PREFABS_USE_NS;

BATTLE_SCENE_NS_BEG

shared_ptr<BSMoveSelector> BSMoveSelector::create(BattleScene *scn) {
	auto ret = make_shared<BSMoveSelector>(scn);
	if (ret && ret->init())
		return ret;
	return nullptr;
}

BSMoveSelector::BSMoveSelector(BattleScene *scn) : btlScn(scn) {}

BSMoveSelector::~BSMoveSelector(){
	cleanUI();
}

bool BSMoveSelector::init() {
	if (!btlScn)
		return false;

	auto scnroot = btlScn->getScnRoot();
	root = scnroot->getChildByName("Attack_Selector");
	if (!root)
		return false;

	container = dynamic_cast<ui::Layout*>(root->getChildByName("Info_Panel"));
	topPanel = dynamic_cast<ui::Layout*>(container->getChildByName("Top_Panel"));
	
	auto namePanel = dynamic_cast<ui::Layout*>(topPanel->getChildByName("Name_Panel"));
	petNameLb = dynamic_cast<ui::Text*>(namePanel->getChildByName("Pokemon_Name"));

	auto hpPanel = dynamic_cast<ui::Layout*>(namePanel->getChildByName("HpPanel"));
	curHpLb = dynamic_cast<ui::Text*>(hpPanel->getChildByName("CurHp"));
	maxHpLb = dynamic_cast<ui::Text*>(hpPanel->getChildByName("MaxHp"));

	auto energyPanel = dynamic_cast<ui::Layout*>(topPanel->getChildByName("Energy_Panel"));
	eSpritePanel = dynamic_cast<ui::Layout*>(energyPanel->getChildByName("EnergySprite_Panel"));

	listView = dynamic_cast<ui::ListView*>(container->getChildByName("ListView"));
	
	bottomPanel = dynamic_cast<ui::Layout*>(container->getChildByName("Bottom_Panel"));
	
	weakPanel = dynamic_cast<ui::Layout*>(bottomPanel->getChildByName("Weakness_Panel"));
	weakEnergyMarker = weakPanel->getChildByName("Marker");
	
	resistPanel = dynamic_cast<ui::Layout*>(bottomPanel->getChildByName("Resistance_Panel"));
	resistEnergyMarker = resistPanel->getChildByName("Marker");

	retreatPanel = dynamic_cast<ui::Layout*>(bottomPanel->getChildByName("Retreat_Panel"));
	retreatEnergyPanel = dynamic_cast<ui::Layout*>(retreatPanel->getChildByName("RetreatEnergyPanel"));
	retreatBtn = dynamic_cast<ui::Button*>(retreatPanel->getChildByName("UseBtn"));

	blackPanel = dynamic_cast<ui::Layout*>(root->getChildByName("Black_Panel"));
	blackPanel->addTouchEventListener([this](Ref *sender, ui::Widget::TouchEventType ev) {
		switch (ev) {
		case ui::Widget::TouchEventType::ENDED:
			hide();
		}
		});
	
	return true;
}

void BSMoveSelector::showInfoHolder(CardHolder *holder) {
	//--Input--//
	
	const CardId activeId = "P2";
	const std::vector<CardId> preEvIds = { "P2", "P1" };
	const std::vector<CardId> eIds = { "E1", "E2", "E3"};
	const unsigned int dmgCounter = 40;
	//---------//
	cleanUI();
	root->setVisible(true);
	auto dm = GM_DATA_MGR;
	auto activeData = dynamic_pointer_cast<const PetCardData>(dm->getCardData(activeId));
	
	// PetNameLb
	auto petName = activeData->name;
	petNameLb->setString(petName);
	// Hp
	auto maxHp = activeData->hp;
	auto curHp = maxHp >= dmgCounter * DMG_COUNTER_VAL ? maxHp - dmgCounter * DMG_COUNTER_VAL : 0;
	maxHpLb->setString(to_string(maxHp));
	curHpLb->setString(to_string(curHp));
	// Energy Panel
	for (const auto &eid : eIds) {
		auto eData = dynamic_pointer_cast<const EnergyCardData>(dm->getCardData(eid));
		auto eNum = eData->eNum;
		auto eType = eData->eType;
		auto eItem = EnergyItem::create(eType, eNum);
		eSpritePanel->addChild(eItem);
	}
	updateEnergySpritePanel();
	// Retreat Panel
	auto &retreatMap = activeData->retreatMap;
	for (const auto itr : retreatMap) {
		auto eType = itr.first;
		auto eNum = itr.second;
		auto eItem = EnergyItem::create(eType, eNum);
		retreatEnergyPanel->addChild(eItem);
	}
	updateRetreatEnergySpritePanel();
	retreatBtn->setVisible(false);
	// Weakness Panel
	auto &weakSet = activeData->weakSet;
	if (!weakSet.empty()) {
		auto eType = *(weakSet.cbegin());
		
		auto eItem = EnergyItem::create(eType, 1);
		weakEnergyMarker->addChild(eItem);
		
		auto text = dynamic_cast<ui::Text*>(weakPanel->getChildByName("Text"));
		text->setString("x2");
	}
	else {
		weakEnergyMarker->setVisible(false);
		auto text = dynamic_cast<ui::Text*>(weakEnergyMarker->getChildByName("Text"));
		text->setVisible(false);
	}

	// Resist Panel
	auto &resistMap = activeData->resistanceMap;
	if (!resistMap.empty()) {
		auto eType = resistMap.cbegin()->first;
		auto eNum = resistMap.cbegin()->second;

		auto eItem = EnergyItem::create(eType, 1);
		resistEnergyMarker->addChild(eItem);

		auto text = dynamic_cast<ui::Text*>(resistPanel->getChildByName("Text"));
		text->setString(to_string(eNum));
	}
	else {
		resistEnergyMarker->setVisible(false);
		auto text = dynamic_cast<ui::Text*>(resistPanel->getChildByName("Text"));
		text->setVisible(false);
	}
};

void BSMoveSelector::updateEnergySpritePanel() {
	auto children = eSpritePanel->getChildren();
	unsigned int i = 0;
	constexpr float spaceX = 10.0f;
	for (const auto &child : children) {
		auto eChild = dynamic_cast<EnergyItem*>(child);
		auto eSize = eChild->getEnergySize();
		//auto eSize = child->getContentSize();
		child->setPositionX((eSize.width / 2 + spaceX) * i);
		child->setPositionY(0);
		++i;
	}
}

void BSMoveSelector::updateRetreatEnergySpritePanel() {
	auto children = retreatEnergyPanel->getChildren();
	unsigned int i = 0;
	constexpr float spaceX = 10.0f;
	for (const auto &child : children) {
		auto eChild = dynamic_cast<EnergyItem*>(child);
		auto eSize = eChild->getEnergySize();
		child->setPositionX((eSize.width / 2 + spaceX) * i);
		child->setPositionY(0);
		++i;
	}
}

void BSMoveSelector::hide() {
	cleanUI();
	root->setVisible(false);
}

void BSMoveSelector::cleanUI() {
	eSpritePanel->removeAllChildren(); // * Return Pool energyItem
	retreatEnergyPanel->removeAllChildren(); // * Return Pool energyItem
	weakEnergyMarker->removeAllChildren();
	resistEnergyMarker->removeAllChildren();
}


//void BSPrizeSelector::showPrizeCards(const PlayerIdType &pid) {
//	root->setVisible(true);
//	curPid = pid;
//	auto prizePile = btlScn->getPrizePile(curPid);
//	auto &prizeCards = prizePile->cardVec;
//
//	for (unsigned int idx = 0; idx < prizeCards.size(); ++idx) {
//		auto card = prizeCards[idx];
//		if (!card) // card == nullptr
//			continue;
//
//		auto holder = holderVec[idx];
//		holder->addCard(card);
//		holder->setSelectable(false);
//		holder->setOnTouched(nullptr);
//		prizeCards[idx] = nullptr; // Clear vector trong prizePile
//	}
//
//	doneBtn->setVisible(false);
//}

//void BSPrizeSelector::showPrizeCardsToSelect(const PlayerIdType &pid, unsigned int num) {
//	root->setVisible(true);
//	curPid = pid;
//	auto prizePile = btlScn->getPrizePile(curPid);
//	auto &prizeCards = prizePile->cardVec;
//
//	for (unsigned int idx = 0; idx < prizeCards.size(); ++idx) {
//		auto card = prizeCards[idx];
//		if (!card) // card == nullptr
//			continue;
//
//		auto holder = holderVec[idx];
//		holder->addCard(card);
//		holder->setOnTouched(bind(&BSPrizeSelector::onHolderTouched, this, placeholders::_1));
//		holder->setSelectable(true);
//		prizeCards[idx] = nullptr; // Clear vector trong prizePile
//	}
//	selectNum = num;
//	selectedNum = 0;
//	doneBtn->setVisible(true);
//}

//void BSPrizeSelector::hidePrizeCard() {
//	auto prizePile = btlScn->getPrizePile(curPid);
//	for (unsigned int idx = 0; idx < holderVec.size(); ++idx) {
//		auto holder = holderVec[idx];
//		if (holder->hasCard()) {
//			auto card = holder->removeCard();
//			//prizePile->addCard(card, idx, 0.5f + idx * 0.2f);
//			prizePile->addCard(card, idx, 0.0f);
//		}
//	}
//	root->setVisible(false);
//}

//const string BSPrizeSelector::ON_DONE_SELECT = "ON_DONE_SELECT";

	//void BSPrizeSelector::onDoneBtnTouched(cocos2d::Ref* sender) {
	//	// Tao moi ket qua lua chon
	//	auto prizePile = btlScn->getPrizePile(curPid);
	//	idxVec.clear(); // Clear ket qua truoc do
	//
	//	for (unsigned int idx = 0; idx < holderVec.size(); ++idx) {
	//		auto holder = holderVec[idx];
	//		if (holder->hasCard() &&holder->isSelected())
	//				idxVec.push_back(idx);
	//	}
	//
	//	if (idxVec.size() != selectNum)
	//		return;
	//
	//	// Kiem tra ket qua lua chon
	//	auto pAction = make_shared<MGame::PA_SelectPrizeCards>(curPid, idxVec);
	//	bool checkAction = btlScn->onPlayerDoAction(pAction);
	//
	//	if (!checkAction)
	//		return;
	//	else {
	//		hidePrizeCard();
	//		dispatchEvent(ON_DONE_SELECT);
	//	}
	//}
	//
	//void BSPrizeSelector::onHolderTouched(BSPrizeHolder *holder) {
	//	bool isSelected = holder->isSelected();
	//	if (isSelected) {
	//		holder->setSelected(!isSelected);
	//		
	//		if(selectedNum > 0)
	//			selectedNum -= 1;
	//	}
	//	else { // Chua duoc chon
	//		if (selectedNum < selectNum) {
	//			holder->setSelected(!isSelected);
	//			selectedNum += 1;
	//		}
	//	}
	//	
	//}
	//
	//void BSPrizeSelector::onShowMatchBtnTouched(cocos2d::Ref* sender) {
	//	CCLOG("Show match touched");
	//
	//}

BATTLE_SCENE_NS_END