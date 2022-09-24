#include "GameTree.h"
#include "TreeNode.h"

#include "../game/game_state/GameState.h"
#include "../game/game_actions/GameAction.h"

#include <algorithm>

using namespace std;

void GameTree::clear() {
	rootNode = nullptr;
	internalNodes.clear();

	turnCount = 0;
	maxTurnCount = 0;
}

void GameTree::initWithRoot(const std::shared_ptr<MGame::GameState> &gstate) {
	rootNode = make_shared<TreeNode>(gstate);

	internalNodes.clear();
	if (!rootNode->hasNextNodes())
		internalNodes.push_back(rootNode);
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


bool GameTree::canGenMore() const {
	return !internalNodes.empty();
}

bool GameTree::gen() {
	if(canGenMore())
		return genNextNodes();
	return false;
}

bool GameTree::genNextNodes() {
	std::vector<unsigned int > rmvIdxVec;
	
	unsigned int i = 0;
	unsigned int n = internalNodes.size();

	while (i < n) {
		auto curNode = internalNodes[i];
		internalNodes.pop_front();

		// Chi de kiem tra cho chac chan, Khong qua can thiet //
		if (!curNode->isLeaf()) {
			rmvIdxVec.push_back(i);
			continue;
		}
		//----------------------------------------------------//
		auto gstate = curNode->gamestate;
		if (curNode->hasNextNodes()) { // Co node tiep theo -> dang wait input
			const auto &actionQueue = gstate->getActionQueue();

			/// *Chi xet cac action can AI dua ra quyet dinh
			auto gstatePtr = (MGame::GameState*) curNode->gamestate.get();
			const auto waitInputAction = dynamic_pointer_cast<MGame::WaitInputAction>(actionQueue.front());
			auto possibleMoves = waitInputAction->getPossibleMoves(gstatePtr);
			for (const auto &move : possibleMoves) {
				// Tao node moi
				auto gstateCloned = curNode->gamestate->clone();
				auto suc = gstateCloned->onPlayerTakeAction(move) == ActionError::Succeeded;
				if (suc) {
					auto newNextNode = make_shared<TreeNode>(gstateCloned); // GameState sau khi thuc hien move
					newNextNode->prevMove = move;
					newNextNode->tryToRunActionQueue(); // * Co gang chay gstate den action GameOver hoac EmptyAction hoac WaitInput
					curNode->nexts.push_back(newNextNode); // Bo newNextNode vao danh sach node con

					if (!newNextNode->hasNextNodes())
						internalNodes.push_back(newNextNode);
				}
			}
		}

		++i;
	}

	//for (auto &curNode : internalNodes) {
	//	// Chi de kiem tra cho chac chan, Khong qua can thiet //
	//	if (!curNode->isLeaf()) { 
	//		rmvIdxVec.push_back(i);
	//		continue;
	//	}
	//	//----------------------------------------------------//
	//	auto gstate = curNode->gamestate;
	//	if (curNode->hasNextNodes()) { // Co node tiep theo -> dang wait input
	//		const auto &actionQueue = gstate->getActionQueue();

	//		/// *Chi xet cac action can AI dua ra quyet dinh
	//		auto gstatePtr = (MGame::GameState*) curNode->gamestate.get();
	//		const auto waitInputAction = dynamic_pointer_cast<MGame::WaitInputAction>(actionQueue.front());
	//		auto possibleMoves = waitInputAction->getPossibleMoves(gstatePtr);
	//		for (const auto &move : possibleMoves) {
	//			// Tao node moi
	//			auto gstateCloned = curNode->gamestate->clone();
	//			auto suc = gstateCloned->onPlayerTakeAction(move) == ActionError::Succeeded;
	//			if (suc) {
	//				auto newNextNode = make_shared<TreeNode>(gstateCloned); // GameState sau khi thuc hien move
	//				newNextNode->prevMove = move;
	//				newNextNode->tryToRunActionQueue(); // * Co gang chay gstate den action GameOver hoac EmptyAction hoac WaitInput
	//				curNode->nexts.push_back(newNextNode); // Bo newNextNode vao danh sach node con

	//				if (!newNextNode->hasNextNodes())
	//					internalNodes.push_back(newNextNode);
	//			}
	//		}
	//	}
	//	++i;
	//}

	bool ret = rmvIdxVec.size() == n;

	for (const auto &idx : rmvIdxVec) {
		internalNodes.erase(internalNodes.begin() + idx);
	}
	
	return ret;
	
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
