#include "BattleScene.h"
#include "BattleManager.h"

#include "BSHand.h"
#include "BSDeck.h"
#include "BSBoard.h"
#include "BSPlayer.h"
#include "BSCard.h"

#include "actions/BSAction.h"

#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/DataManager.h"
#include "common/LangSys.h"
#include "common/Utilize.h"
#include "components/WidgetTouchComponent.h"
#include "define/CommonDefine.h"

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

	clearCardDetails();
}

bool BattleScene::init() {
	if (!Scene::init())
		return false;

	auto director = Director::getInstance();
	auto winSize = director->getWinSize();
	auto gm = GameManager::getInstance();
	auto resPool = gm->getPool();
	
	ui = static_cast<ui::Layout*>(resPool->tryGetNodeCsb("ccstd_csb/battle_scene/battle_scene.csb"));
	ui::Helper::doLayout(ui);
	if (!ui)
		return false;

	playerPanels[PLAYER] = dynamic_cast<ui::Layout*>(ui->getChildByName("P1_Panel"));
	playerPanels[OPPONENT] = dynamic_cast<ui::Layout*>(ui->getChildByName("P2_Panel"));

	// Khoi tao 2 player data
	auto dataMgr = GM_DATA_MGR;
	playerData[PLAYER] = dataMgr->getPlayerData();
	playerData[OPPONENT] = ::PlayerData::createPseudo(false);

	// Khoi tao du lieu player trong game
	players[PLAYER] = make_shared<BSPlayer>(PLAYER);
	players[OPPONENT] = make_shared<BSPlayer>(OPPONENT);

	
	// Khoi tao Hand
	hands[PLAYER] = BSHand::create(this, PLAYER); 
	hands[PLAYER]->retain();
	Node *tempMarker = ui->getChildByName("P1_Panel")->getChildByName("Hand_Marker");
	tempMarker->addChild(hands[PLAYER]); // *Hand duoc addChild vao BattleScene

	// Khoi tao deck
	decks[PLAYER] = BSDeck::create(this, PLAYER);
	//decks[OPPONENT] = BSDeck::create(this, OPPONENT);

	// Khoi tao Board
	boards[PLAYER] = BSBoard::create(this, PLAYER);
	boards[OPPONENT] = BSBoard::create(this, OPPONENT);

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
		new DrawCardAction(firstPlayerId, 7),
		new DrawCardAction(firstPlayerId, 7),
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

void BattleScene::showCardDetails(const std::shared_ptr<const CardData> &data) {
	if (detailedCard) {
		detailedCard->removeFromParent();
		detailedCard = nullptr;
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
	clearCardDetails();
}

void BattleScene::clearCardDetails() {
	if (detailedCard) {
		detailedCard->removeFromParent();
		detailedCard = nullptr;
	}
}

BATTLE_SCENE_NS_END
