#include "BSMoveSelector.h"
#include "MoveHolder.h"

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
#include "scenes/battle_scene_3/BSPlayer.h"
#include "scenes/battle_scene_3/BSBoard.h"

#include "scenes/battle_scene_3/prefabs/BSPrefabs.h"
#include "scenes/battle_scene_3/game/player_actions/PlayerAction.h"

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

	cardMarker1 = root->getChildByName("CardMarker1");
	cardMarker2 = root->getChildByName("CardMarker2");

	nextBtn = dynamic_cast<ui::Button*>(root->getChildByName("Next_Button"));
	prevBtn = dynamic_cast<ui::Button*>(root->getChildByName("Prev_Button"));


	blackPanel = dynamic_cast<ui::Layout*>(root->getChildByName("Black_Panel"));
	blackPanel->addTouchEventListener([this](Ref *sender, ui::Widget::TouchEventType ev) {
		switch (ev) {
		case ui::Widget::TouchEventType::ENDED:
			hide();
		}
		});
	
	return true;
}

void BSMoveSelector::showInfoHolder2(CardHolder *holder) {
	//--Input--//

	const CardId activeId = "P3";
	const std::vector<CardId> preEvIds = { "P2", "P1" };
	const std::vector<CardId> eIds = { "E1", "E2", "E3" };
	//const std::vector<CardId> preEvIds;
	//const std::vector<CardId> eIds;
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
	auto curHp = maxHp >= dmgCounter  ? maxHp - dmgCounter  : 0;
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

	// Moves
	auto moveVec = activeData->moveVec;
	for (const auto &moveData : moveVec) {
		auto moveItem = MoveHolder::create(moveData);
		listView->addChild(moveItem);
	}

	// Cards
	std::vector<CardId> allCardVec;
	allCardVec.push_back(activeId);
	allCardVec.insert(allCardVec.cend(), preEvIds.cbegin(), preEvIds.cend());
	allCardVec.insert(allCardVec.cend(), eIds.cbegin(), eIds.cend());

	auto offset = Vec2(10.0, -10.0);
	auto maxIdxHasOffset = 4;
	for (unsigned int i = 0; i < allCardVec.size(); ++i) {
		auto cid = allCardVec[i];
		auto cardData = dm->getCardData(cid);
		auto bsCard = BSCard::createWithData(cardData);
		bsCard->setFlip(false);
		bsCard->setLocalZOrder(allCardVec.size() - i);

		cardMarker1->addChild(bsCard);
		cardVec.push_back(bsCard); // Phai co thu tu : Active, PreEv, Energy,..

		bsCard->retain();
		if (i <= maxIdxHasOffset)
			bsCard->setPosition(Vec2(offset.x * i, offset.y * i));
		else
			bsCard->setPosition(Vec2(offset.x * maxIdxHasOffset, offset.y * maxIdxHasOffset));
	}
	shownCurIdx = 0;
	nextBtn->addClickEventListener(CC_CALLBACK_1(BSMoveSelector::onClickNextBtn, this));
	prevBtn->addClickEventListener(CC_CALLBACK_1(BSMoveSelector::onClickPrevBtn, this));
	prevBtn->setVisible(false);
	nextBtn->setVisible(cardVec.size() > 1);
}


