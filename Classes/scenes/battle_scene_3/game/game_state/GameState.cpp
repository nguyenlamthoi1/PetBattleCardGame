#include "GameState.h"
#include "Deck.h"
#include "Hand.h"
#include "Board.h"
#include "Player.h"
#include "DiscardPile.h"
#include "PrizePile.h"
#include "CoinFlipper.h"

#include "card/Card.h"
#include "../game_actions/GameAction.h"

#include "GameManager.h"
#include "data/PlayerData.h"

#include "data/MoveData.h"

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

		initSuc = initPlayer(pid);
		if (!initSuc)
			return false;

		initSuc = initPrize(pid);
		if (!initSuc)
			return false;

		initSuc = initDiscard(pid);
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

bool  GameState::initDiscard(const PlayerIdType &id) {
	discardPiles.insert({ id, DiscardPile::createShPtr(id) });
	return true;
}

bool  GameState::initPrize(const PlayerIdType &id) {
	prizePiles.insert({ id, PrizePile::createShPtr(id) });
	return true;
}

bool  GameState::initFlipper(const PlayerIdType &id) {
	flippers.insert({ id, CoinFlipper::createShPtr(id) });
	return true;
}

void GameState::startGame() {
	turnCount = 0;
	winnerId = "";
	gameOver = false;
	phase = Phase::Start;

	actionQueue.insert(actionQueue.cend(), {
		make_shared<FirstDrawAction>(pids[0], GConfig::DRAW_NUM_ON_START),
		make_shared<DrawFirstPrizeCards>(pids[0], GConfig::PRIZE_CARDS_NUM_PER_PLAYER),
		make_shared<FirstDrawAction>(pids[1], GConfig::DRAW_NUM_ON_START),
		make_shared<DrawFirstPrizeCards>(pids[1], GConfig::PRIZE_CARDS_NUM_PER_PLAYER),
		make_shared<StartSetupAction>(),
		make_shared<StartSetupActivePet>(pids[0]),
		});
}

void GameState::onTurnStart(unsigned int playerIdx) {
	phase = Phase::PlayerTurn;
	turnCount += 1;

	if (updateTurnCountAfterSetup) {
		turnCount = 0;
		updateTurnCountAfterSetup = false;
	}
	else
		turnCount += 1;

	curPlayer = playerIdx;
	auto curPlayerId = pids[curPlayer];
	auto player = players.at(curPlayerId);
	player->resetDoneCount(); // Reset lai so lan thuc hien cua cac player
}


void GameState::progressGameNoAnimation() {
	// Lay ra 1 action de thuc thi
	if (isGameOver())
		return;

	if (!actionQueue.empty()) {
		auto curAction = actionQueue.front();
		if (curAction->state == GameAction::State::Wait) {
			curAction->executeOn(this); // Thuc thi tung action

		}
		else if (curAction->state == GameAction::State::Done)
			actionQueue.pop_front();
	}
}

shared_ptr<BattleSceneNS::BSAction> GameState::progressGame() {
	// Lay ra 1 action de thuc thi
	//if (isGameOver())
	//	return nullptr;

	shared_ptr<BattleSceneNS::BSAction> bsAction;

	//removedQueue.clear();
	if (!actionQueue.empty()) {
		auto curAction = actionQueue.front();
		if (curAction->state == GameAction::State::Wait) {
			curAction->executeOn(this); // Thuc thi tung action

			bsAction = curAction->getBSAction(); // Them animation
		}
		else if (curAction->state == GameAction::State::Done)
			actionQueue.pop_front();
	}

	return bsAction;
}

void GameState::startSetup() {
	phase = Phase::Setup;
	curPlayer = 0;
	turnCount = 0;
}

void GameState::endSetup() {
	turnCount = 0;
	updateTurnCountAfterSetup = true;
}


void GameState::clearState() {
	curPlayer = 0;
	pids.clear();
	actionQueue.clear();

	decks.clear();
	hands.clear();
	boards.clear();
	discardPiles.clear();
	players.clear();
	prizePiles.clear();
	flippers.clear();

	firstPlayerIdx = 0;
	phase = Phase::None;
	turnCount = 0;
	winnerId = "";
	updateTurnCountAfterSetup = false;
	gameOver = false;
}

void GameState::setGameOver(const PlayerIdType & wid) {
	winnerId = wid;
	gameOver = true;
	//Might delete
	actionQueue.clear();
}

/// Getter, Setter & Checkers
shared_ptr<Deck> GameState::getDeck(const PlayerIdType &pid) const {
	auto itr = decks.find(pid);
	if (itr != decks.end())
		return itr->second;
	return nullptr;
}

shared_ptr<Hand> GameState::getHand(const PlayerIdType &pid) const {
	auto itr = hands.find(pid);
	if (itr != hands.end())
		return itr->second;
	return nullptr;
}

