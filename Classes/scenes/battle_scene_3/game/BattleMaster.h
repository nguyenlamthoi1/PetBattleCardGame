#pragma once

#include "cocos2d.h"
#include "GameDefine.h"
#include <vector>
#include <string>

class PlayerData;

NS_GAME_BEGIN

class GameAction;
class PlayerAction;
class GameState;

/*
	- Class luu tru game state: deck, hand, board,...
	- Dieu khien qua trinh choi
	- Singleton
	* Luu y khong ke thua class nay
*/
class BattleMaster final {
public:
	using BMPtr = std::shared_ptr<BattleMaster>;
	static BMPtr get();

	BattleMaster();
	virtual ~BattleMaster();

	/*
	- Khoi tao tran dau giua 2 nguoi choi
	* P1, P2 khong phan biet thu tu
	*/
	bool initGame(const PlayerIdType &p1, const PlayerIdType &p2);
	/*
	- Sau khi init, startGame duoc call se bat dau game
	*/
	bool startGame();
	void endGame();

	/// Getters, Setters, Checkers
	std::shared_ptr<GameState> getGameState() const { return gstate; }
	
private:
	static BMPtr instance; // Singleton

	std::unordered_map<PlayerIdType, std::shared_ptr<PlayerData>> playerData; // * Thong tin data cua moi nguoi choi, bao gom: deck, card back, loai coin duoc dung, avatar,..

	unsigned int curTurn = 0;
	PlayerIdType curTurnId = PlayerIdInvalid;
	std::vector<PlayerIdType> pids;
	std::shared_ptr<GameState> gstate;

	struct PlayerGameInfo; // Thong tin can thiet cua moi nguoi choi trong qua trinh choi

	// Utilize functions
	void clearGame(); // * Xoa data Game truoc do

	std::shared_ptr<GameAction> getCurrentAction() const;

	static const std::string GAME_LOOP_SCHEDULER;
	/*
	- Turn based game loop
	- Thuc thi GameAction hien tai cua GameState, sau khi thuc thi xong se thuc thi GameAction tiep theo trong Queue
	*/
	void gameLoop(float t);
	
	/// Nhung Action do nguoi choi thuc hien trong qua trinh choi ///

public:
	void onPlayerDoEndGame();
	void onPlayerDoRestartGame();
	ActionError onPlayerChooseAction(const std::shared_ptr<PlayerAction> &action);
};

struct BattleMaster::PlayerGameInfo {
	bool setupDone = false;
};

NS_GAME_END