void BSMoveSelector::showInfoHolder(CardHolder *holder, bool allowUseMove, bool allowRetreat) {
	if (!holder->hasPetCard())
		return;

	//--Input--//
	auto activePetCard = holder->getActivePetCard();
	auto activeData = activePetCard->getPetData();
	const CardId activeId = activeData->id;

	std::vector<CardId> eIds;
	auto energyCards = holder->getAttachedEnergyCards();
	for (const auto &eCard : energyCards) {
		eIds.push_back(eCard->getData()->id);
	}

	std::vector<CardId> preEvIds;
	auto evCards = holder->getPrevEvCards();
	for (const auto &petCard : evCards) {
		preEvIds.push_back(petCard->getData()->id);
	}

	const unsigned int dmgCounter = holder->getDmgCounter();
	//---------//
	cleanUI();
	root->setVisible(true);
	auto dm = GM_DATA_MGR;

	//auto activeData = dynamic_pointer_cast<const PetCardData>(dm->getCardData(activeId));
	
	bool isActiveCard = holder->isActiveSpot();

	// PetNameLb
	auto petName = activeData->name;
	petNameLb->setString(petName);
	// Hp
	auto maxHp = activeData->hp;
	auto curHp = maxHp >= dmgCounter  ? maxHp - dmgCounter  : 0;
	maxHpLb->setString(to_string(maxHp));
	curHpLb->setString(to_string(curHp));
	// Energy Panel
	std::unordered_map<string, unsigned int> attachedEnergyMap;
	for (const auto &eid : eIds) {
		auto eData = dynamic_pointer_cast<const EnergyCardData>(dm->getCardData(eid));
		auto eNum = eData->eNum;
		auto eType = eData->eType;
		auto eItem = EnergyItem::create(eType, eNum);
		eSpritePanel->addChild(eItem);

		attachedEnergyMap[eType] += eNum; // * Cap nhat energy dang co
	}
	updateEnergySpritePanel();
	// Retreat Panel
	auto &retreatMap = activeData->retreatMap; // So nang luong can de retreat
	bool enoughEnergyToRetreat = true;
	for (const auto itr : retreatMap) {
		auto eType = itr.first;
		auto eNum = itr.second;
		auto eItem = EnergyItem::create(eType, eNum);
		retreatEnergyPanel->addChild(eItem);

		enoughEnergyToRetreat &= attachedEnergyMap[eType] >= eNum;
	}
	updateRetreatEnergySpritePanel();
	auto ownerId = holder->getOwnerId();
	auto player = btlScn->getBSPlayer(ownerId);
	auto board = btlScn->getBoard(ownerId);
	auto showRetreat = allowRetreat && isActiveCard &&
		!player->actionExceedsLimit(BSPlayer::TurnAction::Retreat)
		&& enoughEnergyToRetreat && board->hasPetOnBoard();
	retreatBtn->setVisible(showRetreat);
	retreatBtn->addClickEventListener([this, holder](Ref* sender) {
		onClickRetreat(holder);
		});

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

	// Moves
	auto moveVec = activeData->moveVec;
	unsigned int moveIdx = 0;
	for (const auto &moveData : moveVec) {
		auto moveItem = MoveHolder::create(moveData);
		listView->addChild(moveItem);

		const auto &costMap = moveItem->getRequiredEnergies();
		bool enoughEnergy = true;
		for (const auto &itr : costMap) {
			auto eType = itr.first;
			auto eNum = itr.second;
			enoughEnergy &= attachedEnergyMap[eType] >= eNum;
		}
		moveItem->setEnabledUseBtn(isActiveCard && enoughEnergy);
		moveItem->setClickUseBtnFunc([this, holder, moveIdx](MoveHolder *mHolder) {
			onClickUseMove(holder, mHolder, moveIdx);
			});
		++moveIdx;
	}

	// Cards
	std::vector<CardId> allCardVec;
	allCardVec.push_back(activeId);
	allCardVec.insert(allCardVec.cend(), preEvIds.cbegin(), preEvIds.cend());
	allCardVec.insert(allCardVec.cend(), eIds.cbegin(), eIds.cend());

	auto offset = Vec2(10.0, -10.0);
	auto maxIdxHasOffset = 4;
	for (unsigned int i = 0; i < allCardVec.size(); ++i) {
		auto cid = allCardVec[i];
		auto cardData = dm->getCardData(cid);
		auto bsCard = BSCard::createWithData(cardData);
		bsCard->setFlip(false);
		bsCard->setLocalZOrder(allCardVec.size() - i);
		
		cardMarker1->addChild(bsCard);
		cardVec.push_back(bsCard); // Phai co thu tu : Active, PreEv, Energy,..
		
		bsCard->retain();
		if(i <= maxIdxHasOffset)
			bsCard->setPosition(Vec2(offset.x * i, offset.y * i));
		else
			bsCard->setPosition(Vec2(offset.x * maxIdxHasOffset, offset.y * maxIdxHasOffset));
	}
	shownCurIdx = 0;
	nextBtn->addClickEventListener(CC_CALLBACK_1(BSMoveSelector::onClickNextBtn, this));
	prevBtn->addClickEventListener(CC_CALLBACK_1(BSMoveSelector::onClickPrevBtn, this));
	prevBtn->setVisible(false);
	nextBtn->setVisible(cardVec.size() > 1);
};

