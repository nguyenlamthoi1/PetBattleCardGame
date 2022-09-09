#include "GameAction.h"

#include "../BattleMaster.h"
#include "../game_state/GameState.h"
#include "../game_state/Deck.h"
#include "../game_state/Hand.h"
#include "../game_state/Board.h"
#include "../game_state/DiscardPile.h"
#include "../game_state/PrizePile.h"

#include "../game_state/card/Card.h"

#include "../../BattleScene.h"
#include "../../scene_actions/BSAction.h"

#include "../player_actions/PlayerAction.h"

using namespace std;
USING_NS_CC;
using namespace BattleSceneNS;

NS_GAME_BEGIN

//---------------------//
//GameAction Base Class//
//---------------------//

std::shared_ptr<GameAction> GameAction::clone() const {
	return nullptr;
}

std::shared_ptr<BSAction> GameAction::getBSAction() const {
	return nullptr;
}


//--------------------------//
//WaitInputAction Base Class//
//--------------------------//

vector<shared_ptr<PlayerAction>> WaitInputAction::getPossibleMoves(GameState *gameState) const {return {};}

ActionError WaitInputAction::onReceiveInput(GameState *gameState, const std::shared_ptr<PlayerAction> &move) { 
	return ActionError::Failed;
}

//---------------------//
//FirstDrawAction Class//
//---------------------//

FirstDrawAction::FirstDrawAction(const PlayerIdType &id, unsigned int num) : pid(id), drawnNum(num) {}

FirstDrawAction::~FirstDrawAction() {};

void FirstDrawAction::executeOn(GameState *gstate) {
	state = State::Process;

	auto deck = gstate->getDeck(pid);

	cardIdsVec.clear();
	auto tempDrawnNum = drawnNum;
	if (drawnNum > 0) {
		vector<shared_ptr<Card>> cardObjVec;

		// Lay ra 1 la bai basic pet
		deck->popTopWithABasicPet(1, cardObjVec);
		tempDrawnNum -= cardObjVec.size();

		// Neu rut duoc 1 la bai basic pet
		if (!cardObjVec.empty()) {
			auto petCard = cardObjVec.back();
			cardObjVec.clear();

			// Rut them 6 la con lai
			deck->popTop(tempDrawnNum, cardObjVec);

			// Shuffle pet card vao 7 la nay
			auto insertIdx = cocos2d::RandomHelper::random_int(0, (int)cardObjVec.size());
			cardObjVec.insert(cardObjVec.cbegin() + insertIdx, petCard);

			// Bo 7 la vao hand
			auto hand = gstate->getHand(pid);
			hand->pushCards(cardObjVec);

			// Cap nhat result vector
			for (const auto& cardObj : cardObjVec) 
				cardIdsVec.emplace_back(cardObj->getId());
		}
		else {
			// Khong co bai basic pet thi cho ket thuc game luon
			gstate->pushAction(make_shared<GameOverAction>());
		}
	}

	state = State::Done;
}

std::shared_ptr<BattleSceneNS::BSAction> FirstDrawAction::getBSAction() const {
	return BattleSceneNS::SequenceAction::create({
		make_shared<BattleSceneNS::DrawCardAction>(pid, drawnNum, cardIdsVec),
		make_shared<BattleSceneNS::WaitAction>(1.0f)
		});
}

std::shared_ptr<GameAction> FirstDrawAction::clone() const {
	return  make_shared<FirstDrawAction>(pid, drawnNum);
}


//----------------//
//DrawAction Class//
//----------------//

DrawAction::DrawAction(DrawType drType, const PlayerIdType &id, unsigned int num, bool checkOver) : drawType(drType), pid(id), drawnNum(num), checkGameOverOnDrawn(checkOver) {
}

DrawAction::~DrawAction() {
};

void DrawAction::executeOn(GameState *gstate) {
	state = State::Process;

	auto deck = gstate->getDeck(pid);

	vector<shared_ptr<Card>> cardObjVec;

	if (drawType == DrawType::FromTop)
		deck->popTop(drawnNum, cardObjVec); // Rut tu tu Top Deck
	else if (drawType == DrawType::FromBottom)
		deck->popBottom(drawnNum, cardObjVec); // Rut tu Bottom Deck

	// Cap nhat Hand
	auto hand = gstate->getHand(pid);
	hand->pushCards(cardObjVec);

	// Cap nhat result vector
	for (const auto& cardObj : cardObjVec)
		cardIdsVec.emplace_back(cardObj->getId());

	// * Khi het bai dau moi turn phai check ket thuc game
	if (checkGameOverOnDrawn && cardObjVec.size() < drawnNum) {
		gstate->pushAction(make_shared<GameOverAction>(gstate->getOpponentOf(pid)));
	}

	state = State::Done;
}

std::shared_ptr<BattleSceneNS::BSAction> DrawAction::getBSAction() const {
	return BattleSceneNS::SequenceAction::create({
		make_shared<BattleSceneNS::DrawCardAction>(pid, drawnNum, cardIdsVec),
		make_shared<BattleSceneNS::WaitAction>(1.0f)
		});
}

