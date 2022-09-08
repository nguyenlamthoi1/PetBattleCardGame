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

using namespace std;
USING_NS_CC;
using namespace BattleSceneNS;

NS_GAME_BEGIN

/*
	GameAction Class
*/

std::shared_ptr<GameAction> GameAction::clone() const {
	return nullptr;
}

std::shared_ptr<BSAction> GameAction::getBSAction() const {
	return nullptr;
}



/*
	CustomAction Class
*/

//CustomAction::CustomAction(const CustomFunction &f){}
//
//CustomAction::~CustomAction(){}
//
//void CustomAction::start() {
//	state = State::Process;
//	doFunc();
//	state = State::Done;
//}


/*
	DrawAction Class
*/

FirstDrawAction::FirstDrawAction(const PlayerIdType &id, unsigned int num) : pid(id), drawnNum(num) {}

FirstDrawAction::~FirstDrawAction() {};

void FirstDrawAction::executeOn(const shared_ptr<GameState> &gstate) {
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

/*
	DrawAction Class
*/

DrawAction::DrawAction(DrawType drType, const PlayerIdType &id, unsigned int num, bool checkOver) : drawType(drType), pid(id), drawnNum(num), checkGameOverOnDrawn(checkOver) {
}

DrawAction::~DrawAction() {
};

void DrawAction::executeOn(const shared_ptr<GameState> &gstate) {
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

/*SetupActivePet Class*/

SetupActivePet::SetupActivePet(const PlayerIdType &id, unsigned int idx) : pid(id), handIdx(idx) {}
SetupActivePet::~SetupActivePet(){}

void SetupActivePet::executeOn(const std::shared_ptr<GameState> &gstate) {
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
		return BattleSceneNS::SequenceAction::create({}); //TODO
}

shared_ptr<GameAction> SetupActivePet::clone() const {
	return make_shared<SetupActivePet>(pid, handIdx);
}

/*SetupBenchPet Class*/

SetupActiveBench::SetupActiveBench(const PlayerIdType &id, unsigned int hIdx, unsigned int bIdx) : pid(id), handIdx(hIdx), benchIdx(bIdx) {}
SetupActiveBench::~SetupActiveBench() {}

void SetupActiveBench::executeOn(const std::shared_ptr<GameState> &gstate) {
	state = State::Process;

	auto hand = gstate->getHand(pid);
	auto board = gstate->getBoard(pid);

	suc = false;
	// Kiem tra handIdx co phai basic pet card
	auto petCard = dynamic_pointer_cast<PetCard>(hand->getCardAt(handIdx));
	suc = board->addBasicPetCardToBench(petCard, benchIdx);
	if (suc)
		hand->removeCard(handIdx);

	state = State::Done;
}

shared_ptr<BattleSceneNS::BSAction> SetupActiveBench::getBSAction() const {
	if (suc)
		return BattleSceneNS::SequenceAction::create({}); //TODO
}

shared_ptr<GameAction> SetupActiveBench::clone() const {
	return make_shared<SetupActiveBench>(pid, handIdx, benchIdx);
}

/*StartSetupAction Class*/

StartSetupAction::StartSetupAction() {}

StartSetupAction::~StartSetupAction() {}

void StartSetupAction::executeOn(const shared_ptr<GameState> &gstate) {
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


/*StartSetupActive Class*/

StartSetupActivePet::StartSetupActivePet(const PlayerIdType &id) {}

StartSetupActivePet::~StartSetupActivePet() {}

void StartSetupActivePet::executeOn(const shared_ptr<GameState> &gstate) {
	state = State::Process;
	state = State::Done;
}

shared_ptr<BattleSceneNS::BSAction> StartSetupActivePet::getBSAction() const {
	return nullptr;
}

shared_ptr<GameAction> StartSetupActivePet::clone() const {
	return make_shared<StartSetupAction>();
}

/*StartSetupBenchPet Class*/

StartSetupBenchPet::StartSetupBenchPet(const PlayerIdType &id) {}

StartSetupBenchPet::~StartSetupBenchPet() {}

void StartSetupBenchPet::executeOn(const shared_ptr<GameState> &gstate) {
	state = State::Process;
	state = State::Done;
}

shared_ptr<BattleSceneNS::BSAction> StartSetupBenchPet::getBSAction() const {
	return nullptr;
}

shared_ptr<GameAction> StartSetupBenchPet::clone() const {
	return make_shared<StartSetupBenchPet>(pid);
}

//---///
//---///
//---///


/*
	GameOverAction Class
*/

GameOverAction::GameOverAction(const PlayerIdType &wid) : winnerId(wid) {}

GameOverAction::~GameOverAction() {}

void GameOverAction::executeOn(const shared_ptr<GameState> &gstate) {
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