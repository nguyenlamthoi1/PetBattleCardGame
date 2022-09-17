#include "BSCardSelector.h"

#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/LangSys.h"
#include "ui/UIHelper.h"

#include "scenes/battle_scene_3/BattleScene.h"
#include "scenes/battle_scene_3/BSCard.h"
#include "scenes/battle_scene_3/BSResources.h"
#include "scenes/battle_scene_3/game/player_actions/PlayerAction.h"


USING_NS_CC;
//BATTLE_SCENE_USE_NS;
using namespace std;

BATTLE_SCENE_NS_BEG

// BSPrizeHolder Class //

//shared_ptr<BSPrizeHolder> BSPrizeHolder::create(Node * rootnode) {
//	auto ret = make_shared<BSPrizeHolder>(rootnode);
//	if (ret && ret->init())
//		return ret;
//	return nullptr;
//}
//BSPrizeHolder::BSPrizeHolder(cocos2d::Node *rootnode) : root(rootnode) {}
//BSPrizeHolder::~BSPrizeHolder() {}
//
//bool BSPrizeHolder::init() {
//	if (!root)
//		return false;
//
//	container = dynamic_cast<ui::Layout*>(root->getChildByName("Container"));
//	cardMarker = container->getChildByName("Card_Marker");
//
//	selectBorder = dynamic_cast<ui::ImageView*>(container->getChildByName("Select_Border"));
//	selectBorder->setVisible(false);
//
//	auto func = [this](Ref *sender, ui::Widget::TouchEventType eventType)->void {
//		switch (eventType) {
//		case ui::Widget::TouchEventType::ENDED:
//			if (onTouched)
//				onTouched(this);
//			break;
//		}
//	};
//	
//	touchPanel = dynamic_cast<ui::Layout*>(container->getChildByName("Touch_Panel"));
//	touchPanel->addTouchEventListener(func);
//
//	return true;
//}



/*
	UnselectedCardHolder Class
*/

UnselectedCardHolder* UnselectedCardHolder::create() {
	auto ret = new (nothrow) UnselectedCardHolder();
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

UnselectedCardHolder::UnselectedCardHolder() {}

UnselectedCardHolder::~UnselectedCardHolder() {
	if (root) {
		root->removeFromParent();
		ResourcePool::returnNode(root);
	}
}

bool UnselectedCardHolder::init() {
	if (!ui::Layout::init())
		return false;

	auto pool = GM_POOL;
	root = pool->tryGetNodeCsb("ccstd_csb/battle_scene/prefabs/unselected_card_holder.csb");
	if (!root)
		return false;
	this->addChild(root);

	container = dynamic_cast<ui::Layout*>(root->getChildByName("Container"));
	numPanel = dynamic_cast<ui::ImageView*>(container->getChildByName("Num_Panel"));
	numLabel = dynamic_cast<ui::Text*>(numPanel->getChildByName("Num_Label"));

	auto containerSize = container->getContentSize();
	this->setContentSize(containerSize);

	return true;
}

void UnselectedCardHolder::renderWith(const CardId &id, unsigned int num) {
	cid = id;
	stackedNum = num;

	numPanel->setVisible(stackedNum > 1);
	numLabel->setString(to_string(stackedNum));
}


/*
	CardSelector Class
*/

std::shared_ptr<BSCardSelector> BSCardSelector::create(BattleScene *scn) {
	auto ret = make_shared<BSCardSelector>(scn);
	if (ret && ret->init())
		return ret;
	return nullptr;
}

BSCardSelector::BSCardSelector(BattleScene *scn) : btlScn(scn) {}

BSCardSelector::~BSCardSelector() {}

bool BSCardSelector::init() {
	if (!btlScn)
		return false;

	auto scnroot = btlScn->getScnRoot();
	root = scnroot->getChildByName("Card_Selector");
	if (!root)
		return false;

	// Unselected Panel
	unselectedPanel = dynamic_cast<ui::Layout*>(root->getChildByName("Unselected_Panel"));
	unselectedContainer = dynamic_cast<ui::Layout*>(unselectedPanel->getChildByName("Container"));
	unselectedTouchPanel = dynamic_cast<ui::Layout*>(unselectedPanel->getChildByName("Touch_Panel"));
	unselectedPrevBtn = dynamic_cast<ui::Button*>(unselectedPanel->getChildByName("Unsel_Prev_Button"));
	unselectedNextBtn = dynamic_cast<ui::Button*>(unselectedPanel->getChildByName("Unsel_Next_Button"));

	return true;
}

void BSCardSelector::showToSelect(const std::vector<CardId> &cidVec, const SelectMap &selMap) {
	clearUI();
	
	const auto holderSize = Size(390, 567); // Phai ghi dung Size that cua holder
	auto containerSize = unselectedContainer->getContentSize();
	auto holderScale = containerSize.height / holderSize.height;

	map<CardId, unsigned int> stackedMap;
	for (const auto &cid : cidVec) {
		stackedMap[cid] += 1;
	}

	float spaceX = 20.0f;
	constexpr unsigned int MaxPerRow = 5;
	containerSize.width = MaxPerRow * holderScale * holderSize.width + spaceX * (MaxPerRow - 1) * holderScale;
	containerSize.height = containerSize.height + 50.0f;
	unselectedContainer->setContentSize(containerSize); // Thay doi kich thuoc moi
	unselectedTouchPanel->setContentSize(containerSize);
	unselectedPanel->setContentSize(containerSize);

	unsigned int i = 0;
	for (const auto &itr : stackedMap) {
		const auto &cid = itr.first;
		const auto &num = itr.second;

		auto holder = UnselectedCardHolder::create();
		holder->renderWith(cid, num);
		unselectedContainer->addChild(holder);
		holder->setPosition(Vec2(holderScale* i * (spaceX + holderSize.width), 20.0f));
		holder->setScale(holderScale);
		++i;
	}

	auto containerPos = unselectedContainer->getPosition();
	unselectedPrevBtn->setPosition(Vec2(containerPos.x - 40.0f, containerPos.y + containerSize.height / 2.0f));
	unselectedNextBtn->setPosition(Vec2(containerSize.width + 40.0f, containerPos.y + containerSize.height / 2.0f));


	//for (const auto &itr : selMap) { // Duyet tung muc phai lua chon
	//	const auto selType = itr.first;
	//	const auto selNum = itr.second;

	//	for (unsigned int i = 0; i < selNum; ++i) { // Xet tung muc phai lua chon

	//	}
	//}
}

void BSCardSelector::clearUI() {
}

BATTLE_SCENE_NS_END