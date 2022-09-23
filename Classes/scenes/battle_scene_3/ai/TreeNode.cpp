#include "TreeNode.h"

#include "../game/game_state/GameState.h"

using namespace std;

TreeNode::TreeNode(const std::shared_ptr<const MGame::GameState> & gstate) : gamestate(gstate) {
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

