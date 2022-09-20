#include "GameAction.h"

#include "../BattleMaster.h"
#include "../game_state/GameState.h"
#include "../game_state/Deck.h"
#include "../game_state/Hand.h"
#include "../game_state/Board.h"
#include "../game_state/DiscardPile.h"
#include "../game_state/PrizePile.h"
#include "../game_state/Holder.h"
#include "../game_state/Player.h"

#include "data/CardData.h"

#include "../game_state/card/Card.h"

#include "../../BattleScene.h"
#include "../../scene_actions/BSAction.h"

#include "../player_actions/PlayerAction.h"

using namespace std;
USING_NS_CC;
//using namespace BattleSceneNS;

NS_GAME_BEGIN

//---------------------//
//GameAction Base Class//
//---------------------//

std::shared_ptr<GameAction> GameAction::clone() const {
	return nullptr;
}

std::shared_ptr<BattleSceneNS::BSAction> GameAction::getBSAction() const {
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
			auto insertIdx = cocos2d::RandomHelper::random_int(0, (int)cardObjVec.size() - 1);
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
		if (card->getType() == Card::Type::Pet) {
			auto petCard = dynamic_pointer_cast<PetCard>(card);
			auto data = petCard->getPetData();
			if (petCard && data->isBasicCard())
				ret.push_back(make_shared<PA_SetupActive>(pid, handIdx));
		}
		++handIdx;
	}
	return ret;
}

ActionError StartSetupActivePet::onReceiveInput(GameState *gstate, const std::shared_ptr<PlayerAction> &move) {
	if (state != State::Process)
		return ActionError::Failed;

	if (move->getType() == PlayerAction::Type::SetupActivePet) {
		auto setupMove = dynamic_pointer_cast<PA_SetupActive>(move);

		if (!setupMove || setupMove->pid != pid)
			return ActionError::Failed;

		auto handIdx = setupMove->handIdx;
		auto hand = gstate->getHand(pid);
		auto card = hand->getCardAt(handIdx);
		auto petCard = dynamic_pointer_cast<PetCard> (card);

		if (petCard && petCard->isBasicCard()) {
			state = State::Done;
			/*gstate->pushActionsAtFront({
				make_shared<SetupActivePet>(pid, handIdx),
				make_shared<StartSetupBenchPet>(pid)
				});*/

			gstate->replaceCurActionWith({
				make_shared<SetupActivePet>(pid, handIdx),
				make_shared<StartSetupBenchPet>(pid)
				});
			return ActionError::Succeeded;
		}
	}
	else if (move->getType() == PlayerAction::Type::DoForMe) {
		auto pMoves = getPossibleMoves(gstate);
		if (!pMoves.empty()) {
			auto randIdx = cocos2d::RandomHelper::random_int(0, (int) pMoves.size() - 1);
			auto chosenMove = pMoves[randIdx];
			return onReceiveInput(gstate, chosenMove);
		}
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
		if (card->getType() == Card::Type::Pet) {
			auto petCard = dynamic_pointer_cast<PetCard>(card);
			auto data = petCard->getPetData();
			if (petCard && data->isBasicCard())
				ret.push_back(make_shared<PA_SetupBench>(pid, handIdx));
		}

		++handIdx;
	}

	ret.push_back(make_shared<PA_EndTurn>(pid));

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
			/*gstate->pushActionsAtFront({ 
				make_shared<SetupBenchPet>(pid, handIdx),
				make_shared<StartSetupBenchPet>(pid),
				});*/

			gstate->replaceCurActionWith({
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
			//gstate->pushActionsAtFront({ make_shared<StartSetupActivePet>(otherId) });
			gstate->replaceCurActionWith({ make_shared<StartSetupActivePet>(otherId) });

			return ActionError::Succeeded;
		}
		else {
			state = State::Done;
			/*gstate->pushActionsAtFront({ 
				make_shared<FlipCoinGetFirst>(),
				make_shared<EndSetup>(),
				});*/
			gstate->replaceCurActionWith({
				make_shared<FlipCoinGetFirst>(),
				make_shared<EndSetup>(),
				});
			return ActionError::Succeeded;
		}

	}
	else if (move->getType() == PlayerAction::Type::DoForMe) {
		auto pMoves = getPossibleMoves(gstate);
		if (!pMoves.empty()) {
			auto randIdx = cocos2d::RandomHelper::random_int(0, (int)pMoves.size() - 1);
			return onReceiveInput(gstate, pMoves[randIdx]);
		}
	}

	return ActionError::Failed;
}
	//EndSetup Class//
