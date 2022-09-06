#include "BattleScene.h"

#include "GameManager.h"
#include "game/BattleMaster.h"

#include "data/PlayerData.h"
#include "scene_actions/BSAction.h"

#include "BSResources.h"
#include "BSHand.h"
#include "BSDeck.h"

#include <stdlib.h>

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

BattleScene *BattleScene::curBtlScn = nullptr;

std::shared_ptr<BSResources> BattleScene::getResources() {
	return getScene()->bsres;
}

BattleScene* BattleScene::getScene() {
	return curBtlScn;
}

BattleScene* BattleScene::getScn() {
	return curBtlScn;
}

BattleScene* BattleScene::create() {
	auto scn = new (nothrow) BattleScene();
	curBtlScn = scn;
	if (scn && scn->init()) {
		scn->autorelease();
		return scn;
	}
	else {
		delete scn;
		scn = nullptr;
		curBtlScn = nullptr;
	}
	
	return scn;
}

BattleScene::BattleScene() :
bsres(make_shared<BSResources>())
{
}

BattleScene::~BattleScene(){
	curBtlScn = nullptr;
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

	pid = "Player";
	oid = "OPP_0";

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

	playerPanels[pid] = dynamic_cast<ui::Layout*>(root->getChildByName("P1_Panel"));
	playerPanels[oid] = dynamic_cast<ui::Layout*>(root->getChildByName("P2_Panel"));

	// Khoi tao 2 player data
	auto dataMgr = GM_DATA_MGR;
	playerData[pid] = dataMgr->getPlayerData();
	playerData[oid] = dataMgr->getOpponentData("OPP_0"); // TODO: Truyen tham so doi thu vao

	// Khoi tao Hand
	auto handLayout = dynamic_cast<ui::Layout*>(root->getChildByName("P1_Hand_Panel"));
	hands.insert({ pid, BSHand::createShPtr(handLayout, pid) });
	handLayout = dynamic_cast<ui::Layout*>(root->getChildByName("P2_Hand_Panel"));
	hands.insert({ oid, BSHand::createShPtr(handLayout, oid) });

	auto playerPanel = root->getChildByName("P1_Panel");
	auto opponentPanel = root->getChildByName("P2_Panel");

	// Khoi tao deck
	auto deckNode = playerPanel->getChildByName("Deck");
	decks[pid] = BSDeck::createShPtr(deckNode, pid);
	deckNode = opponentPanel->getChildByName("Deck");
	decks[oid] = BSDeck::createShPtr(deckNode, oid);

	// Khoi tao du lieu player trong game
	//players[PLAYER] = make_shared<BSGamer>(PLAYER);
	//players[OPPONENT] = make_shared<BSPlayer>(OPPONENT);
	
	

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
	initTopLayer();

	// Khoi tao Loading Layout
	loadingLayout = dynamic_cast<ui::Layout*>(root->getChildByName("Loading_Layout"));
	if (!loadingLayout)
		return false;
	loadingLayout->setVisible(true);
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

	// Init Scene
	initGame();

	// Tat hien thi loading layout va bat dau Game
	loadingLayout->runAction(
		Sequence::create(
			DelayTime::create(0.5f),
			FadeOut::create(0.5f),
			CallFunc::create([this]() {
				loadingLayout->setVisible(false);
				startGame();
				}),
			nullptr));
}

void BattleScene::initGame() {
}

void BattleScene::startGame() {
	//bm->startGame();

	/*pushActions({
		CustomAction::createShPtr([]() {
			CCLOG("Clear Screen");
			}),
		WaitAction::createShPtr(1.0f),
		DrawCardAction::createShPtr(pid, 7, {"P1", "P1", "P2", "P2", "P3", "E1", "E2", "E2", "E3"})
		});*/

	startPipeline();

	auto bm = MGame::BattleMaster::get();
	bm->initGame(PLAYER_ID, oid);
	bm->startGame();
}

void BattleScene::initTopLayer() {
	// P1
	auto portraitImg1 = dynamic_cast<ui::ImageView*>(topLayout->getChildByName("Portrait_P1")->getChildByName("Portrait_Image"));
	portraitImg1->loadTexture(playerData[pid]->getAvatarImg(), ui::Widget::TextureResType::PLIST);
	// P2
	auto portraitImg2 = dynamic_cast<ui::ImageView*>(topLayout->getChildByName("Portrait_P2")->getChildByName("Portrait_Image"));
	portraitImg2->loadTexture(playerData[oid]->getAvatarImg(), ui::Widget::TextureResType::PLIST);
}


void BattleScene::onExit() {
	Scene::onExit();
}

///-------------------///
///Getters and Setters///
///------------------///

std::shared_ptr<PlayerData> BattleScene::getPlayerData(const PlayerIdType &id) {
	return playerData[id]; 
}
std::shared_ptr<BSHand> BattleScene::getHand(const PlayerIdType &id) { return hands[id]; }
std::shared_ptr<BSDeck> BattleScene::getDeck(const PlayerIdType &id) { return decks[id]; }


///----------------------///
///Action pipline members///
///----------------------///

const string BattleScene::PIPELINE_SCHEDULER = "PIPELINE_SCHEDULER";

void BattleScene::updatePipeline(float dt) {
	//if (waitTime > 0.0f) {
	//	waitTime -= dt;
	//	return;
	//}

	if (!pipeline.empty()) {
		auto first = *(pipeline.begin());
		if (first->state == BSAction::State::Wait) {
			pipState = PipelineState::Process;
			first->executeOn(this);
		}
		else if (first->state == BSAction::State::Done) { // Action ket thuc
			pipeline.pop_front();
		}
	}
}

void BattleScene::startPipeline() {
	schedule(CC_SCHEDULE_SELECTOR(BattleScene::updatePipeline));
}

void BattleScene::stopPipeline() {
	unschedule(CC_SCHEDULE_SELECTOR(BattleScene::updatePipeline));
}

void BattleScene::pushAction(const ActionPtr &ptr) {
	pipeline.push_back(ptr);
}

void BattleScene::pushActions(initializer_list<ActionPtr> list) {
	for (const auto &action : list)
		pipeline.push_back(action);
}

void BattleScene::clearPipeline() {
	pipeline.clear();
}

BATTLE_SCENE_NS_END
