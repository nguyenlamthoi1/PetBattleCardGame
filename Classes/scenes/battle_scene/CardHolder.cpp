#include "CardHolder.h"
#include "BSCard.h"
#include "GameManager.h"
#include "data/CardData.h"
#include "BattleScene.h"
#include "BSPlayer.h"
#include "prefabs/BSPrefabs.h"
#include "common/ResourcePool.h"
#include "common/Utilize.h"
#include <new>

using namespace std;
USING_NS_CC;
BS_PREFABS_USE_NS;

BATTLE_SCENE_NS_BEG

Size CardHolder::HOLDER_SIZE = Size(95, 137);

CardHolder::CardHolder(BattleScene *scn, PlayerIdType id) : btlScn(scn), ownerId(id) {
}

CardHolder::~CardHolder() {
	CCLOG("CardHolder dtor : called");
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
}

CardHolder* CardHolder::create(BattleScene *scn, PlayerIdType id) {
	auto holder = new (nothrow) CardHolder(scn, id);
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
	
	auto panel = node->getChildByName("PanelHolder");
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


bool CardHolder::tryAddPetCard(PetCard *card) {
	auto petData = dynamic_pointer_cast<const PetCardData>(card->getData());
	bool isBasic = petData->evStage < 1;
	if (isBasic) {
		if (petCard)
			return false;
		petCard = card;
		card->removeFromParent();
		cardMarker->addChild(card);
		auto pos = Vec2(0, 0);
		card->setPosition(pos + Vec2(0, 25));
		card->runAction(MoveTo::create(0.5f, pos));
		updateInfoPanel(true);
		return true;
	}
	else { // Evolved card
		if (!petCard)
			return false;
		
		auto holderPetData = dynamic_pointer_cast<const PetCardData>(petCard->getData());
		if (holderPetData->name != petData->evolveFrom)
			return false;

		petCard->removeFromParent();
		petCard->retain();
		preEvCardVec.push_back(petCard);

		petCard = card;
		card->removeFromParent();
		cardMarker->addChild(card);
		auto pos = Vec2(0, 0);
		card->setPosition(pos + Vec2(0, 25));
		card->runAction(MoveTo::create(0.5f, pos));
		showFlyingText("Evolved");
		updateInfoPanel(true);
		return true;
	}
	return false;
}

bool CardHolder::tryAddEnergyCard(EnergyCard *energyCard) {
	if (!petCard)
		return false;
	
	auto energyData = dynamic_pointer_cast<const EnergyCardData>(energyCard->getData());
	auto player = btlScn->getPlayer(ownerId);
	auto energyAttach = player->getActionData(BSPlayer::PlayerActionId::ATTACH_ENERGY);
	
	if (energyAttach->doneCount >= energyAttach->maxCount)
		return false;

	auto eType = energyData->eType;
	auto foundIt = energyItemMap.find(eType);
	if (foundIt == energyItemMap.cend()) {
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
	energyCard->removeFromParent();
	energyCardMarker->addChild(energyCard);
	auto pos = Vec2(0, 0);
	energyCard->setPosition(pos + Vec2(50, 0));
	energyCard->runAction(MoveTo::create(0.5f, pos));
	showFlyingText("Energy attached");


	energyAttach->doneCount += 1;

	return true;


}

void CardHolder::updateInfoPanel(bool show) {
	auto holderPetData = dynamic_pointer_cast<const PetCardData>(petCard->getData());
	if (show)
		hpPanel->setVisible(true);
	hpText->setString(to_string(holderPetData->hp - dmgCounter));
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

BATTLE_SCENE_NS_END