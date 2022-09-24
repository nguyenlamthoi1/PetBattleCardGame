#include "AIMinimax.h"
#include "GameTree.h"
#include "TreeNode.h"

#include "../game/player_actions/PlayerAction.h"
#include "../game/game_state/GameState.h"
#include "../BattleScene.h"


using namespace std;
USING_NS_CC;

AIMinimax::AIMinimax() {
	tree = make_shared<GameTree>();
}

AIMinimax::~AIMinimax() {
	Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
}

const string AIMinimax::AI_SCHEDULER = "AI_SCHEDULER";

void AIMinimax::startThinking(BattleSceneNS::BattleScene *scn, const std::shared_ptr<MGame::GameState> &gamestate) {
	btlScn = scn;
	orgGamestate = gamestate->clone();
	chosenMove.reset(); // Reset ve Nullptr
	
	// Thiet lap ban dau
	curState = State::UPDATE_TREE;
	tree->clear(); // Xoa Tree Cu
	tree->initWithRoot(gamestate); // Khoi tao Node Root
	tree->setMaxTurnCount(3); // Xet So luot di tinh
	thinkTime = 0; // Thoi gian da suy nghi
	maxThinkTime = 1.0f; // Thoi gian suy nghi toi da

	numFloorToTraversed = 5;
	numFloorInc = 1;

	// Bat dau suy nghi
	Director::getInstance()->getScheduler()->schedule(bind(&AIMinimax::thinkLoop, this, placeholders::_1), this, 0.0f, false, AI_SCHEDULER);
}

void AIMinimax::thinkLoop(float dt) {
	if (thinkTime > maxThinkTime) { // Neu nhu qua thoi gian suy nghi thi phai dua ra move lien
		if (!chosenMove) { // Khong tim thay thi danh ngau nhien
			chosenMove = make_shared<MGame::PA_DoForMe>(orgGamestate->getCurrentId());
		}
		curState = State::DONE;
	}

	if (curState == State::UPDATE_TREE) {
		if (!tree->canGenMore()) { // Chua Gen du cac node de tinh toan
			tree->gen(); // Tao cac cac node con
		}
		else { // Da gen du tat ca cac node co the duyet
			curState == State::TRAVERSE_TREE;
		}
	}
	else if (curState == State::TRAVERSE_TREE)
	{
		if (numFloorToTraversed <= tree->maxFloorGened) {
			auto node = tree->getBestNextNode(numFloorToTraversed);
			chosenMove = node->getPreMove();
			numFloorToTraversed += numFloorInc; // Tang dan do sau
		}
		else {
			curState = State::DONE;
		}
	}
	else if (curState == State::DONE) {
		onDoneThinking();
	}

	thinkTime += dt; // Thoi gian frame truoc do suy nghi
}

void AIMinimax::onDoneThinking() {
	// Ket thuc think loop
	Director::getInstance()->getScheduler()->unschedule(AI_SCHEDULER, this);
	if(chosenMove)
		bool suc = btlScn->onPlayerDoAction(chosenMove);
}
