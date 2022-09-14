#include "BSPrizeSelector.h"

#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/LangSys.h"
#include "scenes/battle_scene_3/BattleScene.h"
#include "ui/UIHelper.h"

#include "scenes/battle_scene_3/BSCard.h"
#include "scenes/battle_scene_3/BSResources.h"

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
BSPrizeHolder::~BSPrizeHolder() {}

bool BSPrizeHolder::init() {
	if (!root)
		return false;

	container = dynamic_cast<ui::Layout*>(root->getChildByName("Container"));
	cardMarker = container->getChildByName("Card_Marker");

	selectBorder = dynamic_cast<ui::ImageView*>(container->getChildByName("Select_Border"));
	selectBorder->setVisible(false);

	return true;
}



//---------------------//


shared_ptr<BSPrizeSelector> BSPrizeSelector::create(BattleScene *scn) {
	auto ret = make_shared<BSPrizeSelector>(scn);
	if (ret && ret->init())
		return ret;
	return nullptr;
}

BSPrizeSelector::BSPrizeSelector(BattleScene *scn) : btlScn(scn) {
	holderVec.reserve(PRIZE_CARDS_NUM_PER_PLAYER);
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
	for (const auto &holderNode : container->getChildren()) {
		auto holder = BSPrizeHolder::create(holderNode);
		if (holder) {
			holderVec.push_back(holder);
		}
	}
	
	return true;
}

BATTLE_SCENE_NS_END