#include "SelectedCardHolder.h"

#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/LangSys.h"
#include "common/DataManager.h"
#include "ui/UIHelper.h"

#include "data/CardData.h"

#include "scenes/battle_scene_3/BattleScene.h"
#include "scenes/battle_scene_3/BSCard.h"
#include "scenes/battle_scene_3/BSResources.h"
#include "scenes/battle_scene_3/game/player_actions/PlayerAction.h"


USING_NS_CC;
using namespace std;

BATTLE_SCENE_NS_BEG

BSSelectedCardHolder* BSSelectedCardHolder::create() {
	auto ret = new BSSelectedCardHolder();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	else {
		delete ret;
		ret = nullptr;
	}

	return ret;
}

BSSelectedCardHolder::BSSelectedCardHolder() {}

BSSelectedCardHolder::~BSSelectedCardHolder() {
	if (root) {
		root->removeFromParent();
		ResourcePool::returnNode(root);
	}
}

bool BSSelectedCardHolder::init() {
	if (!ui::Layout::init())
		return false;

	auto pool = GM_POOL;
	root = pool->tryGetNodeCsb("ccstd_csb/battle_scene/prefabs/selected_card_holder_1.csb");
	if (!root)
		return false;
	this->addChild(root);
	this->setTouchEnabled(true);
	container = dynamic_cast<ui::Layout*>(root->getChildByName("Container"));
	numPanel = dynamic_cast<ui::ImageView*>(container->getChildByName("Num_Panel"));
	numLabel = dynamic_cast<ui::Text*>(numPanel->getChildByName("Num_Label"));
	numPanel->setVisible(false);
	cardMarker = container->getChildByName("Card_Marker");
	this->setContentSize(root->getContentSize());

	return true;
}

bool BSSelectedCardHolder::canAddCard(BSCard *bscard) const {
	if (hasCard())
		return false;
	return CardData::checkSelectType(selType, bscard->getData());
}

bool BSSelectedCardHolder::addCard(BSCard* bscard, BSUnselectedCardHolder *holder) {
	if (!canAddCard(bscard))
		return false;

	card = bscard;
	fromHolder = holder;
	card->removeFromParent();
	card->stopAllActions();
	cardMarker->addChild(card);
	
	numPanel->setVisible(false);
	numLabel->setString(to_string(1));
}

BSCard* BSSelectedCardHolder::removeCard() {
	if (!card)
		return nullptr;
	
	auto removedCard = card;
	card->removeFromParent();
	card->stopAllActions();
	card = nullptr;
	fromHolder = nullptr;
	numPanel->setVisible(false);
	numLabel->setString(to_string(0));
	return removedCard;
}


BATTLE_SCENE_NS_END