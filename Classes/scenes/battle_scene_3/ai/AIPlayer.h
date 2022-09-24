#pragma once

#include "cocos2d.h"

#include <vector>
#include <unordered_map>
#include <initializer_list>
#include <functional>
#include <utility>
#include <memory>

namespace MGame {
	class PlayerAction;
	class GameState;
}

namespace BattleSceneNS {
	class BattleScene;
}

/*
	Abstract Class cho nhung nguoi choi AI
	Moi Subclass cua Class nay dai dien cho  viec su dung mot chien thuat AI nhat dinh
*/
class AIPlayer {
public:
	AIPlayer() = default;
	virtual ~AIPlayer() = default;

	// Ham nay se duoc goi boi BattleScene, de co the chuyen nhung decision thuc hien boi AI cho BattleMaster
	virtual void startThinking(BattleSceneNS::BattleScene *btlScn, const std::shared_ptr<MGame::GameState> &gamestate) = 0;

	BattleSceneNS::BattleScene *btlScn = nullptr;
	std::shared_ptr<MGame::PlayerAction> chosenMove; // Nuoc di ma AI chon
	std::shared_ptr<const MGame::GameState> orgGamestate; // Luu giu gamestate ban dau
};
