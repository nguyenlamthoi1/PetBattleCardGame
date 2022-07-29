#include "BattleManager.h"
#include "BMPlayer.h"
#include "actions/BMAction.h"
USING_NS_CC;
using namespace std;

BATTLE_SCENE_NS_BEG

const string BattleManager::BATTLE_MANAGER_SCHEDULER = "BATTLE_MANAGER_SCHEDULER";

BattleManager::BattleManager(){
	
}


BattleManager::~BattleManager() {

}

void BattleManager::startPipeline() {
	auto scheduler = Director::getInstance()->getScheduler();
	scheduler->schedule(bind(&BattleManager::processPipeline, this, placeholders::_1), this, 0, false, BATTLE_MANAGER_SCHEDULER);
}

void BattleManager::processPipeline(float dt) {
	while (!actionPipeline.empty()) {
		auto first = actionPipeline.front();
		if (first->state == BMAction::State::Wait)
			first->start();
	}
}

void BattleManager::pushAction(BMAction *action) {
	if (!action)
		return;
	actionPipeline.push_back(action);
}

BATTLE_SCENE_NS_END