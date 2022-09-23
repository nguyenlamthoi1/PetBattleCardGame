#pragma once


#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>
#include <memory>

namespace MGame {
	class PlayerAction;
	class GameState;
}

namespace BattleSceneNS {
	class BattleScene;
}

class TreeNode;

class GameTree {
public:
	void gen();
	
	std::shared_ptr<const TreeNode> getBestNextNode(unsigned int maxFloor); // Duyet cay 3 tang
	void setMaxFloorToGen(unsigned int n);
	void setMaxTurnCount(unsigned int n);
	bool isReachMaxDepth() const;
	void clear();
	void initWithRoot(const std::shared_ptr<const MGame::GameState> &gstate);
	
	std::shared_ptr<TreeNode> rootNode;
	unsigned int turnCount = 0;
	unsigned int maxTurnCount = 0;
	unsigned int maxFloorToGenEachFrame = 0;
	unsigned int maxFloorGened = 0;
	unsigned int maxTraverseFloor = 0;
	struct Result {
		std::shared_ptr<const TreeNode> node;
		int val = 0;
	};
	Result minimax(const std::shared_ptr<const TreeNode> &curNode, bool isMax, unsigned int floor);

	void genNextNodes(const std::shared_ptr<TreeNode> &curNode);

};
