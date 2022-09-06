#pragma once

#include "../GameDefine.h"

#include <vector>
#include <list>
#include <unordered_map>
#include <memory>

NS_GAME_BEGIN

class Deck;
class Hand;
class Board;
class Player;
class GameAction;

class GameState {
public:
	friend class BattleMaster;

	GameState();
	GameState(const PlayerIdType &p1, const PlayerIdType &p2);
	virtual ~GameState();

public:
	bool isGameOver() const;
	PlayerIdType getWinnerId() const;
	PlayerIdType getOpponentOf(const PlayerIdType &pid) const;
	bool empty() const;

	std::shared_ptr<Deck> getDeck(const PlayerIdType &pid) const;
	std::shared_ptr<Hand> getHand(const PlayerIdType &pid) const;
	std::shared_ptr<Board> getBoard(const PlayerIdType &pid) const;
	std::shared_ptr<Player> getPlayer(const PlayerIdType &pid) const;

	std::shared_ptr<GameState> clone() const; // * Clone Game State moi, doc lap voi State duoc clone
	void startGame();
	void clearState();
	void setGameOver(const PlayerIdType & winnerId);
	bool initStateWith2Players(const PlayerIdType &p1, const PlayerIdType &p2);

	void pushAction(const std::shared_ptr<GameAction> &action);
	void pushActions(std::initializer_list<std::shared_ptr<GameAction>> actions);

protected:
	bool initDeck(const PlayerIdType &id);
	bool initHand(const PlayerIdType &id);
	bool initBoard(const PlayerIdType &id);
	bool initPlayer(const PlayerIdType &id);

protected:
	std::vector<PlayerIdType> pids; // vector gom cac player id
	std::unordered_map<PlayerIdType, std::shared_ptr<Deck>> decks;
	std::unordered_map<PlayerIdType, std::shared_ptr<Hand>> hands;
	std::unordered_map<PlayerIdType, std::shared_ptr<Board>> boards;
	std::unordered_map<PlayerIdType, std::shared_ptr<Player>> players;

	std::list<std::shared_ptr<GameAction>> actionQueue;

	PlayerIdType winnerId;
	bool gameOver = false;
public:
	/// Nhung action la thay doi game state ///
	ActionError onPlayerDrawCard(const PlayerIdType &pid, unsigned int num); // AI Game State phai hien thuc cai nay

};

NS_GAME_END