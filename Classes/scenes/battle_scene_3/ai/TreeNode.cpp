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


std::string TreeNode::getCurPid() const {
	return gamestate->getCurrentId();
}

void TreeNode::tryToRunActionQueue() {
	
	unsigned int max = 100;
	unsigned int i = 0;
	while (true) {
		bool shouldStop = false;

		bool isGameOver = gamestate->isGameOver(); // Tro choi ket thuc
		shouldStop &= isGameOver;

		auto &actionQueue = gamestate->getActionQueue(); // Action Queue trong
		shouldStop &= actionQueue.empty();

		auto firstAction = actionQueue.front();
		auto waitInputAction = dynamic_pointer_cast<MGame::WaitInputAction>(firstAction);
		shouldStop &= waitInputAction != nullptr; // Den luc make decision tiep theo

		shouldStop &= i >= max;

		if (shouldStop)
			break;

		gamestate->progressGameNoAnimation();
		++i;
	}
}