std::shared_ptr<GameAction> DrawAction::clone() const {
	return  make_shared<DrawAction>(drawType, pid, drawnNum, checkGameOverOnDrawn);
}


//----------//
//SETUP FLOW//
//----------//

	//SetupActivePet Class//

SetupActivePet::SetupActivePet(const PlayerIdType &id, unsigned int idx) : pid(id), handIdx(idx) {}
SetupActivePet::~SetupActivePet(){}

void SetupActivePet::executeOn(GameState *gstate) {
	state = State::Process;

	auto hand = gstate->getHand(pid);
	auto board = gstate->getBoard(pid);
	
	suc = false;
	// Kiem tra handIdx co phai basic pet card
	auto petCard = dynamic_pointer_cast<PetCard>(hand->getCardAt(handIdx));
	suc = board->addBasicPetCardToActive(petCard);
	if (suc)
		hand->removeCard(handIdx);
	
	state = State::Done;
}

shared_ptr<BattleSceneNS::BSAction> SetupActivePet::getBSAction() const {
	if(suc)
		return make_shared<BattleSceneNS::DoSetupPetActive>(pid, handIdx); //TODO
}

shared_ptr<GameAction> SetupActivePet::clone() const {
	return make_shared<SetupActivePet>(pid, handIdx);
}

	//SetupBenchPet Class//

void SetupBenchPet::executeOn(GameState *gstate) {
	state = State::Process;

	auto hand = gstate->getHand(pid);
	auto board = gstate->getBoard(pid);

	suc = false;
	// Kiem tra handIdx co phai basic pet card
	auto petCard = dynamic_pointer_cast<PetCard>(hand->getCardAt(handIdx));
	suc = board->addBasicPetCardToBench(petCard);
	if (suc)
		hand->removeCard(handIdx);

	state = State::Done;
}

shared_ptr<BattleSceneNS::BSAction> SetupBenchPet::getBSAction() const {
	if (suc)
		return make_shared<BattleSceneNS::DoSetupPetBench>(pid, handIdx);
}

shared_ptr<GameAction> SetupBenchPet::clone() const {
	return make_shared<SetupBenchPet>(pid, handIdx);
}

	//StartSetupAction Class//

StartSetupAction::StartSetupAction() {}

StartSetupAction::~StartSetupAction() {}

void StartSetupAction::executeOn(GameState *gstate) {
	state = State::Process;
	gstate->startSetup();
	state = State::Done;
}

shared_ptr<BattleSceneNS::BSAction> StartSetupAction::getBSAction() const {
	return make_shared<BattleSceneNS::StartSetupAction>();
}

shared_ptr<GameAction> StartSetupAction::clone() const {
	return make_shared<StartSetupAction>();
}


	//StartSetupActive Class//

void StartSetupActivePet::executeOn(GameState *gstate) {
	state = State::Process;
	//state = State::Done;
}

shared_ptr<BattleSceneNS::BSAction> StartSetupActivePet::getBSAction() const {
	return BattleSceneNS::SequenceAction::create({
		make_shared<BattleSceneNS::WaitAction>(0.7f),
		make_shared<BattleSceneNS::StartSetupActive>(pid),
		});
}

shared_ptr<GameAction> StartSetupActivePet::clone() const {
	return make_shared<StartSetupAction>();
}

vector<shared_ptr<PlayerAction>> StartSetupActivePet::getPossibleMoves(GameState *gstate) const {
	vector<shared_ptr<PlayerAction>> ret;
	auto hand = gstate->getHand(pid);
	const auto &cards = hand->getAllCards();

	unsigned int handIdx = 0;
	for (const auto &card : cards) {
		auto petCard = dynamic_pointer_cast<PetCard>(card);
		auto data = petCard->getPetData();
		if (petCard && data->isBasicCard()) 
			ret.push_back(make_shared<PA_SetupActive>(pid, handIdx));
		++handIdx;
	}
	return ret;
}

ActionError StartSetupActivePet::onReceiveInput(GameState *gstate, const std::shared_ptr<PlayerAction> &move) {
	if (state != State::Process)
		return ActionError::Failed;

	auto setupMove = dynamic_pointer_cast<PA_SetupActive>(move);
	if (!setupMove || setupMove->pid != pid)
		return ActionError::Failed;
	
	auto handIdx = setupMove->handIdx;
	auto hand = gstate->getHand(pid);
	auto card = hand->getCardAt(handIdx);
	auto petCard = dynamic_pointer_cast<PetCard> (card);
	
	if (petCard && petCard->isBasicCard()) {
		state = State::Done;
		gstate->pushActionsAtFront({
			make_shared<SetupActivePet>(pid, handIdx),
			make_shared<StartSetupBenchPet>(pid)
			});
		return ActionError::Succeeded;
	}
	
	return ActionError::Failed;
}

