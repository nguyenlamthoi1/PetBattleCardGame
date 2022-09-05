#include "BattleScene.h"
#include "BattleManager.h"

#include "BSHand.h"
#include "BSDeck.h"
#include "BSBoard.h"
#include "BSCard.h"
#include "BSCoinFlipper.h"
#include "BSNotifier.h"

#include "actions/BSAction.h"
#include "players/BSPlayer.h"

#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/DataManager.h"
#include "common/LangSys.h"
#include "common/Utilize.h"
#include "components/WidgetTouchComponent.h"
#include "define/CommonDefine.h"
#include "data/PlayerData.h"

#include "ui/UIHelper.h"

#include <new>
#include <algorithm>

USING_NS_CC;
WIDGET_TOUCH_USE_NS;
using namespace std;
UTILIZE_USE_NS;

class PlayerData;

BATTLE_SCENE_NS_BEG;

BattleScene* BattleScene::getScene() {
	return dynamic_cast<BattleScene*>(Director::getInstance()->getRunningScene());
}


BattleScene* BattleScene::create() {
	auto scn = new (nothrow) BattleScene();
	if (scn && scn->init()) {
		scn->autorelease();
		return scn;
	}
	else {
		delete scn;
		scn = nullptr;
	}
	
	return scn;
}

BattleScene* BattleScene::getScn() {
	return dynamic_cast<BattleScene*>(Director::getInstance()->getRunningScene());
}


BattleScene::BattleScene() : btlMgr(new BattleManager(this)){
	CCLOG("BattleScene::Ctor called");
}

BattleScene::~BattleScene(){
	CCLOG("BattleScene::Dtor called");
	clearPipeline();
	detailedCard = nullptr;

#if DEBUG_CLEAN > 0
	checkClean();
#endif
}

void BattleScene::checkClean() {
#if DEBUG_CLEAN > 0
	for (const auto& e : pipeline) {
		CCASSERT(e == nullptr, "checkClean:: mem leak!");
	}

	CCASSERT(detailedCard == nullptr, "checkClean:: mem leak!");
#endif
}

bool BattleScene::init() {
	if (!Scene::init())
		return false;

	auto director = Director::getInstance();
	auto winSize = director->getWinSize();
	auto pool = GM_POOL;
	
	root = static_cast<ui::Layout*>(pool->tryGetNodeCsb("ccstd_csb/battle_scene/battle_scene.csb"));
	ui::Helper::doLayout(root);
	if (!root)
		return false;

	root->setContentSize(winSize);
	ui::Helper::doLayout(root);
	this->addChild(root);

	playerPanels[PLAYER] = dynamic_cast<ui::Layout*>(root->getChildByName("P1_Panel"));
	playerPanels[OPPONENT] = dynamic_cast<ui::Layout*>(root->getChildByName("P2_Panel"));

	// Khoi tao 2 player data
	auto dataMgr = GM_DATA_MGR;
	playerData[PLAYER] = dataMgr->getPlayerData();
	oppId = "OPP_0";
	playerData[OPPONENT] = dataMgr->getOpponentData(oppId);

	// Khoi tao du lieu player trong game
	//players[PLAYER] = make_shared<BSGamer>(PLAYER);
	//players[OPPONENT] = make_shared<BSPlayer>(OPPONENT);

	// Khoi tao Hand
	hands[PLAYER] = BSHand::create(this, PLAYER); 
	Node *tempMarker = root->getChildByName("P1_Panel")->getChildByName("Hand_Marker");
	tempMarker->addChild(hands[PLAYER]); // *Hand duoc addChild vao BattleScene

	// Khoi tao deck
	decks[PLAYER] = shared_ptr<BSDeck>(BSDeck::create(this, PLAYER));
	decks[OPPONENT] = shared_ptr<BSDeck>(BSDeck::create(this, OPPONENT));

	// Khoi tao Board
	boards[PLAYER] = shared_ptr<BSBoard>(BSBoard::create(this, PLAYER));
	//boards[OPPONENT] = BSBoard::create(this, OPPONENT);

	// Khoi tao Coin Flipper
	coinFlipper = shared_ptr<BSCoinFlipper>(BSCoinFlipper::create(this));
	coinFlipper->getNode()->setVisible(false);

	// Khoi tao Notifier
	notifier = shared_ptr<BSNotifier>(BSNotifier::create(this));
	notifier->getNode()->setVisible(true);

	// Khoi tao Top Layer
	initTopLayer();

	return true;
}

void BattleScene::initTopLayer() {
	topLayout = dynamic_cast<ui::Layout*>(root->getChildByName("Top_Layer"));

	// P1
	auto portraitImg1 = dynamic_cast<ui::ImageView*>(topLayout->getChildByName("Portrait_P1")->getChildByName("Portrait_Image"));
	portraitImg1->loadTexture(playerData[PLAYER]->getAvatarImg(), ui::Widget::TextureResType::PLIST);
	// P2
	auto portraitImg2 = dynamic_cast<ui::ImageView*>(topLayout->getChildByName("Portrait_P2")->getChildByName("Portrait_Image"));
	portraitImg2->loadTexture(playerData[OPPONENT]->getAvatarImg(), ui::Widget::TextureResType::PLIST);
}

void BattleScene::onEnter() {
	Scene::onEnter();
	start();
}

void BattleScene::onExit() {
	Scene::onExit();
}

void BattleScene::start() {
	btlMgr->startGame();
}

/*
	Action pipline members
*/

void BattleScene::startPipeline() {

	schedule(CC_SCHEDULE_SELECTOR(BattleScene::updatePipeline));
}

void BattleScene::pushActions(std::initializer_list<BSAction*> list) {
	for (auto &action : list) {
		action->pushedTo(this);
	}
}


void BattleScene::updatePipeline(float dt) {
	if(!pipeline.empty()) {
		auto first = pipeline.front();
		if(first->state == BSAction::State::Wait)
			first->start();
	}
}

void BattleScene::clearPipeline() {
	for (auto &action : pipeline) {
		delete action;
		action = nullptr;
	}
	pipeline.clear();
}

void BattleScene::showCardDetails(const std::shared_ptr<const CardData> &data) {
	if (detailedCard) {
		hideCardDetails();
	}
	detailedCard = BSCard::createWithData(data);
	detailedCard->removeFromParent();
	this->addChild(detailedCard, BATTLE_SCENE_Z::DETAILED_CARD);

	auto dir = Director::getInstance();
	auto vSize = dir->getVisibleSize();
	detailedCard->setPosition(Vec2(vSize.width / 2, vSize.height / 2 + 20));
	detailedCard->setVisible(true);
	detailedCard->setNormalSize();
	//detailedCard->setScale(1.0f);
}

void BattleScene::hideCardDetails() {
	if (detailedCard) {
		detailedCard->returnToPool();
		detailedCard->removeFromParent();
		detailedCard = nullptr;
	}
}
	

void BattleScene::clearCardDetails() {
	if (detailedCard) {
		detailedCard->removeFromParent();
		detailedCard = nullptr;
	}
}

BATTLE_SCENE_NS_END