shared_ptr<Board> GameState::getBoard(const PlayerIdType &pid) const {
	auto itr = boards.find(pid);
	if (itr != boards.end())
		return itr->second;
	return nullptr;
}

shared_ptr<Player> GameState::getPlayer(const PlayerIdType &pid) const {
	auto itr = players.find(pid);
	if (itr != players.end())
		return itr->second;
	return nullptr;
}

shared_ptr<DiscardPile> GameState::getDiscardPile(const PlayerIdType &pid) const {
	auto itr = discardPiles.find(pid);
	if (itr != discardPiles.end())
		return itr->second;
	return nullptr;
}

shared_ptr<PrizePile> GameState::getPrizePile(const PlayerIdType &pid) const {
	auto itr = prizePiles.find(pid);
	if (itr != prizePiles.end())
		return itr->second;
	return nullptr;
}

shared_ptr<CoinFlipper> GameState::getFlipper(const PlayerIdType &pid) const {
	auto itr = flippers.find(pid);
	if (itr != flippers.end())
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

// Player Actions

bool GameState::isWaitingInput() {
	if (actionQueue.empty())
		return false;
	auto waitInputAction = dynamic_pointer_cast<WaitInputAction>(actionQueue.front());
	//return waitInputAction != nullptr && waitInputAction->state != GameAction::State::Done;
	return waitInputAction != nullptr && waitInputAction->state == GameAction::State::Process;
}

ActionError GameState::onPlayerTakeAction(const shared_ptr<PlayerAction> &pa) {
	if (actionQueue.empty())
		return ActionError::Failed;

	auto waitInputAction = dynamic_pointer_cast<WaitInputAction>(actionQueue.front());
	if (waitInputAction && waitInputAction->state != GameAction::State::Done)
		return waitInputAction->onReceiveInput(this, pa);

	return ActionError::Failed;
}


// Actions to update Game State

//shared_ptr<GameAction> GameState::createAction(const std::shared_ptr<const ActionData> &adata) {
//		auto aid = adata->id;
//	if (aid == "Attack_Default") {
//		auto baseDmg = adata->ovrValues.at(to_string(0));
//		return make_shared<MGame::DefaultAttack>();
//	}
//	else if(false) {
//		return nullptr;
//	}
//	return nullptr;
//}

void GameState::pushAction(const std::shared_ptr<GameAction> &action) {
	actionQueue.push_back(action);
}

void GameState::pushActions(std::initializer_list<shared_ptr<GameAction>> actions) {
	for (const auto &action : actions) 
		actionQueue.push_back(action);
}

void GameState::pushActionsAtFront(initializer_list<shared_ptr<GameAction>> actions) {
	actionQueue.insert(actionQueue.begin(), actions.begin(), actions.end());
}

void GameState::replaceCurActionWith(initializer_list<shared_ptr<GameAction>> actions) {
	actionQueue.pop_front();
	actionQueue.insert(actionQueue.begin(), actions.begin(), actions.end());
}

void GameState::replaceCurActionWithVec(const vector<shared_ptr<GameAction>> &actions) {
	actionQueue.pop_front();
	actionQueue.insert(actionQueue.begin(), actions.begin(), actions.end());
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
		// Clone Deck
		auto deckItr = decks.find(id);
		if (deckItr != decks.cend())
			cloneState->decks.insert({ id, deckItr->second->clone() });

		// Clone Hand
		auto handItr = hands.find(id);
		if (handItr != hands.cend())
			cloneState->hands.insert({ id, handItr->second->clone() });

		// Clone Board
		auto boardItr = boards.find(id);
		if (boardItr != boards.cend())
			cloneState->boards.insert({ id, boardItr->second->clone() });

		// Clone Discard Pile
		auto discardItr = discardPiles.find(id);
		if (discardItr != discardPiles.cend())
			cloneState->discardPiles.insert({ id, discardItr->second->clone() });

		// Clone Player
		auto playerItr = players.find(id);
		if (playerItr != players.cend())
			cloneState->players.insert({ id, playerItr->second->clone() });

		// Clone Prize Pile
		auto prizeItr = prizePiles.find(id);
		if (prizeItr != prizePiles.cend())
			cloneState->prizePiles.insert({ id, prizeItr->second->clone() });

		// Clone Flipper
		auto flipperItr = flippers.find(id);
		if (flipperItr != flippers.cend())
			cloneState->flippers.insert({ id, flipperItr->second->clone() });
	}

	cloneState->firstPlayerIdx = 0;
	cloneState->phase = phase;
	cloneState->turnCount = turnCount;
	cloneState->winnerId = winnerId;
	cloneState->updateTurnCountAfterSetup = updateTurnCountAfterSetup;
	cloneState->gameOver = gameOver;

	return cloneState;
}

int GameState::getIdxOfPlayer(const PlayerIdType &id) {
	for (unsigned int i = 0; i < pids.size(); ++i) {
		if (pids[i] == id) {
			return i;
		}
	}
	return -1;
}


NS_GAME_END