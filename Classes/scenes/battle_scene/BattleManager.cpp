#include "BattleManager.h"
#include "BattleScene.h"
#include "BSPlayer.h"
#include "actions/BSAction.h"

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

BATTLE_SCENE_NS_BEG

const string BattleManager::BATTLE_MANAGER_SCHEDULER = "BATTLE_MANAGER_SCHEDULER";
const string BattleManager::BATTLE_MANAGER_ACTION_SCHEDULER = "BATTLE_MANAGER_ACTION_SCHEDULER";


BattleManager::BattleManager(BattleScene *scn) : btlScn(scn) {
	CCLOG("BattleManager::Ctor %p", this);
}

BattleManager::~BattleManager() {
	CCLOG("BattleManager::Dtor %p", this);
	endGame();
}

void BattleManager::startGame() {
	players[PLAYER] = make_shared<BSPlayer>(PLAYER);
	players[OPPONENT] = make_shared<BSPlayer>(OPPONENT);

	gameState = GameState::NONE;
	changeState(gameState, GameState::START);
	Director::getInstance()->getScheduler()->schedule(bind(&BattleManager::doGameLoop, this, placeholders::_1), this, 0, CC_REPEAT_FOREVER, 0, false, BATTLE_MANAGER_SCHEDULER);
}

void BattleManager::endGame() {
	Director::getInstance()->getScheduler()->unschedule(BATTLE_MANAGER_SCHEDULER, this);
	Director::getInstance()->getScheduler()->unschedule(BATTLE_MANAGER_ACTION_SCHEDULER, this);
}

void BattleManager::doGameLoop(float dt) {
	//if (gameState == GameState::START) {
	//	players[PLAYER]->drawFromDeck(BEGIN_TURN_DRAW_COUNT);
	//	players[OPPONENT]->drawFromDeck(BEGIN_TURN_DRAW_COUNT);
	//}
}

void BattleManager::changeState(GameState fromState, GameState toState) {
	if (toState == GameState::NONE)
		return;

	if (fromState == GameState::NONE) {
		if (toState == GameState::START) { // NONE -> START
			// Thiet lap cac first action
			startPipeline({
				new DrawCardAction(btlScn->getBattleManager(), PLAYER, BEGIN_TURN_DRAW_COUNT),
				//new DrawCardAction(btlScn->getBattleManager(), OPPONENT, BEGIN_TURN_DRAW_COUNT), //TODO
				new CustomAction([this]() {
					changeState(gameState, GameState::SET_UP);
					})
				});
			gameState = toState;
			return;
		}
	}
	
	return;
}

void BattleManager::startPipeline(initializer_list<BSAction*> actionList) {
	CCASSERT(pipState != PipelineState::DOING, "BattleManager::startPipeline: pipeline is being processed");

	for (const auto& action : actionList) {
		if (action->state == BSAction::State::Wait) {
			pipeline.push_back(shared_ptr<BSAction>(action));
			action->pipIdx = pipeline.size() - 1;
		}
	}

	pipState = PipelineState::DOING;
	Director::getInstance()->getScheduler()->schedule(bind(&BattleManager::updatePipeline, this, placeholders::_1), this, 0, CC_REPEAT_FOREVER, 0, false, BATTLE_MANAGER_ACTION_SCHEDULER);
}

void BattleManager::updatePipeline(float dt) {
	if (pipState != PipelineState::DOING)
		return;

	if (!pipeline.empty()) {
		auto first = *(pipeline.begin());
		if (first->state == BSAction::State::Wait) {
			pipState = PipelineState::DOING;
			first->start();
		}
		else if (first->state == BSAction::State::Done) { // Action ket thuc
			//pipeline.erase(pipeline.begin()); // Xoa action
			pipeline.pop_front();
		}
	}
	
	if (pipeline.empty()) {
		pipState = PipelineState::WAIT;
		Director::getInstance()->getScheduler()->unschedule(BATTLE_MANAGER_ACTION_SCHEDULER, this);
	}
}

void BattleManager::onActionEnd(std::shared_ptr<BSAction> endedaAction) {
	//TODO
}



BATTLE_SCENE_NS_END