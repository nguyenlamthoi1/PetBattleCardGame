#include "TreeNode.h"

#include "../game/game_state/GameState.h"
#include "../game/game_actions/GameAction.h"


using namespace std;

TreeNode::TreeNode(const std::shared_ptr<MGame::GameState> & gstate) : gamestate(gstate) {
}

TreeNode::~TreeNode() {

}

int TreeNode::calValue() const  {

	return 0;
}

bool TreeNode::isLeaf() const {
	return nexts.empty();
}

bool TreeNode::hasNextNodes() const {
	if (gamestate == nullptr)
		return false;

	//bool isGameOver = gamestate->isGameOver(); // Tro choi ket thuc
	//if(isGameOver)
	//	return false;

	//bool emptyQueue = gamestate->actionQueueEmpty(); // Action Queue trong
	//if (emptyQueue)
	//	return false;

	bool waitInput = gamestate->isWaitingInput();
	return waitInput;
}


std::string TreeNode::getCurPid() const {
	return gamestate->getCurrentId();
}

void TreeNode::tryToRunActionQueue() {
	
	unsigned int max = 100;
	unsigned int i = 0;
	while (true) {
		bool shouldStop = false;

		bool isGameOver = gamestate->isGameOver(); 
		shouldStop |= isGameOver; // Tro choi ket thuc

		bool emptyQueue = gamestate->actionQueueEmpty(); 
		shouldStop |= emptyQueue; // Action Queue empty

		bool waitInput = gamestate->isWaitingInput();
		shouldStop |= waitInput; // Khong dang doi input

		shouldStop |= i >= max; // Lap lai qua nhieu action gan nhu vo tan

		if (shouldStop) {
			CCASSERT(i < max, "Why run forever");
			break;
		}
		
		gamestate->progressGameNoAnimation();
		++i;
	}
}

