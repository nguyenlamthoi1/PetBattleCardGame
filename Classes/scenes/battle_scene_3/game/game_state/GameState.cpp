#include "GameState.h"
#include "Deck.h"
#include "Hand.h"
#include "Board.h"
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

	return true;
}

bool GameState::initHand(const PlayerIdType &id) {
	hands.insert({ id, Hand::createShPtr(id) });
	return true;
}

bool GameState::initBoard(const PlayerIdType &id) {
	boards.insert({ id, Board::createShPtr(id) });
	return true;
}

void GameState::startGame() {
	actionQueue.insert(actionQueue.cend(), {
		make_shared<DrawAction>(DrawAction::DrawType::FromTop, pids[0], GConfig::DRAW_NUM_ON_START),
		make_shared<DrawAction>(DrawAction::DrawType::FromTop,  pids[1], GConfig::DRAW_NUM_ON_START),
		});
}

void GameState::clearState() {
	pids.clear();
	decks.clear();
	hands.clear();
	boards.clear();
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

bool GameState::empty() const {
	return pids.empty();
}


/// Actions to update Game State

ActionError GameState::onPlayerDrawCard(const PlayerIdType &pid, unsigned int num) {
	//TODO
	return ActionError::Succeeded;
}


NS_GAME_END