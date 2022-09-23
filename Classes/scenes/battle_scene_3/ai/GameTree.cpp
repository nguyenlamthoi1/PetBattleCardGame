#include "GameTree.h"
#include "TreeNode.h"

#include "../game/game_state/GameState.h"
#include "../game/game_actions/GameAction.h"

#include <algorithm>

using namespace std;

void GameTree::clear() {
	rootNode = nullptr;
	turnCount = 0;
	maxTurnCount = 0;
}

void GameTree::initWithRoot(const std::shared_ptr<const MGame::GameState> &gstate) {
	rootNode = make_shared<TreeNode>(gstate);
}


bool GameTree::isReachMaxDepth() const {
	return turnCount >= maxTurnCount;
}

void GameTree::setMaxTurnCount(unsigned int n) {
	maxTurnCount = n;
}

void GameTree::setMaxFloorToGen(unsigned int n) {
	maxFloorToGenEachFrame = n;
}

void GameTree::gen() {

}

void GameTree::genNextNodes(const std::shared_ptr<TreeNode> &curNode) {
	auto &actionQueue = curNode->gamestate->getActionQueue();
	if (actionQueue.empty());
		return;

	const auto &firstAction = actionQueue.front();
	auto waitInputAction = dynamic_pointer_cast<MGame::WaitInputAction>(firstAction);
	if (waitInputAction && waitInputAction->state != MGame::GameAction::State::Done) { // Kiem tra co phai input khong

	}
}


shared_ptr<const TreeNode> GameTree::getBestNextNode(unsigned int maxFloor) {
	maxTraverseFloor = maxFloor; // Xet do sau cho qua trinh duyet
	auto res = minimax(rootNode, true, 0);
	return res.node;
}

GameTree::Result GameTree::minimax(const std::shared_ptr<const TreeNode> &curNode, bool isMaxPlayer, unsigned int floor) {
	if (floor == maxTraverseFloor || curNode->isLeaf()) {
		auto res = Result({curNode, curNode->calValue()});
		return res;
	}

	string curPid = curNode->getCurPid();

	if (isMaxPlayer) {
		int bestVal = -INT_MAX;
		for (const auto node : curNode->nexts) {
			auto ret = minimax(node, node->getCurPid() == curPid, floor + 1);
			bestVal = max(bestVal, ret.val);
		}
	}
	else {
		int bestVal = INT_MAX;
		for (const auto node : curNode->nexts) {
			auto ret = minimax(node, node->getCurPid() == curPid, floor + 1);
			bestVal = min(bestVal, ret.val);
		}
	}
}
