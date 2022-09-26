#include "GameTree.h"
#include "TreeNode.h"

#include "../game/game_state/GameState.h"
#include "../game/game_state/PrizePile.h"
#include "../game/game_state/Board.h"
#include "../game/game_state/Holder.h"

#include "../game/game_actions/GameAction.h"


#include <algorithm>

using namespace std;

GameTree::GameTree(const std::string &id) : pid(id) {

}

GameTree::~GameTree(){}

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
	if (floor == maxTraverseFloor || curNode->isLeaf()) {
		auto hVal = calVal(curNode);
		auto res = Result({curNode, hVal });
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
		}
	}

	return ret;
}

int GameTree::calVal(const std::shared_ptr<const TreeNode> &curNode) {
	int val = 0;
	
	auto gstate = curNode->gamestate;
	auto oppId = gstate->getOpponentOf(pid); 

	if (gstate->isGameOver()) {
		auto winnerId = gstate->getWinnerId();
		if (winnerId == pid) {
			val +=  1000;
		}
		else if (winnerId == oppId) {
			val += -100;
		}
	}

	// Player Side(AI)
	auto pPrizePile = gstate->getPrizePile(pid);
	auto pActiveHolder = gstate->getBoard(pid)->getActiveHolder();
	// Opp Side(Player
	auto oPrizePile = gstate->getPrizePile(oppId);
	auto oActiveHolder = gstate->getBoard(oppId)->getActiveHolder();

	// Prize Card
	int d1 = (int)pPrizePile->getCurCardsInPile() - (int) oPrizePile->getCurCardsInPile();
	val += d1;
	if (d1 > 0) {
		CCLOG("Find gmae state > prize");
	}
	// Hp
	int d2 = ((int)pActiveHolder->getCurHp() - (int)oActiveHolder->getCurHp()) / 10;
	val += d2;
	int d3 = (int)pActiveHolder->getTotalEnergy() -(int)oActiveHolder->getTotalEnergy();

	if (d2 > 0) {
		CCLOG("Find gmae state > energy");
	}

	return val;
	// Energy

}