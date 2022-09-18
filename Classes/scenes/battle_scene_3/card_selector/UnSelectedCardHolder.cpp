#include "UnselectedCardHolder.h"

#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/LangSys.h"
#include "ui/UIHelper.h"

#include "data/CardData.h"

#include "scenes/battle_scene_3/BattleScene.h"
#include "scenes/battle_scene_3/BSCard.h"
#include "scenes/battle_scene_3/BSResources.h"
#include "scenes/battle_scene_3/game/player_actions/PlayerAction.h"


USING_NS_CC;
using namespace std;

BATTLE_SCENE_NS_BEG

BSUnselectedCardHolder* BSUnselectedCardHolder::create() {
	auto ret = new BSUnselectedCardHolder();
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

BSUnselectedCardHolder::BSUnselectedCardHolder() {}

BSUnselectedCardHolder::~BSUnselectedCardHolder() {
	if (root) {
		root->removeFromParent();
		ResourcePool::returnNode(root);
	}
}

bool BSUnselectedCardHolder::init() {
	if (!ui::Layout::init())
		return false;

	auto pool = GM_POOL;
	root = pool->tryGetNodeCsb("ccstd_csb/battle_scene/prefabs/unselected_card_holder.csb");
	if (!root)
		return false;
	this->addChild(root);
	this->setTouchEnabled(true);

	container = dynamic_cast<ui::Layout*>(root->getChildByName("Container"));
	numPanel = dynamic_cast<ui::ImageView*>(container->getChildByName("Num_Panel"));
	numLabel = dynamic_cast<ui::Text*>(numPanel->getChildByName("Num_Label"));
	cardMarker = container->getChildByName("Card_Marker");
	this->setContentSize(root->getContentSize());

	return true;
}

bool BSUnselectedCardHolder::addCard(BSCard *card) {
	if (!cardVec.empty()) {
		auto cid = card->getData()->id;
		auto firstId = cardVec.front()->getData()->id;
		if(cid != firstId)
			return false;
	}
	card->removeFromParent();
	card->stopAllActions();
	cardMarker->addChild(card);
	cardVec.push_back(card);

	numPanel->setVisible(cardVec.size() > 1);
	numLabel->setString(to_string(cardVec.size()));
	return true;
}

BSCard* BSUnselectedCardHolder::removeCard() {
	if (cardVec.empty())
		return nullptr;

	auto lastCard = cardVec.back();
	cardVec.pop_back();
	lastCard->removeFromParent();
	lastCard->stopAllActions();

	numPanel->setVisible(cardVec.size() > 1);
	numLabel->setString(to_string(cardVec.size()));
	return lastCard;
}


BATTLE_SCENE_NS_END