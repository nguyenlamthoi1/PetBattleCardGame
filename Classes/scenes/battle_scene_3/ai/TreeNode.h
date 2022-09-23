#pragma once

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>
#include <memory>



namespace MGame {
	class GameState;
	class PlayerAction;
}

class GameTree;

class TreeNode {
public:
	friend class GameTree;
	TreeNode(const std::shared_ptr<const MGame::GameState> & gamestate);
	virtual ~TreeNode();

	friend class GameTree;
	int calValue() const; // * Ham heuristic
	bool isLeaf() const;
	std::string getCurPid() const; // Lay thong tin nguoi choi dang thuc thi luot
	std::shared_ptr<MGame::PlayerAction> getPreMove() const {
		return prevMove;
	}
	
protected:
	std::shared_ptr<const MGame::GameState> gamestate;
	std::shared_ptr<MGame::PlayerAction> prevMove;
	std::vector<std::shared_ptr<TreeNode>> nexts;

	int curVal = 0;

};