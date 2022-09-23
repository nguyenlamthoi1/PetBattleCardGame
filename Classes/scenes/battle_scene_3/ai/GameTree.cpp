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
	genNextNodes(rootNode);
}

void GameTree::genNextNodes(const std::shared_ptr<TreeNode> &curNode) {
	auto &actionQueue = curNode->gamestate->getActionQueue();
	if (actionQueue.empty());
		return;

	const auto &firstAction = actionQueue.front();
	auto waitInputAction = dynamic_pointer_cast<MGame::WaitInputAction>(firstAction);
	if (waitInputAction && waitInputAction->state != MGame::GameAction::State::Done) { // Kiem tra co phai input khong
		/// *Chi xet cac action can AI dua ra quyet dinh
		auto gstatePtr = (MGame::GameState*) curNode->gamestate.get();
		auto possibleMoves = waitInputAction->getPossibleMoves(gstatePtr);
		for (const auto &move : possibleMoves) {
			// Tao node moi
			auto gstateCloned = curNode->gamestate->clone();
			auto suc = gstateCloned->onPlayerTakeAction(move) == ActionError::Succeeded;
			if (suc) {
				auto newNextNode = make_shared<TreeNode>(gstateCloned); // GameState sau khi thuc hien move
				newNextNode->prevMove = move;
				newNextNode->tryToRunActionQueue(); // * Co gang chay gstate den action GameOver hoac EmptyAction
				curNode->nexts.push_back(newNextNode); // Bo newNextNode vao danh sach node con
			}
		}
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