void BSMoveSelector::onClickNextBtn(cocos2d::Ref *sender) {
	if (shownCurIdx + 1 > cardVec.size()) {
		nextBtn->setVisible(false);
		return;
	}
	shownCurIdx += 1;
	updateCardPos();
}
void BSMoveSelector::onClickPrevBtn(cocos2d::Ref *sender) {
	if (shownCurIdx <= 0) {
		prevBtn->setVisible(false);
		return;
	}
	shownCurIdx -= 1;
	updateCardPos();
}

void BSMoveSelector::updateCardPos() {
	cardMarker1->removeAllChildren();
	cardMarker2->removeAllChildren();
	for (unsigned int i = 0; i < shownCurIdx; ++i) {
		cardMarker2->addChild(cardVec[i]);
		cardVec[i]->setLocalZOrder(i);
		cardVec[i]->setPosition(Vec2(0, 0));
	}

	auto offset = Vec2(10.0, -10.0);
	auto maxIdxHasOffset = 4;
	for (unsigned int i = shownCurIdx; i < cardVec.size(); ++i) {
		cardMarker1->addChild(cardVec[i]);
		cardVec[i]->setLocalZOrder(cardVec.size() - i);
		if (i <= maxIdxHasOffset)
			cardVec[i]->setPosition(Vec2(offset.x * i, offset.y * i));
		else
			cardVec[i]->setPosition(Vec2(offset.x * maxIdxHasOffset, offset.y * maxIdxHasOffset));
	}

	nextBtn->setVisible(shownCurIdx + 1 < cardVec.size());
	prevBtn->setVisible(shownCurIdx > 0);
}


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
	listView->removeAllChildren();

	for (const auto &bsCard : cardVec) {
		bsCard->returnToPool(); // Return Pool
		bsCard->removeFromParent(); // Sau do xoa di
		bsCard->release();
	}
	cardVec.clear();

}

void BSMoveSelector::onClickUseMove(CardHolder * cHolder, MoveHolder *mHolder, unsigned int moveIdx) {
	if (!cHolder || !mHolder) // Phai khac Nullptr
		return;

	bool isActive = cHolder->isActiveSpot(); 
	if (!isActive) // Holder thi trien skill phai la active
		return;

	auto ownerId = cHolder->getOwnerId();
	if (ownerId != PLAYER) // Khong phai nguoi choi thi khong duoc thuc thi
		return;

	auto player = btlScn->getBSPlayer(ownerId);
	if (player->actionExceedsLimit(BSPlayer::TurnAction::Attack)) // Chua tung Attack
		return;

	auto pMove = make_shared<MGame::PA_UseMove>(ownerId, moveIdx);
	bool suc = btlScn->onPlayerDoAction(pMove);
	if (suc) {
		hide();
		btlScn->setEnableEndTurnButton(false);
	}
}

void BSMoveSelector::onClickRetreat(CardHolder * holder) {
	if (!holder) // Phai khac Nullptr
		return;

	bool isActive = holder->isActiveSpot();
	if (!isActive) // Holder thi trien skill phai la active
		return;

	auto ownerId = holder->getOwnerId();
	auto holderIdx = holder->getHolderIdx();
	if (ownerId != PLAYER) // Khong phai nguoi choi thi khong duoc thuc thi
		return;

	auto player = btlScn->getBSPlayer(ownerId);
	if (player->actionExceedsLimit(BSPlayer::TurnAction::Retreat))
		return;

	auto board = btlScn->getBoard(ownerId);
	if (board->hasPetOnBoard()) {
		board->startSelectOneOfBench([this, ownerId, holderIdx](CardHolder *holder) {
			auto pMove = make_shared<MGame::PA_RetreatWith>(ownerId, holder->getHolderIdx());
			bool suc = btlScn->onPlayerDoAction(pMove);
			});
		hide();
	}

}

BATTLE_SCENE_NS_END