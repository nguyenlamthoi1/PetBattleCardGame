#include "GameState.h"
#include "Deck.h"
#include "Hand.h"
#include "Board.h"
#include "Player.h"
#include "DiscardPile.h"
#include "PrizePile.h"

#include "card/Card.h"
#include "../game_actions/GameAction.h"

#include "GameManager.h"
#include "data/PlayerData.h"

using namespace std;

NS_GAME_BEGIN

GameState::GameState(){}

GameState::GameState(const PlayerIdType &p1, const PlayerIdType &p2) {
	CCASSERT(initStateWith2Players(p1, p2), "GameState::init state failed!");
}

GameState::~GameState() {

}

bool GameState::initStateWith2Players(const PlayerIdType &p1, const PlayerIdType &p2) {
	clearState();

	pids.push_back(p1);
	pids.push_back(p2);

	for (const auto &pid : pids) {
		bool initSuc = initDeck(pid);
		if (!initSuc)
			return false;

		initSuc = initHand(pid);
		if (!initSuc)
			return false;

		initSuc = initBoard(pid);
		if (!initSuc)
			return false;
	}

	return true;
}

bool GameState::initDeck(const PlayerIdType &id) {
	auto dtaMgr = GM_DATA_MGR;
	auto playerData = dtaMgr->getPlayerData(id);
	if (!playerData)
		return false;

	decks.insert({id, Deck::createShPtr(id)});

	auto cardList = playerData->getCurDeck();
	for (const auto &data : cardList) {
		auto card = Card::createShPtr(data->id);
		if (card)
			decks.at(id)->pushCard(card); // Them card vao deck
	}

	decks.at(id)->shuffleDeck();

	return true;
}

bool GameState::initHand(const PlayerIdType &id) {
	hands.insert({ id, Hand::createShPtr(id) });
	return true;
}

bool GameState::initBoard(const PlayerIdType &id) {
	boards.insert({ id, Board::createShPtr(this, id) });
	return true;
}

bool GameState::initPlayer(const PlayerIdType &id) {
	players.insert({ id, Player::createShPtr(id) });
	return true;
}


void GameState::startGame() {
	turnCount = 0;
	winnerId = "";
	gameOver = false;
	phase = Phase::Start;

	actionQueue.insert(actionQueue.cend(), {
		make_shared<FirstDrawAction>(pids[0], GConfig::DRAW_NUM_ON_START),
		make_shared<FirstDrawAction>(pids[1], GConfig::DRAW_NUM_ON_START),
		make_shared<StartSetupAction>(),
		make_shared<StartSetupActivePet>(pids[0]),
		});
}

void GameState::startSetup() {

}

void GameState::clearState() {
	pids.clear();
	decks.clear();
	hands.clear();
	boards.clear();
}

void GameState::setGameOver(const PlayerIdType & wid) {
	winnerId = wid;
	gameOver = false;
}

/// Getter, Setter & Checkers
std::shared_ptr<Deck> GameState::getDeck(const PlayerIdType &pid) const {
	auto itr = decks.find(pid);
	if (itr != decks.end())
		return itr->second;
	return nullptr;
}

std::shared_ptr<Hand> GameState::getHand(const PlayerIdType &pid) const {
	auto itr = hands.find(pid);
	if (itr != hands.end())
		return itr->second;
	return nullptr;
}

std::shared_ptr<Board> GameState::getBoard(const PlayerIdType &pid) const {
	auto itr = boards.find(pid);
	if (itr != boards.end())
		return itr->second;
	return nullptr;
}

std::shared_ptr<Player> GameState::getPlayer(const PlayerIdType &pid) const {
	auto itr = players.find(pid);
	if (itr != players.end())
		return itr->second;
	return nullptr;
}

std::shared_ptr<DiscardPile> GameState::getDiscardPile(const PlayerIdType &pid) const {
	auto itr = discardPiles.find(pid);
	if (itr != discardPiles.end())
		return itr->second;
	return nullptr;
}

std::shared_ptr<PrizePile> GameState::getPrizePile(const PlayerIdType &pid) const {
	auto itr = prizePiles.find(pid);
	if (itr != prizePiles.end())
		return itr->second;
	return nullptr;
}

bool GameState::empty() const {
	return pids.empty();
}

bool GameState::isGameOver() const {
	return gameOver;
}

PlayerIdType GameState::getWinnerId() const {
	return winnerId;
}

PlayerIdType GameState::getOpponentOf(const PlayerIdType &pid) const {
	return pid == pids[0] ? pids[1] : pids[0];
}


/// Actions to update Game State

void GameState::pushAction(const std::shared_ptr<GameAction> &action) {
	actionQueue.push_back(action);
}

void GameState::pushActions(std::initializer_list<std::shared_ptr<GameAction>> actions) {
	for (const auto &action : actions) 
		actionQueue.push_back(action);
}

ActionError GameState::onPlayerDrawCard(const PlayerIdType &pid, unsigned int num) {
	//TODO
	return ActionError::Succeeded;
}

std::shared_ptr<GameState> GameState::clone() const {
	shared_ptr<GameState> cloneState = make_shared<GameState>();
	
	cloneState->curPlayer = curPlayer;
	cloneState->pids = pids;

	// Clone ActionQueue
	auto &clActionQueue = cloneState->actionQueue;
	for (const auto &action : actionQueue) 
		clActionQueue.emplace_back(action->clone());

	for (const auto &id : pids) {
		// Clone Hand
		auto handItr = hands.find(id);
		if (handItr != hands.cend())
			cloneState->hands.insert({ id, handItr->second->clone() });
		// Clone Deck
		auto deckItr = decks.find(id);
		if (deckItr != decks.cend())
			cloneState->decks.insert({ id, deckItr->second->clone() });
		// Clone Board
		auto boardItr = boards.find(id);
		if (boardItr != boards.cend())
			cloneState->boards.insert({ id, boardItr->second->clone() });
		// Clone Player
		auto playerItr = players.find(id);
		if (playerItr != players.cend())
			cloneState->players.insert({ id, playerItr->second->clone() });
		// Clone Discard Pile
		auto discardItr = discardPiles.find(id);
		if (discardItr != discardPiles.cend())
			cloneState->discardPiles.insert({ id, discardItr->second->clone() });
		// Clone Prize Pile
		auto prizeItr = prizePiles.find(id);
		if (prizeItr != prizePiles.cend())
			cloneState->prizePiles.insert({ id, prizeItr->second->clone() });
	}

	cloneState->phase = phase;
	cloneState->turnCount = turnCount;
	cloneState->winnerId = winnerId;
	cloneState->gameOver = gameOver;

	return nullptr;
}

NS_GAME_END