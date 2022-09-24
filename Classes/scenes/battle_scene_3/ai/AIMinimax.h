#pragma once

#include "AIPlayer.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>
#include<memory>

class GameTree;

class AIMinimax : public AIPlayer{
public:
	static const std::string AI_SCHEDULER;

	AIMinimax();
	virtual ~AIMinimax();

	virtual void startThinking(BattleSceneNS::BattleScene *btlScn, const std::shared_ptr<MGame::GameState> &gamestate) override;
	virtual void onDoneThinking();
protected:
	enum class State {
		NONE = -1,
		UPDATE_TREE,
		TRAVERSE_TREE,
		DONE
	};
	State curState = State::NONE;
	std::shared_ptr<GameTree> tree;
	unsigned int LimitMaxCount = 1; // Tree tao den duoc node tang Max thu 2 thi dung
	void thinkLoop(float dt);
	float maxThinkTime = 0.0f; // Thoi gian danh cho AI suy nghi
	float thinkTime = 0.0f; // Thoi gian tieu ton de AI suy nghi
	
	unsigned int numFloorToTraversed = 0; // So tang duyet cay o moi frame
	unsigned int numFloorInc = 0; // So lan duyet cay thang them o moi frame
	
	float timeUsedForPrevTraverse = 0.0f;

	int curBestVal = 0;
};