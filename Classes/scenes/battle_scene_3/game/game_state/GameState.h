#pragma once

#include "../GameDefine.h"

#include <vector>
#include <list>
#include <unordered_map>
#include <memory>

namespace BattleSceneNS{
	class BSAction;
}
#define GameState_TAG_CLONE //

NS_GAME_BEGIN

class Deck;
class Hand;
class Board;
class Player;
class DiscardPile;
class PrizePile;
class GameAction;
class PlayerAction;
class CoinFlipper;

class GameState {
public:
	friend class BattleMaster;

	enum class Phase {
		None, // Game chua bat dau
		Start, // Game bat dau
		Setup, // 2 nguoi choi thiet lap board
		PlayerTurn, // nguoi choi thuc thi luot cua minh
		BetweenTurn, // giua moi lan thay doi luot nguoi choi
	};

	GameState();
	GameState(const PlayerIdType &p1, const PlayerIdType &p2);
	virtual ~GameState();

public:
	bool isGameOver() const;
	PlayerIdType getWinnerId() const;
	PlayerIdType getOpponentOf(const PlayerIdType &pid) const;
	bool empty() const;
	
	void setFirstIdx(unsigned int idx) { firstPlayerIdx = idx; }
	unsigned int getFirstIdx() const { return firstPlayerIdx; }
	unsigned int getNextTurnIdx() const { return (curPlayer + 1) % pids.size(); }
	void setTurnCount(unsigned int tcount) { turnCount = tcount; }
	unsigned int getTurnCount() const { return turnCount; }
	PlayerIdType getCurrentId() const { return pids[curPlayer]; }
	void setCurrentPlayerToNext() { curPlayer = (curPlayer + 1) % pids.size(); }
	unsigned int getPlayerCount() const { return pids.size(); }
	const std::vector<PlayerIdType>& getPids() const { return pids; }
	PlayerIdType getPlayerIdAt(unsigned int idx) { return pids[idx]; }
	int getIdxOfPlayer(const PlayerIdType &id);
	std::shared_ptr<Deck> getDeck(const PlayerIdType &pid) const;
	std::shared_ptr<Hand> getHand(const PlayerIdType &pid) const;
	std::shared_ptr<Board> getBoard(const PlayerIdType &pid) const;
	std::shared_ptr<Player> getPlayer(const PlayerIdType &pid) const;
	std::shared_ptr<DiscardPile> getDiscardPile(const PlayerIdType &pid) const;
	std::shared_ptr<PrizePile> getPrizePile(const PlayerIdType &pid) const;
	std::shared_ptr<CoinFlipper> getFlipper(const PlayerIdType &pid) const;
	std::list<std::shared_ptr<GameAction>>& getActionQueue() { return actionQueue; }

	std::shared_ptr<GameState> clone() const; // * Clone Game State moi, doc lap voi State duoc clone
	
	bool isWaitingInput();

	std::shared_ptr<BattleSceneNS::BSAction> progressGame();
	void progressGameNoAnimation(); // AI se goi ham nay de thay doi game state

	void onTurnStart(unsigned int playerIdx);
	void startGame();
	void startSetup();
	void endSetup();
	void clearState();
	void setGameOver(const PlayerIdType & winnerId);
	bool initStateWith2Players(const PlayerIdType &p1, const PlayerIdType &p2);

	void pushAction(const std::shared_ptr<GameAction> &action);
	void pushActions(std::initializer_list<std::shared_ptr<GameAction>> actions);
	void pushActionsAtFront(std::initializer_list<std::shared_ptr<GameAction>> actions);

protected:
	bool initDeck(const PlayerIdType &id);
	bool initHand(const PlayerIdType &id);
	bool initBoard(const PlayerIdType &id);
	bool initPlayer(const PlayerIdType &id);
	bool initDiscard(const PlayerIdType &id);
	bool initPrize(const PlayerIdType &id);
	bool initFlipper(const PlayerIdType &id);
protected:
	std::unordered_map<PlayerIdType, std::shared_ptr<Deck>> decks; //GameState_TAG_CLONE
	std::unordered_map<PlayerIdType, std::shared_ptr<Hand>> hands; //GameState_TAG_CLONE
	std::unordered_map<PlayerIdType, std::shared_ptr<Board>> boards; //GameState_TAG_CLONE
	std::unordered_map<PlayerIdType, std::shared_ptr<DiscardPile>> discardPiles; //GameState_TAG_CLONE
	std::unordered_map<PlayerIdType, std::shared_ptr<PrizePile>> prizePiles; //GameState_TAG_CLONE
	std::unordered_map<PlayerIdType, std::shared_ptr<Player>> players; //GameState_TAG_CLONE
	std::unordered_map<PlayerIdType, std::shared_ptr<CoinFlipper>> flippers; //GameState_TAG_CLONE

	std::list<std::shared_ptr<GameAction>> actionQueue; //GameState_TAG_CLONE

	std::vector<PlayerIdType> pids; //GameState_TAG_CLONE // vector gom cac player id
	unsigned int curPlayer; //GameState_TAG_CLONE // Actor hien tai dang thuc hien hanh dong 
	unsigned int firstPlayerIdx = 0;
	Phase phase = Phase::None; //GameState_TAG_CLONE
	unsigned int turnCount = 0; //GameState_TAG_CLONE // Phan biet giua cac luot tang truong: luot 1(P1), luot 2(P2), luot 3(P1) 
	PlayerIdType winnerId; //GameState_TAG_CLONE
	bool gameOver = false; //GameState_TAG_CLONE

	bool paused = false; // Gameloop dang doi (input) hay 1 cai gi do
public:
	/// Nhung action la thay doi game state ///
	ActionError onPlayerTakeAction(const std::shared_ptr<PlayerAction> &pa);
	ActionError onPlayerDrawCard(const PlayerIdType &pid, unsigned int num); // AI Game State phai hien thuc cai nay
};

NS_GAME_END