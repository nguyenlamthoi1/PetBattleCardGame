#include "BattleScene.h"

#include "GameManager.h"

#include "data/PlayerData.h"

//#include "BattleManager.h"
//
//#include "BSHand.h"
//#include "BSDeck.h"
//#include "BSBoard.h"
//#include "BSCard.h"
//#include "BSCoinFlipper.h"
//#include "BSNotifier.h"
//
//#include "actions/BSAction.h"
//#include "players/BSPlayer.h"
//
//#include "common/Utilize.h"
//#include "components/WidgetTouchComponent.h"
//#include "define/CommonDefine.h"
#include "data/PlayerData.h"
//
#include "ui/UIHelper.h"

#include <new>
#include <algorithm>

USING_NS_CC;
//WIDGET_TOUCH_USE_NS;
using namespace std;
//UTILIZE_USE_NS;

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


BattleScene::BattleScene()/* : btlMgr(new BattleManager(this))*/{
	CCLOG("BattleScene::Ctor called");
}

BattleScene::~BattleScene(){
	CCLOG("BattleScene::Dtor called");
	//clearPipeline();
	//detailedCard = nullptr;

#if DEBUG_CLEAN > 0
	checkClean();
#endif
}

void BattleScene::checkClean() {
//#if DEBUG_CLEAN > 0
//	for (const auto& e : pipeline) {
//		CCASSERT(e == nullptr, "checkClean:: mem leak!");
//	}
//
//	CCASSERT(detailedCard == nullptr, "checkClean:: mem leak!");
//#endif
}

bool BattleScene::init() {
	if (!Scene::init())
		return false;

	auto pool = GM_POOL;
	root = pool->tryGetNodeCsb("ccstd_csb/battle_scene/battle_scene.csb");
	ui::Helper::doLayout(root);
	if (!root)
		return false;
	auto director = Director::getInstance();
	auto winSize = director->getWinSize(); // Kich thuoc man hinh
	root->setContentSize(winSize);
	ui::Helper::doLayout(root);
	this->addChild(root);

	playerPanels[PLAYER] = dynamic_cast<ui::Layout*>(root->getChildByName("P1_Panel"));
	playerPanels[OPPONENT] = dynamic_cast<ui::Layout*>(root->getChildByName("P2_Panel"));


	// Khoi tao du lieu player trong game
	//players[PLAYER] = make_shared<BSGamer>(PLAYER);
	//players[OPPONENT] = make_shared<BSPlayer>(OPPONENT);

	// Khoi tao Hand
	//hands[PLAYER] = BSHand::create(this, PLAYER); 
	//Node *tempMarker = root->getChildByName("P1_Panel")->getChildByName("Hand_Marker");
	//tempMarker->addChild(hands[PLAYER]); // *Hand duoc addChild vao BattleScene

	//// Khoi tao deck
	//decks[PLAYER] = shared_ptr<BSDeck>(BSDeck::create(this, PLAYER));
	//decks[OPPONENT] = shared_ptr<BSDeck>(BSDeck::create(this, OPPONENT));

	//// Khoi tao Board
	//boards[PLAYER] = shared_ptr<BSBoard>(BSBoard::create(this, PLAYER));
	////boards[OPPONENT] = BSBoard::create(this, OPPONENT);

	//// Khoi tao Coin Flipper
	//coinFlipper = shared_ptr<BSCoinFlipper>(BSCoinFlipper::create(this));
	//coinFlipper->getNode()->setVisible(false);

	//// Khoi tao Notifier
	//notifier = shared_ptr<BSNotifier>(BSNotifier::create(this));
	//notifier->getNode()->setVisible(true);

	// Khoi tao Top Layer
	topLayout = dynamic_cast<ui::Layout*>(root->getChildByName("Top_Layer"));
	if (!topLayout)
		return false;
	// Khoi tao Loading Layout
	loadingLayout = dynamic_cast<ui::Layout*>(root->getChildByName("Loading_Layout"));
	if (!loadingLayout)
		return false;

	return true;
}

void BattleScene::onEnterTransitionDidFinish() {
	Scene::onEnterTransitionDidFinish();
	startLoadingAndInit();
}

void BattleScene::startLoadingAndInit() {
	// Hien thi loading layout
	loadingLayout->setVisible(true);
	loadingLayout->setOpacity(255);

	// Load cac tai nguyen trong game
	// TODO

	// Khoi tao Game State
	initGame();

	// Tat hien thi loading layout va bat dau Game
	loadingLayout->runAction(
		Sequence::create(
			DelayTime::create(0.5f),
			FadeOut::create(0.5f),
			CallFunc::create([this]() {
				loadingLayout->setVisible(false);
				}),
			nullptr));
}

void BattleScene::initGame() {
	// Khoi tao 2 player data
	auto dataMgr = GM_DATA_MGR;
	playerData[PLAYER] = dataMgr->getPlayerData();
	playerData[OPPONENT] = dataMgr->getOpponentData("OPP_0"); // TODO: Truyen tham so doi thu vao

	initTopLayer();
}

void BattleScene::initTopLayer() {
	// P1
	auto portraitImg1 = dynamic_cast<ui::ImageView*>(topLayout->getChildByName("Portrait_P1")->getChildByName("Portrait_Image"));
	portraitImg1->loadTexture(playerData[PLAYER]->getAvatarImg(), ui::Widget::TextureResType::PLIST);
	// P2
	auto portraitImg2 = dynamic_cast<ui::ImageView*>(topLayout->getChildByName("Portrait_P2")->getChildByName("Portrait_Image"));
	portraitImg2->loadTexture(playerData[OPPONENT]->getAvatarImg(), ui::Widget::TextureResType::PLIST);
}


void BattleScene::onExit() {
	Scene::onExit();
}




/*
	Action pipline members
*/

//void BattleScene::startPipeline() {
//
//	schedule(CC_SCHEDULE_SELECTOR(BattleScene::updatePipeline));
//}
//
//void BattleScene::pushActions(std::initializer_list<BSAction*> list) {
//	for (auto &action : list) {
//		action->pushedTo(this);
//	}
//}
//
//void BattleScene::updatePipeline(float dt) {
//	if(!pipeline.empty()) {
//		auto first = pipeline.front();
//		if(first->state == BSAction::State::Wait)
//			first->start();
//	}
//}
//
//void BattleScene::clearPipeline() {
//	for (auto &action : pipeline) {
//		delete action;
//		action = nullptr;
//	}
//	pipeline.clear();
//}

BATTLE_SCENE_NS_END
