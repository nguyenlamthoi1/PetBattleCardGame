#include "BattleScene.h"
#include "actions/BSAction.h"

#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/LangSys.h"
#include "common/Utilize.h"
#include "components/WidgetTouchComponent.h"
#include "define/CommonDefine.h"

#include "BSHand.h"
#include "BSDeck.h"

#include "BattleManager.h"

#include "ui/UIHelper.h"

#include <new>
#include <algorithm>

USING_NS_CC;
WIDGET_TOUCH_USE_NS;
using namespace std;
UTILIZE_USE_NS;

class PlayerData;

BATTLE_SCENE_NS_BEG;

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


BattleScene::BattleScene(){
	CCLOG("BattleScene::ctor called");
}

BattleScene::~BattleScene(){
	CCLOG("BattleScene::dtor called");
	clearPipeline();
}

bool BattleScene::init() {
	if (!Scene::init())
		return false;

	auto director = Director::getInstance();
	auto winSize = director->getWinSize();
	auto gm = GameManager::getInstance();
	auto resPool = gm->getPool();
	
	ui = static_cast<ui::Layout*>(resPool->tryGetNodeCsb("ccstd_csb/battle_scene/battle_scene.csb"));
	if (!ui)
		return false;

	// Khoi tao 2 player data
	playerData[PLAYER] = ::PlayerData::createPseudo(true);
	playerData[OPPONENT] = ::PlayerData::createPseudo(false);
	
	// Khoi tao Hand
	hands[PLAYER] = BSHand::create(this, PLAYER); 
	hands[PLAYER]->retain();
	Node *tempMarker = Utilize::getChildByName(ui, "P1_Hand_Marker");
	tempMarker->addChild(hands[PLAYER]); // *Hand duoc addChild vao BattleScene

	// Khoi tao deck
	decks[PLAYER] = BSDeck::create(this, PLAYER);
	decks[OPPONENT] = BSDeck::create(this, OPPONENT);


	ui->setContentSize(winSize);
	ui::Helper::doLayout(ui);
	this->addChild(ui);
	return true;
}

void BattleScene::onEnter() {
	Scene::onEnter();
	start();
}

void BattleScene::onExit() {
	Scene::onExit();
}

void BattleScene::start() {
	// Thiet lap cac first action
	auto firstPlayerId = PLAYER;
	auto secondPlayerId = OPPONENT;
	pushActions({
		new DrawCardAction(firstPlayerId, 10),
		new DrawCardAction(firstPlayerId, 10),
		});

	// Bat dau action pipeline
	startPipeline();
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

BATTLE_SCENE_NS_END
