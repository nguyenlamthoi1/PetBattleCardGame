#include "CardHolder.h"
#include "BSCard.h"
#include "move_selector/BSMoveSelector.h"
#include "GameManager.h"
#include "data/CardData.h"
#include "data/MoveData.h"
#include "BSPlayer.h"
#include "BattleScene.h"
#include "common/Utilize.h"

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
	//this->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	//this->setBackGroundColor(Color3B::BLUE);
	//this->setBackGroundColorOpacity(255 / 3);
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
	//evCardMarker = panel->getChildByName("Ev_Marker");

	flyingText = dynamic_cast<ui::Text*>(panel->getChildByName("Flying_Text"));
	flyingText->setVisible(false);

	auto layout = node->getChildByName("PanelHolder");
	this->setContentSize(layout->getContentSize());

	hpPanel = dynamic_cast<ui::Layout*>(panel->getChildByName("Hd_HpPanel"));
	hpText = dynamic_cast<ui::Text*>(hpPanel->getChildByName("Hd_HpPanel_Text"));
	petTypeImg = dynamic_cast<ui::ImageView*>(hpPanel->getChildByName("Hd_HpPanel_Energy"));

	energyPanel = dynamic_cast<ui::Layout*>(panel->getChildByName("Energy_Panel"));

	dmgImg = dynamic_cast<ui::ImageView*>(layout->getChildByName("Dmg_Img"));
	dmgTxt = dynamic_cast<ui::Text*>(dmgImg->getChildByName("Dmg_Lb"));
	dmgImg->setVisible(false);

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
	//if (player->actionExceedsLimit(BSPlayer::TurnAction::EvolvePet)) // * Check 2
	//	return false;

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

		constexpr float CardMoveSpeed = 400.0f;
		auto destPosition = Vec2(0, 0);
		auto dist = destPosition.distance(startPosition);
		toCard->setPosition(startPosition);

		toCard->runAction(
			Sequence::create(
				MoveTo::create(dist / CardMoveSpeed, destPosition),
				CallFunc::create([this, onDone]() {
					showFlyingText("Evolved");
					if (onDone)
						onDone();
					}),
				nullptr
						));
		WidgetTouchNS::setEnableDragComponent(toCard, false);

		/*auto destPosition = Vec2(0, 0);
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
				nullptr));*/

		// * Cap nhat thong tin holder
		playedTurn = btlScn->getTurnCount();
		maxHp = toPetData->hp;
		updateInfoPanel(true); // * Cap nhat Hp UI
		//player->updateActionCount(BSPlayer::TurnAction::EvolvePet, 1);
	}

	return suc;
}

bool CardHolder::tryAddEnergyCard(EnergyCard *energyCard, const function<void()> &onDone) {
	if (!petCard || !energyCard)
		return false;
	
	auto player = btlScn->getBSPlayer(ownerId);
	//if (player->actionExceedsLimit(BSPlayer::TurnAction::AttachEnergy))
	//	return false;

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

	//player->updateActionCount(BSPlayer::TurnAction::AttachEnergy, 1);
	return true;
}

void CardHolder::addEnergyItem(const std::string eType, unsigned int eNum) {}

bool CardHolder::tryAttackActiveOpp(CardHolder* taker, unsigned int totalDmg, bool triggerWeak, bool triggerResist, const function<void()> &onDone) {
	if (!taker) {
		if (onDone)
			onDone();
		return false;
	}

	petCard->runAction( // Action 1
		Sequence::create(
			EaseBackInOut::create(MoveBy::create(0.4f, Vec2(0, 50))),
			MoveTo::create(0.6f, Vec2(0, 0)),
			nullptr));

	petCard->runAction( // Action 2
		Sequence::create(
			DelayTime::create(0.35f),
			CallFunc::create([taker, totalDmg, triggerWeak, triggerResist, onDone]() {
				taker->onTakeDamage(totalDmg, triggerWeak, triggerResist);
				if (onDone)
					onDone();
				}),
			nullptr));

}

void CardHolder::onTakeDamage(unsigned int totalDmg, bool triggerWeak, bool triggerResist) {
	dmgCounter += totalDmg;
	updateInfoPanel(true);

	// Do animation
	launchFlyingMsg(to_string(totalDmg), Color3B::ORANGE);
	float delay = 0.2f;
	if (triggerWeak) {
		launchFlyingMsg("Weakness", Color3B::RED, delay);
		delay += 0.2f;
	}
	if (triggerResist)
		launchFlyingMsg("Resistance", Color3B::BLUE, delay);

	updateDmgImg(true);
}

void CardHolder::launchFlyingMsg(const std::string &msg, cocos2d::Color3B color, float delay) {
	auto flyingTextParent = flyingText->getParent();
	auto cloned = dynamic_cast<ui::Text*>(flyingText->clone());
	flyingTextParent->addChild(cloned);
	cloned->setString(msg);
	cloned->setColor(color);
	cloned->setVisible(true);
	Vec2 pos = Vec2(45, 67);
	cloned->runAction(Sequence::create(
		DelayTime::create(delay),
		MoveTo::create(0.1f, pos + Vec2(0, 30)),
		RemoveSelf::create(),
		nullptr));
}