void EndSetup::executeOn(GameState *gstate) {
	if (state != State::Wait)
		return;
	state = State::Process;
	gstate->endSetup();
	auto firstIdx = gstate->getFirstIdx();
	auto firstId = gstate->getPlayerIdAt(gstate->getFirstIdx());

	//gstate->pushActionsAtFront({make_shared<OnTurnStart>(firstIdx, firstId)}); //??? Why deleted
	gstate->replaceCurActionWith({ make_shared<OnTurnStart>(firstIdx, firstId) }); //??? Why deleted


	state = State::Done;
}

shared_ptr<BattleSceneNS::BSAction> EndSetup::getBSAction() const {
	return BattleSceneNS::SequenceAction::create({
	make_shared<BattleSceneNS::WaitAction>(0.7f),
	make_shared<BattleSceneNS::DoEndSetup>()
		});
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
	static constexpr bool Heads = true;
	static constexpr bool Tails = true;

	if (state != State::Wait)
		return;
	state = State::Process;
	auto firstIdx = cocos2d::RandomHelper::random_int(0, (int)gstate->getPlayerCount() - 1);
	firstId = gstate->getPids()[firstIdx];
	gstate->setFirstIdx(firstIdx);
	
	state = State::Done;
}

shared_ptr<BattleSceneNS::BSAction> FlipCoinGetFirst::getBSAction() const {
	return make_shared<BattleSceneNS::FlipCoinGetFirstPlayer>(firstId);
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

/*
	DrawFirstPrizeCards
*/

void DrawFirstPrizeCards::executeOn(GameState *gstate) {
	state = State::Process;

	auto deck = gstate->getDeck(pid);
	auto drawnVec = deck->popTop(drawnNum);
	auto prizePile = gstate->getPrizePile(pid);
	prizePile->pushCards(drawnVec);

	for (const auto & card : drawnVec) {
		cidVec.push_back(card->getId());
	}

	state = State::Done;
}
shared_ptr<BattleSceneNS::BSAction> DrawFirstPrizeCards::getBSAction() const {
	return BattleSceneNS::SequenceAction::create({
	make_shared<BattleSceneNS::WaitAction>(0.7f),
	make_shared<BattleSceneNS::DrawPrizeCards>(pid, cidVec)
	});
}
shared_ptr<GameAction> DrawFirstPrizeCards::clone() const {
	return make_shared<DrawFirstPrizeCards>(pid, drawnNum);
}


/*
	SelectPrizeCards Class
*/

void SelectPrizeCards::executeOn(GameState *gameState) { 
	state = State::Process; 
}

shared_ptr<BattleSceneNS::BSAction> SelectPrizeCards::getBSAction() const {
	return BattleSceneNS::SequenceAction::create({
		make_shared<BattleSceneNS::WaitAction>(0.7f),
		make_shared<BattleSceneNS::SelectPrizeAction>(pid, num),
		});
}

shared_ptr<GameAction> SelectPrizeCards::clone() const {
	return make_shared<SelectPrizeCards>(pid, num);
}

vector<shared_ptr<PlayerAction>> SelectPrizeCards::getPossibleMoves(GameState *gstate) const {
	vector<shared_ptr<PlayerAction>> ret;
	
	auto prizePile = gstate->getPrizePile(pid);
	auto retVec = prizePile->getFirstSelectables(num);
	ret.push_back(make_shared<PA_SelectPrizeCards>(pid, retVec));

	// TODO

	return ret;
}

ActionError SelectPrizeCards::onReceiveInput(GameState *gstate, const std::shared_ptr<PlayerAction> &move) {
	if (state != State::Process)
		return ActionError::Failed;

	if (move->getType() == PlayerAction::Type::Select_Prize_Cards) {
		auto selectMove = dynamic_pointer_cast<PA_SelectPrizeCards>(move);

		if (!selectMove || selectMove->pid != pid || selectMove->idxVec.size() != num)
			return ActionError::Failed;

		auto &idxVec = selectMove->idxVec;
		auto prizePile = gstate->getPrizePile(pid);
		auto check = prizePile->canGetCardsAt(idxVec);
		if (check) {
			prizePile->saveSelectedCardsAt(idxVec); // Luu lai thong tin lua chon
			state = State::Done;
			return ActionError::Succeeded;
		}

		return ActionError::Failed;
	}
	else if (move->getType() == PlayerAction::Type::DoForMe) {
		auto prizePile = gstate->getPrizePile(pid);
		auto selectedVec = prizePile->getFirstSelectables(num);
		auto pMove = make_shared<PA_SelectPrizeCards>(pid, selectedVec);
		return onReceiveInput(gstate, pMove);
	}

	return ActionError::Failed;
}


void GetPrizeCards::executeOn(GameState *gstate) {
	state = State::Process;

	auto prizePile = gstate->getPrizePile(pid);
	auto &selected = prizePile->selected;
	auto &cardVec = prizePile->cardVec;

	vector<shared_ptr<Card>> drawnVec;
	for (const auto &idx : selected) {
		if (0 <= idx && idx < cardVec.size() && cardVec[idx]) {
			drawnVec.push_back(cardVec[idx]);
			idxVec.push_back(idx);
			cardVec[idx].reset(); // reset to nullptr
		}
	}

	auto hand = gstate->getHand(pid);
	hand->pushCards(drawnVec);

	state = State::Done;
}

shared_ptr<GameAction> GetPrizeCards::clone() const {
	return make_shared<GetPrizeCards>(pid);
}

shared_ptr<BattleSceneNS::BSAction> GetPrizeCards::getBSAction() const {
	return make_shared<BattleSceneNS::GetPrizeCards>(pid, idxVec);
}

/*
	OnTurnStart Class
*/

void OnTurnStart::executeOn(GameState *gstate) {
	state = State::Process;
	gstate->onTurnStart(pidx);
	
	gstate->replaceCurActionWithVec({
		make_shared<DrawOnTurnStart>(pid),
		make_shared<PlayerChooseTurnAction>(pid)
		});
	state = State::Done;
}

shared_ptr<GameAction> OnTurnStart::clone() const {
	return make_shared<OnTurnStart>(pidx, pid);
}

shared_ptr<BattleSceneNS::BSAction> OnTurnStart::getBSAction() const {
	return BattleSceneNS::SequenceAction::create({
	make_shared<BattleSceneNS::WaitAction>(0.7f),
	make_shared<BattleSceneNS::DoTurnStart>(pid)
		});
}

/*
	PlayerChooseTurnAction Class
*/

void PlayerChooseTurnAction::executeOn(GameState *gameState) {
	state = State::Process;
}

shared_ptr<BattleSceneNS::BSAction> PlayerChooseTurnAction::getBSAction() const {
	return make_shared<BattleSceneNS::PlayerChooseTurnAction>(pid);
}

shared_ptr<GameAction> PlayerChooseTurnAction::clone() const {
	return make_shared<PlayerChooseTurnAction>(pid);
}

vector<shared_ptr<PlayerAction>> PlayerChooseTurnAction::getPossibleMoves(GameState *gstate) const {
	vector<shared_ptr<PlayerAction>> ret;

	// EndTurn
	ret.push_back(make_shared<PA_EndTurn>(pid));
	
	auto player = gstate->getPlayer(pid);
	// Lay tat ca vi tri Pet tren Board
	auto board = gstate->getBoard(pid);
	std::vector<std::shared_ptr<Holder>> allHolders = board->getAllHolders(); // * Active Holder co idx == 0 trong vector nay
	
	auto hand = gstate->getHand(pid);
	auto cards = hand->getAllCards();
	
	// Play Basic Pet, Play Evolve Peet, Play Energy
	unsigned int handIdx = 0;
	for (const auto &card : cards) {
		switch (card->getType()) {
		case Card::Type::Pet:
		{
			auto pCard = dynamic_pointer_cast<const PetCard>(card);
			if (pCard) {
				auto pData = pCard->getPetData();
				bool isBasic = pData->evStage < 1;
				if (isBasic) {// Check Basic
					// TODO: Them action: play Basic
				}
				else { // Check Evolution
					if (!player->actionExceedsLimit(Player::TurnAction::EvolvePet)) {
						for (unsigned int i = 0; i < allHolders.size(); ++i) {
							auto holder = allHolders[i];
							bool isHolderActive = i <= 0;
							unsigned int benchIdx = isHolderActive ? 0 : i - 1;
							if (holder->canEvolveTo(pCard) && holder->getPlayedTurn() < gstate->getTurnCount())
								ret.push_back(make_shared<PA_EvPetCard>(pid, handIdx, isHolderActive, benchIdx));
						}
					}
				}
			}
			break;
		}
		case Card::Type::Energy: {
			auto eCard = dynamic_pointer_cast<const EnergyCard>(card);
			if (eCard) {
				auto eData = eCard->getEnergyData();
				if (!player->actionExceedsLimit(Player::TurnAction::AttachEnergy))
					for (unsigned int i = 0; i < allHolders.size(); ++i) {
						auto holder = allHolders[i];
						bool isHolderActive = i <= 0;
						unsigned int benchIdx = isHolderActive ? 0 : i - 1;
						if (holder->canAttachEnergy())
						{
							auto plcType = isHolderActive ? PA_AttachEnergy::PlaceType::Active : PA_AttachEnergy::PlaceType::Bench;
							ret.push_back(make_shared<PA_AttachEnergy>(pid, handIdx, plcType, benchIdx));

						}
					}
			}
			break;
		}
		}
		++handIdx;
	}
	return ret;
}

ActionError PlayerChooseTurnAction::onReceiveInput(GameState *gstate, const std::shared_ptr<PlayerAction> &move) {
	if (state != State::Process)
		return ActionError::Failed;

	// Xu ly Action Use Move
	if (move->getType() == PlayerAction::Type::ActiveUseMove) {
		auto pMove = dynamic_pointer_cast<PA_UseMove>(move);
		if (!pMove || pMove->pid != pid)
			return ActionError::Failed;
		auto moveIdx = pMove->mIdx;

		// TODO

		return ActionError::Failed;
	}
	else if (move->getType() == PlayerAction::Type::AttachEnergy) {
		auto pMove = dynamic_pointer_cast<PA_AttachEnergy>(move);
		if (!pMove || pMove->pid != pid)
			return ActionError::Failed;

		auto board = gstate->getBoard(pid);
		auto hand = gstate->getHand(pid);
		auto card = hand->getCardAt(pMove->hidx);
		auto eCard = dynamic_pointer_cast<EnergyCard>(card);
		if (eCard) {
			auto player = gstate->getPlayer(pid);
			bool check = !player->actionExceedsLimit(Player::TurnAction::AttachEnergy);
			if (check) {
				if (pMove->placeType == PA_AttachEnergy::PlaceType::Active) {
					gstate->replaceCurActionWith({
						make_shared<PlayEnergyCard>(pMove->pid, pMove->hidx,PlayEnergyCard::PlaceType::Active),
						make_shared<PlayerChooseTurnAction>(pid)
						});
					state = State::Done;
					return ActionError::Succeeded;
				}
				else {
					gstate->replaceCurActionWith({
						make_shared<PlayEnergyCard>(pMove->pid, pMove->hidx, PlayEnergyCard::PlaceType::Bench, pMove->benchIdx),
						make_shared<PlayerChooseTurnAction>(pid)
						});
					state = State::Done;
					return ActionError::Succeeded;
				}
			}
		}

		return ActionError::Failed;
	}
	else if (move->getType() == PlayerAction::Type::EvolvePet) {
		auto pMove = dynamic_pointer_cast<PA_EvPetCard>(move);
		if (!pMove || pMove->pid != pid)
			return ActionError::Failed;

		auto board = gstate->getBoard(pid);
		auto hand = gstate->getHand(pid);
		auto card = hand->getCardAt(pMove->hidx);
		auto pCard = dynamic_pointer_cast<PetCard>(card);
		if (pCard) {
			auto player = gstate->getPlayer(pid);
			bool check1 = !player->actionExceedsLimit(Player::TurnAction::EvolvePet);
			auto holder = pMove->isActive ? board->getActiveHolder() : board->getBenchHolder(pMove->benchIdx);
			bool check2 = holder->getPlayedTurn() < gstate->getTurnCount();
			if (check1 && check2) {
				if (pMove->isActive) {
					gstate->replaceCurActionWith({
						make_shared<PlayEvPetCard>(pMove->pid, pMove->hidx, pMove->isActive),
						make_shared<PlayerChooseTurnAction>(pid)
						});
					state = State::Done;
					return ActionError::Succeeded;
				}
				else {
					gstate->replaceCurActionWith({
						make_shared<PlayEvPetCard>(pMove->pid, pMove->hidx,pMove->isActive, pMove->benchIdx),
						make_shared<PlayerChooseTurnAction>(pid)
						});
					state = State::Done;
					return ActionError::Succeeded;
				}
			}
		}

		return ActionError::Failed;
	}
	else if (move->getType() == PlayerAction::Type::EndTurn) {
		auto pMove = dynamic_pointer_cast<PA_EndTurn>(move);
		if (!pMove || pMove->pid != pid)
			return ActionError::Failed;

		auto nextIdx = gstate->getNextTurnIdx();
		auto nextId = gstate->getPlayerIdAt(nextIdx);
		state = State::Done;
		
		//gstate->pushActionsAtFront({make_shared<OnTurnStart>(nextIdx, nextId)});
		gstate->replaceCurActionWithVec({ make_shared<OnTurnStart>(nextIdx, nextId) });
		return ActionError::Succeeded;
	}
	else if (move->getType() == PlayerAction::Type::DoForMe) {
		auto pMoves = getPossibleMoves(gstate);
		if (!pMoves.empty()) {
			auto randIdx = cocos2d::RandomHelper::random_int(0, (int)pMoves.size() - 1);
			return onReceiveInput(gstate, pMoves[randIdx]);
		}
	}
	else if (move->getType() == PlayerAction::Type::PlayPetCardToBench) {
		auto pMove = dynamic_pointer_cast<PA_PlayPetCardToBench>(move);
		if (!pMove || pMove->pid != pid)
			return ActionError::Failed;

		auto board = gstate->getBoard(pid);
		auto hand = gstate->getHand(pid);
		auto card = hand->getCardAt(pMove->handIdx);
		auto pCard = dynamic_pointer_cast<PetCard>(card);
		if (pCard && pCard->isBasicCard() && !board->isBenchFull()) {
			//auto player = gstate->getPlayer(pid);
			//bool check = !player->actionExceedsLimit(Player::TurnAction::PlayPetCard);
			gstate->replaceCurActionWith({
					make_shared<PlayerPlayPetCardToBench>(pMove->pid, pMove->handIdx),
					make_shared<PlayerChooseTurnAction>(pid)
				});
			state = State::Done;
			return ActionError::Succeeded;
		}
		return ActionError::Failed;
	}
	return ActionError::Failed;
}

/*
	PlayerEnergyCard Class
*/

void PlayEnergyCard::executeOn(GameState *gstate) {
	state = State::Process;

	auto hand = gstate->getHand(pid);
	auto card = hand->getCardAt(hIdx);
	auto eCard = dynamic_pointer_cast<EnergyCard>(card);

	suc = false;

	if (eCard) {
		auto board = gstate->getBoard(pid);
		auto holder = placeType == PlaceType::Active ? board->getActiveHolder() : board->getBenchHolder(benchIdx);
		suc = holder->attachEnergyCard(eCard);
	}

	if (suc) {
		hand->removeCard(hIdx);
		auto player = gstate->getPlayer(pid);
		player->updateActionCount(Player::TurnAction::AttachEnergy, 1);
	}

	state = State::Done;
}
shared_ptr<BattleSceneNS::BSAction> PlayEnergyCard::getBSAction() const {
	return suc ? make_shared<BattleSceneNS::PlayerEnergyCard>(pid, hIdx, placeType == PlaceType::Active, benchIdx) : nullptr;
}
shared_ptr<GameAction> PlayEnergyCard::clone() const {
	return make_shared<PlayEnergyCard>(pid, hIdx, placeType, benchIdx);
}


/*
	PlayEvPetCard Class
*/

void PlayEvPetCard::executeOn(GameState *gstate) {
	state = State::Process;
	suc = false;
	auto hand = gstate->getHand(pid);
	auto card = hand->getCardAt(hIdx);
	auto pCard = dynamic_pointer_cast<PetCard>(card);
	if (pCard) { // * Pet card khong NULL
		auto board = gstate->getBoard(pid);
		shared_ptr<Holder> holder = isActive ? board->getActiveHolder() : board->getBenchHolder(bIdx);
		if (holder) // * Holder khoong NULL
			suc = holder->evolvePetCardTo(pCard);
	}

	if (suc) {
		hand->removeCard(hIdx);
		auto player = gstate->getPlayer(pid);
		player->updateActionCount(Player::TurnAction::EvolvePet, 1);
	}
		
	state = State::Done;
}
shared_ptr<BattleSceneNS::BSAction> PlayEvPetCard::getBSAction() const {
	return suc ? make_shared<BattleSceneNS::PlayEvPetCard>(pid, hIdx, isActive, bIdx) : nullptr;
}
shared_ptr<GameAction> PlayEvPetCard::clone() const {
	return make_shared<PlayEvPetCard>(pid, hIdx, isActive, bIdx);
}

/*
	DrawOnTurnStart Class
*/

void DrawOnTurnStart::executeOn(GameState *gstate) {
	state = State::Process;

	auto deck = gstate->getDeck(pid);
	vector<shared_ptr<Card>> cardObjVec;
	deck->popTop(drawnNum, cardObjVec); // Rut tu Top Deck

	// Cap nhat Hand
	auto hand = gstate->getHand(pid);
	hand->pushCards(cardObjVec);

	// Cap nhat result vector
	for (const auto& cardObj : cardObjVec)
		cardIdsVec.emplace_back(cardObj->getId());

	// * Khi het bai dau moi turn phai check ket thuc game
	if (cardObjVec.size() < drawnNum) {
		gstate->replaceCurActionWith ({make_shared<GameOverAction>(gstate->getOpponentOf(pid)) });
	}

	state = State::Done;
}

std::shared_ptr<BattleSceneNS::BSAction> DrawOnTurnStart::getBSAction() const {
	return BattleSceneNS::SequenceAction::create({
		make_shared<BattleSceneNS::WaitAction>(0.7f),
		make_shared<BattleSceneNS::DrawCardAction>(pid, drawnNum, cardIdsVec),
		});
}

std::shared_ptr<GameAction> DrawOnTurnStart::clone() const {
	return  make_shared<DrawOnTurnStart>(pid);
}

/*
	PlayerPlayPetCardToBench Class
*/

void PlayerPlayPetCardToBench::executeOn(GameState *gstate) {
	state = State::Process;

	auto hand = gstate->getHand(pid);
	auto board = gstate->getBoard(pid);

	suc = false;
	// Kiem tra handIdx co phai basic pet card
	auto petCard = dynamic_pointer_cast<PetCard>(hand->getCardAt(hIdx));
	suc = board->addBasicPetCardToBench(petCard);
	if (suc)
		hand->removeCard(hIdx);

	state = State::Done;
}

std::shared_ptr<BattleSceneNS::BSAction> PlayerPlayPetCardToBench::getBSAction() const {
	return BattleSceneNS::SequenceAction::create({
		make_shared<BattleSceneNS::WaitAction>(0.5f),
		make_shared<BattleSceneNS::DoPlayPetFromHand>(pid, hIdx),
		});
}

std::shared_ptr<GameAction> PlayerPlayPetCardToBench::clone() const {
	return  make_shared<PlayerPlayPetCardToBench>(pid, hIdx);
}

NS_GAME_END