/*StartSetupBenchPet Class*/

void StartSetupBenchPet::executeOn(GameState *gstate) {
	state = State::Process;
	//state = State::Done;
}

shared_ptr<BattleSceneNS::BSAction> StartSetupBenchPet::getBSAction() const {
	return BattleSceneNS::SequenceAction::create({
		make_shared<BattleSceneNS::WaitAction>(0.7f),
		make_shared<BattleSceneNS::StartSetupBench>(pid),
		});
}

shared_ptr<GameAction> StartSetupBenchPet::clone() const {
	return make_shared<StartSetupBenchPet>(pid);
}

vector<shared_ptr<PlayerAction>> StartSetupBenchPet::getPossibleMoves(GameState *gstate) const {
	vector<shared_ptr<PlayerAction>> ret;
	auto hand = gstate->getHand(pid);
	const auto &cards = hand->getAllCards();

	unsigned int handIdx = 0;
	for (const auto &card : cards) {
		auto petCard = dynamic_pointer_cast<PetCard>(card);
		auto data = petCard->getPetData();
		if (petCard && data->isBasicCard())
			ret.push_back(make_shared<PA_SetupBench>(pid, handIdx));
		++handIdx;
	}
	return ret;
}

ActionError StartSetupBenchPet::onReceiveInput(GameState *gstate, const std::shared_ptr<PlayerAction> &move) {
	if (state != State::Process)
		return ActionError::Failed;

	// Xu ly Player Action thiet lap Bench Pet
	if (move->getType() == PlayerAction::Type::SetupBenchPet) {
		auto setupMove = dynamic_pointer_cast<PA_SetupBench>(move);
		if (!setupMove || setupMove->pid != pid)
			return ActionError::Failed;

		auto handIdx = setupMove->handIdx;
		auto hand = gstate->getHand(pid);
		auto card = hand->getCardAt(handIdx);
		auto petCard = dynamic_pointer_cast<PetCard> (card);

		if (petCard && petCard->isBasicCard()) {
			state = State::Done;
			gstate->pushActionsAtFront({ 
				make_shared<SetupBenchPet>(pid, handIdx),
				make_shared<StartSetupBenchPet>(pid),
				});
			
			return ActionError::Succeeded;
		}
		return ActionError::Failed;
	}
	else if (move->getType() == PlayerAction::Type::EndTurn) {
		auto setupMove = dynamic_pointer_cast<PA_EndTurn>(move);
		if (!setupMove || setupMove->pid != pid)
			return ActionError::Failed;

		auto otherId = gstate->getOpponentOf(pid);
		auto board = gstate->getBoard(otherId);
		if (!board->hasActivePet()){ // Doi thu chua co Active Pet
			state = State::Done;
			gstate->pushActionsAtFront({ make_shared<StartSetupActivePet>(otherId) });
			return ActionError::Succeeded;
		}
		else {
			state = State::Done;
			gstate->pushActionsAtFront({ make_shared<EndSetup>()});
			return ActionError::Succeeded;
		}

	}

	return ActionError::Failed;
}
	//EndSetup Class//
void EndSetup::executeOn(GameState *gstate) {
	if (state != State::Wait)
		return;
	state = State::Process;
	gstate->pushActionsAtFront({make_shared<FlipCoinGetFirst>(),});
	state = State::Done;
}

shared_ptr<BattleSceneNS::BSAction> EndSetup::getBSAction() const {
	return nullptr;
}

shared_ptr<GameAction> EndSetup::clone() const {
	return make_shared<EndSetup>();
}

//---///
//---///
//---///

/*
	FlipCoinGetFirst
*/

void FlipCoinGetFirst::executeOn(GameState *gstate) {
	if (state != State::Wait)
		return;
	state = State::Process;
	firstIdx = cocos2d::RandomHelper::random_int(0, (int)gstate->getPlayerCount());
	state = State::Done;
}

shared_ptr<BattleSceneNS::BSAction> FlipCoinGetFirst::getBSAction() const {
	return nullptr;
}

shared_ptr<GameAction> FlipCoinGetFirst::clone() const {
	return make_shared<FlipCoinGetFirst>();
}

/*
	GameOverAction Class
*/

GameOverAction::GameOverAction(const PlayerIdType &wid) : winnerId(wid) {}

GameOverAction::~GameOverAction() {}

void GameOverAction::executeOn(GameState *gstate) {
	state = State::Process;

	gstate->setGameOver(winnerId);

	state = State::Done;
}

bool GameOverAction::isWinnerFound() const {
	return !winnerId.empty();
}

bool GameOverAction::isDrawnGame() const {
	return winnerId.empty();
}

shared_ptr<GameAction> GameOverAction::clone() const {
	return make_shared<GameOverAction>(winnerId);
}

shared_ptr<BattleSceneNS::BSAction> GameOverAction::getBSAction() const {
	return make_shared<BattleSceneNS::GameOverAction>(winnerId);
}

NS_GAME_END