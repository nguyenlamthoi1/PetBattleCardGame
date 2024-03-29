#include "BattleMaster.h"
#include "game_state/GameState.h"
#include "game_actions/GameAction.h"
#include "../BattleScene.h"

#include "GameManager.h"
#include "data/PlayerData.h"

using namespace std;
USING_NS_CC;

NS_GAME_BEGIN

BattleMaster::BMPtr BattleMaster::instance;

BattleMaster::BMPtr BattleMaster::get() {
	if (!instance)
		instance = make_shared<BattleMaster>();
	return instance;
}

BattleMaster::BattleMaster() : 
	gstate(make_shared<GameState>())
{
}

BattleMaster::~BattleMaster(){
	//endGame();
}

bool BattleMaster::initGame(const PlayerIdType &p1, const PlayerIdType &p2) {
	clearGame();

	pids.clear(); // Vector chua id 2 nguoi choi
	pids.emplace_back(p1);
	pids.emplace_back(p2);

	auto dtaMgr = GM_DATA_MGR;
	for (const auto &id : pids) {
		playerData[id] = dtaMgr->getPlayerData(id);
	}

	// Init Game State
	gstate->initStateWith2Players(p1, p2);
	
	return true;
}

void BattleMaster::clearGame() {

}

const string BattleMaster::GAME_LOOP_SCHEDULER = "GAME_LOOP_SCHEDULER";

bool BattleMaster::startGame(BattleSceneNS::BattleScene *scn) {
	btlScn = scn;
	gstate->startGame();
	Director::getInstance()->getScheduler()->schedule(bind(&BattleMaster::gameLoop, this, placeholders::_1), this, 0, false, GAME_LOOP_SCHEDULER);
	return true;
}

void BattleMaster::endGame() {
	Director::getInstance()->getScheduler()->unschedule(GAME_LOOP_SCHEDULER, this);
}

void BattleMaster::gameLoop(float t) {
	if (gstate->isGameOver())
		endGame();

	//auto& actionQueue = gstate->actionQueue;
	//auto btlScn = BattleSceneNS::BattleScene::getScene();

	//if (!actionQueue.empty()) {
	//	auto curAction = actionQueue.front();
	//	if (curAction->state == GameAction::State::Wait) {
	//		curAction->executeOn(gstate); // Thuc thi tung action

	//		// Them animation cho scene thuc thi
	//		auto bsAction = curAction->getBSAction();
	//		if(bsAction)
	//			btlScn->pushAction(bsAction);
	//	}
	//	else if (curAction->state == GameAction::State::Done)
	//		actionQueue.pop_front();
	//}

	//auto btlScn = BattleSceneNS::BattleScene::getScene();
	auto bsAction = gstate->progressGame();
	if (bsAction)
		btlScn->pushAction(bsAction);
}

std::shared_ptr<GameAction> BattleMaster::getCurrentAction() const {
	return gstate->actionQueue.front();
}

void BattleMaster::onPlayerDoEndGame() {
	//TODO
}
void BattleMaster::onPlayerDoRestartGame() {

}

ActionError BattleMaster::onPlayerChooseAction(const shared_ptr<PlayerAction> &action) {
	return gstate->onPlayerTakeAction(action);
}


NS_GAME_END
