#include "CardHolder.h"
#include "BSCard.h"
#include "move_selector/BSMoveSelector.h"
#include "GameManager.h"
#include "data/CardData.h"
#include "data/MoveData.h"
#include "BSPlayer.h"
#include "BattleScene.h"

#include "prefabs/BSPrefabs.h"
#include "common/ResourcePool.h"
#include "common/Utilize.h"
#include "components/WidgetTouchComponent.h"
#include <new>

//#include "players/BSPlayer.h"

using namespace std;
using namespace BSPrefabNS; // BS_PREFABS_USE_NS
USING_NS_CC;

BATTLE_SCENE_NS_BEG

Size CardHolder::HOLDER_SIZE = Size(95, 137);

CardHolder::CardHolder(HolderType hType, BattleScene *scn, PlayerIdType id, unsigned int hIdx) : 
	type(hType), btlScn(scn), ownerId(id), holderIdx(hIdx) {
	CCLOG("CardHolder::Ctor %p", this);
}

CardHolder::~CardHolder() {
	CCLOG("CardHolder::Dtor %p", this);
	clear();
}

void CardHolder::clear() {
	node = nullptr;

	for (auto &it : energyItemMap) {
		auto &eItem = it.second;
		eItem->release();
		eItem = nullptr;
	}
	energyItemMap.clear();
	energyPanel->removeAllChildren();
}


CardHolder* CardHolder::createActive(BattleScene *scn, PlayerIdType id) {
	auto holder = new (nothrow) CardHolder(HolderType::Active, scn, id);
	if (holder && holder->init()) {
		holder->autorelease();
		return holder;
	}
	delete holder;
	holder = nullptr;
	return holder;
}