void CardHolder::removePetAndAllCards(std::vector<BSCard*> &vec) {
	vec.push_back(petCard);
	petCard->removeFromParent();
	petCard = nullptr;

	for (const auto &pCard : preEvCardVec) {
		pCard->removeFromParent();
		pCard->stopAllActions();
		vec.push_back(pCard);
	}
	preEvCardVec.clear();

	for (const auto &eCard : energyCardVec) {
		eCard->removeFromParent();
		eCard->stopAllActions();
		vec.push_back(eCard);
	}
	energyCardVec.clear();
	energyPanel->removeAllChildren();

	dmgCounter = 0;
	maxHp = 0;
	playedTurn = 0;
	updateInfoPanel(false);
	updateDmgImg();
}



void CardHolder::updateInfoPanel(bool show) {
	hpPanel->setVisible(show);

	unsigned int curHp = maxHp > dmgCounter ? maxHp - dmgCounter : 0;
	hpText->setString(to_string(curHp));
}

void CardHolder::updateDmgImg(bool anim) {
	dmgImg->setVisible(dmgCounter > 0);
	dmgTxt->setString(to_string(dmgCounter));
	if (anim) {
		dmgImg->stopAllActions();
		dmgImg->setScale(0.45);
		dmgImg->runAction(EaseBackOut::create(ScaleTo::create(0.2, 0.5f , 0.5f)));
	}
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
	if (!card || !petCard)
		return false;

	auto toPetData = dynamic_pointer_cast<const PetCardData>(card->getData());
	auto fromPetData = dynamic_pointer_cast<const PetCardData>(petCard->getData());

	return  btlScn->getTurnCount() > playedTurn && // Tai luot nay co the tien hoa
			toPetData->evolveFrom == fromPetData->name; // Co the tien hoa len Pokemon duoc check
}

void CardHolder::setFlipPetCard(bool flip) {
	if (petCard) {
		petCard->setFlip(flip, true);
	}
}

void CardHolder::onTouchHolder() {
	auto moveSelector = btlScn->getMoveSelector();
	btlScn->showHolderInfo(this);
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

void CardHolder::switchWithHolder(CardHolder *withHolder, const std::function<void()> &onDone) {
	if (!withHolder || !withHolder->hasPetCard())
		return;
	auto oldPos1 = Utilize::mnode::getLocalPos(node, withHolder);
	auto oldPos2 = Utilize::mnode::getLocalPos(withHolder->node, this);

	auto newNode = withHolder->node;
	auto oldNode = node;

	// Luu thong tin cu
	HolderData data;
	data.dmgCounter = dmgCounter;
	data.maxHp = maxHp;
	data.playedTurn = playedTurn;
	data.petCard = petCard;
	data.preEvCardVec = preEvCardVec;
	data.energyCardVec = energyCardVec;

	// Cap nhat thong tin moi
	dmgCounter = withHolder->dmgCounter;
	maxHp = withHolder->maxHp;
	playedTurn = withHolder->playedTurn;
	petCard = withHolder->petCard;
	preEvCardVec = withHolder->preEvCardVec;
	energyCardVec = withHolder->energyCardVec;
	newNode->removeFromParent();
	replaceWithNewNode(newNode);
	updateInfoPanel(hasPetCard());
	updateDmgImg(false);


	withHolder->dmgCounter = data.dmgCounter;
	withHolder->maxHp = data.maxHp;
	withHolder->playedTurn = data.playedTurn;
	withHolder->petCard = data.petCard;
	withHolder->preEvCardVec = data.preEvCardVec;
	withHolder->energyCardVec = data.energyCardVec;
	withHolder->replaceWithNewNode(oldNode);
	withHolder->updateInfoPanel(withHolder->hasPetCard());
	withHolder->updateDmgImg(false);
	
	/*oldNode->removeFromParent();
	withHolder->addChild(oldNode);
	oldNode->setPosition(oldPos1);
	oldNode->runAction(Sequence::create(
		MoveTo::create(0.5f, Vec2(0, 0)),
		nullptr
	));*/
	
	newNode->removeFromParent();
	this->addChild(newNode);
	newNode->setPosition(oldPos2);
	newNode->runAction(Sequence::create(
		MoveTo::create(0.5f, Vec2(0, 0)),
		CallFunc::create([onDone]() {
			if (onDone)
				onDone();
			}),
		nullptr));
}

void CardHolder::replaceWithNewNode(Node *newNode) {
	node->removeFromParent();
	node = newNode;
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

	dmgImg = dynamic_cast<ui::ImageView*>(layout->getChildByName("Dmg_Img"));
	dmgTxt = dynamic_cast<ui::Text*>(dmgImg->getChildByName("Dmg_Lb"));
	dmgImg->setVisible(false);
}
BATTLE_SCENE_NS_END