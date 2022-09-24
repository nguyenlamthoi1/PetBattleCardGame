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
	capacity = 1000;
	genedNum = 0;
}

void GameTree::initWithRoot(const std::shared_ptr<MGame::GameState> &gstate) {
	rootNode = make_shared<TreeNode>(gstate);
	genedNum = 0;
	internalNodes.clear();
	if (rootNode->hasNextNodes())
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
	return !internalNodes.empty() && genedNum < capacity;
}

bool GameTree::gen(unsigned int numGen) {
	if(canGenMore())
		return genNextNodes();
	return false;
}

bool GameTree::genNextNodes() {
	std::vector<unsigned int > rmvIdxVec;
	
	unsigned int i = 0;
	unsigned int n = internalNodes.size();
	unsigned int numToGen = 50;
	unsigned int preGenNum = genedNum;
	while (i < n && genedNum < capacity) {
		if (genedNum - preGenNum >= numToGen)
			break;

		auto curNode = internalNodes[0];
		internalNodes.pop_front();

		// Chi de kiem tra cho chac chan, Khong qua can thiet //
		if (!curNode->hasNextNodes()) {
			rmvIdxVec.push_back(i);
			++i;
			continue;
		}
		//----------------------------------------------------//

		auto curTurn = curNode->gamestate->getTurnCount();
		if (curTurn >= maxTurnCount){ // Khong can thiet tao them nua
			++i;
			continue;
		}

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
				//tempfix
				
				auto suc = gstateCloned->onPlayerTakeAction(move) == ActionError::Succeeded;
				if (suc) {
					auto newNextNode = make_shared<TreeNode>(gstateCloned); // GameState sau khi thuc hien move
					newNextNode->prevMove = move;
					newNextNode->tryToRunActionQueue(); // * Co gang chay gstate den action GameOver hoac EmptyAction hoac WaitInput
					curNode->nexts.push_back(newNextNode); // Bo newNextNode vao danh sach node con

					if (newNextNode->hasNextNodes())
						internalNodes.push_back(newNextNode);

					genedNum++;
				}
				else {
					CCASSERT(suc, "Why move not possible ?!");
				}
			}
		}

		++i;
	}

	CCASSERT(rmvIdxVec.empty(), "Tai sao lai remove node");

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
	if (curNode == nullptr)
	{
		CCLOG("ERROR_1");
	}
	
	if (floor == maxTraverseFloor || curNode->isLeaf()) {
		auto res = Result({curNode, curNode->calValue()});
		return res;
	}

	string curPid = curNode->getCurPid();

	Result ret;
	if (isMaxPlayer) {
		ret.val = -INT_MAX;
		for (const auto node : curNode->nexts) {
			auto result = minimax(node, node->getCurPid() == curPid, floor + 1);
			if (ret.val <= result.val) {
				ret.val = result.val;
				ret.node = node;
				if (ret.node == nullptr) {
					CCLOG("EMPTY_2");
				}
			}
		}
	}
	else {
		ret.val = INT_MAX;
		for (const auto node : curNode->nexts) {
			auto result = minimax(node, node->getCurPid() == curPid, floor + 1);
			if (ret.val >= result.val) {
				ret.val = result.val;
				ret.node = node;
			}
			if (ret.node == nullptr) {
				CCLOG("EMPTY_3");
			}
		}
	}

	return ret;
}