CardHolder* CardHolder::createBench(BattleScene *scn, PlayerIdType id, unsigned int benchIdx) {
	auto holder = new (nothrow) CardHolder(HolderType::Bench, scn, id, benchIdx);
	if (holder && holder->init()) {
		holder->autorelease();
		return holder;
	}
	delete holder;
	holder = nullptr;
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
	
	auto panel = node->getChildByName("PanelHolder");

	showInfoPanel = dynamic_cast<ui::Layout*>(panel->getChildByName("ShowInfoPanel"));
	showInfoPanel->addTouchEventListener([this](Ref *sender, ui::Widget::TouchEventType ev) {
		switch (ev) {
		case ui::Widget::TouchEventType::ENDED:
			onTouchHolder();
		}
		});

	cardMarker = panel->getChildByName("Holder_CardMaker");
	energyCardMarker = panel->getChildByName("Energy_Cards_Marker");

	flyingText = dynamic_cast<ui::Text*>(panel->getChildByName("Flying_Text"));
	flyingText->setVisible(false);

	auto layout = node->getChildByName("PanelHolder");
	this->setContentSize(layout->getContentSize());

	hpPanel = dynamic_cast<ui::Layout*>(panel->getChildByName("Hd_HpPanel"));
	hpText = dynamic_cast<ui::Text*>(hpPanel->getChildByName("Hd_HpPanel_Text"));
	petTypeImg = dynamic_cast<ui::ImageView*>(hpPanel->getChildByName("Hd_HpPanel_Energy"));

	energyPanel = dynamic_cast<ui::Layout*>(panel->getChildByName("Energy_Panel"));

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


bool CardHolder::tryAddBasicPetCard(PetCard *card, const function<void()> &onDone) {
	if (!card || petCard)
		return false;

	constexpr float CardMoveSpeed = 400.0f;
	auto petData = dynamic_pointer_cast<const PetCardData>(card->getData());
	bool isBasic = petData->evStage < 1;
	if (isBasic) {
		petCard = card;

		auto startPosition = Utilize::mnode::getLocalPos(card, cardMarker);
		card->removeFromParent();
		cardMarker->addChild(card);

		auto destPosition = Vec2(0, 0);
		auto dist = destPosition.distance(startPosition);
		card->setPosition(startPosition);

		card->runAction(
			Sequence::create(
				MoveTo::create(dist / CardMoveSpeed, destPosition),
				CallFunc::create([onDone]() {
					if (onDone)
						onDone();
					}),
				nullptr
				));
		WidgetTouchNS::setEnableDragComponent(card, false);

		// * Cap nhat thong tin holder
		dmgCounter = 0;
		maxHp = petData->hp;
		playedTurn = btlScn->getTurnCount();
		if (!card->isFlippedDown())
			updateInfoPanel(true);
		return true;
	}
	
	return false;
}

bool CardHolder::tryEvolveTo(PetCard *toCard, const std::function<void()> &onDone) {
	if (!toCard || !petCard) // * Check 1
		return false;

	auto player = btlScn->getBSPlayer(ownerId);
	if (player->actionExceedsLimit(BSPlayer::TurnAction::AttachEnergy)) // * Check 2
		return false;

	bool suc = false;

	auto toPetData = dynamic_pointer_cast<const PetCardData>(toCard->getData());
	//bool isEv = toPetData->evStage >= 1;
	auto holderPetData = dynamic_pointer_cast<const PetCardData>(petCard->getData());
	if (holderPetData->name == toPetData->evolveFrom) 
		suc = true; // Co the tien hoa
	
	if (suc) {
		preEvCardVec.push_back(petCard);
		petCard = toCard;

		auto startPosition = Utilize::mnode::getLocalPos(toCard, cardMarker);
		toCard->removeFromParent();
		cardMarker->addChild(toCard);
		
		toCard->setFlip(false);

		auto destPosition = Vec2(0, 0);
		auto dist = destPosition.distance(startPosition);
		toCard->setPosition(startPosition);
		constexpr float CardMoveSpeed = 420.0f;
		toCard->runAction(
			Sequence::create(
				MoveTo::create(dist / CardMoveSpeed, destPosition),
				CallFunc::create([toCard]() {toCard->setPosition(Vec2(0, 25)); }),
				MoveTo::create(0.6f, Vec2(0, 0)),
				CallFunc::create([onDone, this]() {
					showFlyingText("Evolved");
					if (onDone)
						onDone();
					}),
				nullptr));

		// * Cap nhat thong tin holder
		playedTurn = btlScn->getTurnCount();
		maxHp = toPetData->hp;
		player->updateActionCount(BSPlayer::TurnAction::EvolvePet, 1);
		updateInfoPanel(true); // * Cap nhat Hp UI
	}

	return suc;
}

bool CardHolder::tryAddEnergyCard(EnergyCard *energyCard, const function<void()> &onDone) {
	if (!petCard || !energyCard)
		return false;
	
	auto player = btlScn->getBSPlayer(ownerId);
	if (player->actionExceedsLimit(BSPlayer::TurnAction::AttachEnergy))
		return false;

	auto energyData = dynamic_pointer_cast<const EnergyCardData>(energyCard->getData());
	// Cap nhat EnergyPanel
	auto eType = energyData->eType;
	auto eNum = energyData->eNum;
	auto foundIt = energyItemMap.find(eType);
	if (foundIt == energyItemMap.cend())  // Khong tim thay
	{
		auto &eItem = energyItemMap[eType] = EnergyItem::create(eType, 1);
		eItem->retain();
		eItem->stack(0);
		energyPanel->addChild(eItem);

		// Them Energy Sprite
		auto startPos = Vec2(0, 0);
		auto insIdx = energyItemMap.size() - 1;
		auto spaceX = -5.0f;
		auto ewidth = eItem->getEnergySize().width;
		auto xinc = (eItem->getEnergySize().width + spaceX) * insIdx;
		eItem->setPosition(startPos + xinc * Vec2(1, 0));
	}
	else {
		auto &eItem = foundIt->second;
		eItem->stack(1);
	}

	// Them card
	energyCardVec.push_back(energyCard);

	// Animation
	energyCard->setFlip(false);

	auto startPosition = Utilize::mnode::getLocalPos(energyCard, cardMarker);
	energyCard->removeFromParent();
	energyCardMarker->addChild(energyCard);

	auto destPosition = Vec2(0, 0);
	auto dist = destPosition.distance(startPosition);
	energyCard->setPosition(startPosition);
	constexpr float CardMoveSpeed = 420.0f;
	energyCard->runAction(
		Sequence::create(
			MoveTo::create(dist / CardMoveSpeed, destPosition),
			CallFunc::create([energyCard]() {energyCard->setPosition(Vec2(0, 55)); }),
			MoveTo::create(0.6f, Vec2(0, 0)),
			CallFunc::create([onDone, this]() {
				showFlyingText("Energy attached");
				if (onDone)
					onDone();
				}),
			nullptr));

	player->updateActionCount(BSPlayer::TurnAction::AttachEnergy, 1);
	return true;
}

void CardHolder::addEnergyItem(const std::string eType, unsigned int eNum) {
}


void CardHolder::updateInfoPanel(bool show) {
	if (show)
		hpPanel->setVisible(true);

	unsigned int curHp = maxHp > dmgCounter * DMG_COUNTER_VAL ? maxHp - dmgCounter * DMG_COUNTER_VAL : 0;
	hpText->setString(to_string(curHp));
}

void CardHolder::showFlyingText(const string &s) {
	flyingText->setString(s);
	flyingText->setVisible(true);
	Vec2 pos = Vec2(45, 67);
	flyingText->runAction(Sequence::create(
		MoveTo::create(0.8f, pos + Vec2(0, 20)), 
		CallFunc::create([this]() {flyingText->setVisible(false); }), 
		nullptr));
}

/*
	Checker
*/

bool CardHolder::hasPetCard() {
	return petCard != nullptr;
}

bool CardHolder::isActiveSpot() {
	return type == HolderType::Active;
}

bool CardHolder::canEvolveTo(PetCard *card) {
	//if (!card || !hasPetCard()) // Kiem tra petCard == Null
	//	return false;

	//auto toPetData = dynamic_pointer_cast<const PetCardData>(card->getData());
	//auto fromPetData = dynamic_pointer_cast<const PetCardData>(petCard->getData()); // Loi khi petCard == null

	//auto btlMgr = btlScn->getBattleManager();

	//return  btlMgr->getCurTurn() > playedTurn && // Tai luot nay co the tien hoa
	//	toPetData->evolveFrom == fromPetData->name; // Co the tien hoa len Pokemon duoc check
	return false;
}

void CardHolder::setFlipPetCard(bool flip) {
	if (petCard) {
		petCard->setFlip(flip, true);
	}
}

void CardHolder::onTouchHolder() {
	auto moveSelector = btlScn->getMoveSelector();
	moveSelector->showInfoHolder(this);
}

bool CardHolder::checkCanUseMove(unsigned int moveIdx) const {
	//if (!petCard)
	//	return false;

	//const auto &moveVec = petCard->getPetData()->moveVec;
	//if (moveIdx >= moveVec.size)
	//	return false;

	//auto moveData = moveVec.at(moveIdx);
	//auto costMap = moveData->costMap;

	return true;
}



BATTLE_SCENE_NS_END