#pragma once 

#include "../GameDefine.h"
#include <functional>

namespace BattleSceneNS {
	class BSAction;
}

class ActionData;

NS_GAME_BEGIN

class GameState;
class PlayerAction;
class Holder;

class GameAction {
public:
	enum class Type {
		None,
		Custom,

		DrawFirstPrizeCards,

		StartSetup,
		StartSetupActive,
		StartSetupBench,
		SetupActive,
		SetupBench,
		EndSetup,

		TurnStart,
		ChooseTurnAction,

		DrawOnTurnStart,
		UseActiveMove,
		Attack,
		PetKnockedOut,
		SelectPrizeCards,
		GetPrizeCards,
		PlayBasicPet,

		RetreatPet,
		PlayEvolvePet,
		PlayEnergyCard,

		SwitchToBench,

		flipCoin,
		FirstDrawCard,
		DrawFromDeck,

		SelectFromHand, // *
		SelectFromDiscard, // *
		SelectFromDeck, // *
		SelectFromBoard, // *

		SelectFromPet, // *
		SelectEnergiesFromPet, //*

		SelectEnergyCardFromPet, //*

		GetSelectedFromDiscard,
		GetSelectedFromPrize,
		GetSelectedFromDeck,
		GetSeletectedFromPet,
		SwitchSelectedWithActive,
		//TODO: More

		gainBuff, //*
		removeBuff, //*

		applySpecialCondition, // *
		removeSpecialCondition, // *

		DiscardSelectedCardFromHand, // *
		DiscardSelectedEnergyCardFromPet, // *
		DiscardAttachedItemFromPet,
		PetDie, // *

		GameOver, //*
	

		
	};

	enum class State {
		Wait,
		Process,
		Done
	};
	GameAction() = default;
	virtual ~GameAction() = default;

	virtual void executeOn(GameState *state) = 0;
	virtual Type getType() const = 0;
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const = 0;
	virtual std::shared_ptr<GameAction> clone() const;

	bool suc = false;
	State state = State::Wait;

protected:
	
};

class WaitInputAction : public GameAction {
public:
	WaitInputAction() = default;
	virtual ~WaitInputAction() = default;
	virtual std::vector<std::shared_ptr<PlayerAction>> getPossibleMoves(GameState *gameState) const;
	virtual ActionError onReceiveInput(GameState *gameState, const std::shared_ptr<PlayerAction> &move);

protected:
};

class Card;

class FirstDrawAction : public GameAction {
public:
	using CardId = std::string;

	FirstDrawAction(const PlayerIdType &id, unsigned int num);
	virtual ~FirstDrawAction();

	virtual void executeOn(GameState *state) override;
	virtual Type getType() const override { return Type::FirstDrawCard; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	PlayerIdType pid;
	unsigned int drawnNum = 0;

protected:
	// Result
	std::vector<CardId> cardIdsVec;
};

class DrawAction : public GameAction {
public:
	using CardId = std::string;

	enum class DrawType {
		FromTop,
		FromBottom
	};

	DrawAction(DrawType drType, const PlayerIdType &id, unsigned int num, bool checkOver = false);
	virtual ~DrawAction();

	virtual void executeOn(GameState *state) override;
	virtual Type getType() const override { return Type::DrawFromDeck; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	PlayerIdType pid;
	unsigned int drawnNum = 0;
	DrawType drawType = DrawType::FromTop;
	bool checkGameOverOnDrawn = false;

protected:
	// Result
	std::vector<CardId> cardIdsVec;
};

class DrawOnTurnStart : public GameAction {
public:
	using CardId = std::string;

	DrawOnTurnStart(const PlayerIdType &id) : pid(id) {}
	virtual ~DrawOnTurnStart() = default;

	virtual void executeOn(GameState *state) override;
	virtual Type getType() const override { return Type::DrawOnTurnStart; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	PlayerIdType pid;
	unsigned int drawnNum = 1;
protected:
	std::vector<CardId> cardIdsVec;
};

//--Setup Flow--//

class StartSetupAction: public GameAction {
public:
	StartSetupAction() = default;
	virtual ~StartSetupAction() = default;

	virtual void executeOn(GameState *state) override;
	virtual Type getType() const override { return Type::StartSetup; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;
};

class StartSetupActivePet : public WaitInputAction {
public:

	StartSetupActivePet(const PlayerIdType &id) : pid(id) {}
	virtual ~StartSetupActivePet() = default;

	virtual void executeOn(GameState *gameState) override;
	virtual Type getType() const override { return Type::StartSetupActive; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;
	
	virtual std::vector<std::shared_ptr<PlayerAction>> getPossibleMoves(GameState *gameState) const override;
	virtual ActionError onReceiveInput(GameState *gameState, const std::shared_ptr<PlayerAction> &move);

	PlayerIdType pid;
	unsigned int handIdx = 0;

protected:
};

class StartSetupBenchPet : public WaitInputAction{
public:

	StartSetupBenchPet(const PlayerIdType &id) : pid(id) {}
	virtual ~StartSetupBenchPet() = default;

	virtual void executeOn(GameState *gameState) override;
	virtual Type getType() const override { return Type::StartSetupBench; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	virtual std::vector<std::shared_ptr<PlayerAction>> getPossibleMoves(GameState *gameState) const override;
	virtual ActionError onReceiveInput(GameState *gameState, const std::shared_ptr<PlayerAction> &move);

	PlayerIdType pid;
protected:
};

class SetupActivePet : public GameAction {
public:

	SetupActivePet(const PlayerIdType &id, unsigned int idx);
	virtual ~SetupActivePet();

	virtual void executeOn(GameState *gameState) override;
	virtual Type getType() const override { return Type::SetupActive; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	PlayerIdType pid;
	unsigned int handIdx = 0;

protected:
	// Result
	std::shared_ptr<BattleSceneNS::BSAction> bsAction;
};

class SetupBenchPet : public GameAction {
public:

	SetupBenchPet(const PlayerIdType &id, unsigned int hIdx) : pid(id), handIdx(hIdx) {}
	virtual ~SetupBenchPet() = default;

	virtual void executeOn(GameState *gameState) override;
	virtual Type getType() const override { return Type::SetupActive; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	PlayerIdType pid;
	unsigned int handIdx = 0;

protected:
	// Result
	std::shared_ptr<BattleSceneNS::BSAction> bsAction;
};

class EndSetup : public GameAction {
public:

	EndSetup() = default;
	virtual ~EndSetup() = default;

	virtual void executeOn(GameState *gameState) override;
	virtual Type getType() const override { return Type::EndSetup; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;
protected:
};

//--------------//

class FlipCoinGetFirst : public GameAction {
public:
	
	FlipCoinGetFirst() = default;
	virtual ~FlipCoinGetFirst() = default;

	virtual void executeOn(GameState *gameState) override;
	virtual Type getType() const override { return Type::EndSetup; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

protected:
	//Result
	//unsigned int firstIdx;
	PlayerIdType firstId;
};

class OnTurnStart : public GameAction {
public:

	//OnTurnStart(const PlayerIdType &id) : pid(id) {};
	OnTurnStart(unsigned int idx, const PlayerIdType &id) : pidx(idx), pid(id) {};

	virtual ~OnTurnStart() = default;

	virtual void executeOn(GameState *gameState) override;
	virtual Type getType() const override { return Type::TurnStart; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	PlayerIdType pid;
	unsigned int pidx;
};

class PlayerChooseTurnAction : public WaitInputAction {
public:

	PlayerChooseTurnAction(const PlayerIdType &id) : pid(id) {}
	virtual ~PlayerChooseTurnAction() = default;

	virtual void executeOn(GameState *gameState) override;
	virtual Type getType() const override { return Type::ChooseTurnAction; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	virtual std::vector<std::shared_ptr<PlayerAction>> getPossibleMoves(GameState *gameState) const override;
	virtual ActionError onReceiveInput(GameState *gameState, const std::shared_ptr<PlayerAction> &move);

	PlayerIdType pid;
protected:
};


class GameOverAction : public GameAction {
public:
	GameOverAction(const PlayerIdType &pid = "");
	virtual ~GameOverAction();

	virtual void executeOn(GameState *state) override;
	virtual Type getType() const override { return Type::GameOver; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	bool isWinnerFound() const;
	bool isDrawnGame() const;

	const PlayerIdType winnerId;
};

class DrawFirstPrizeCards : public GameAction {
public:
	using CardId = std::string;
	DrawFirstPrizeCards(const PlayerIdType &id, unsigned int num) : pid(id), drawnNum(num) {}
	virtual ~DrawFirstPrizeCards() = default;

	virtual void executeOn(GameState *state) override;
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	virtual Type getType() const override { return Type::DrawFirstPrizeCards; }

	const PlayerIdType pid;
	unsigned int drawnNum;
	std::vector<CardId> cidVec;
};

class SelectPrizeCards : public WaitInputAction {
public:

	SelectPrizeCards(const PlayerIdType &id, unsigned int selectNum) : pid(id), num(selectNum) {}
	virtual ~SelectPrizeCards() = default;

	virtual void executeOn(GameState *gameState) override;
	virtual Type getType() const override { return Type::SelectPrizeCards; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	virtual std::vector<std::shared_ptr<PlayerAction>> getPossibleMoves(GameState *gameState) const override;
	virtual ActionError onReceiveInput(GameState *gameState, const std::shared_ptr<PlayerAction> &move);

	PlayerIdType pid;
	unsigned int num = 0;
protected:
};

class GetPrizeCards : public GameAction {
public:

	GetPrizeCards(const PlayerIdType &id) : pid(id) {}
	//GetPrizeCards(const PlayerIdType &id, const std::vector<unsigned int> &vec) : pid(id), idxVec(vec.begin(), vec.end()) {}

	virtual ~GetPrizeCards() = default;

	virtual void executeOn(GameState *gameState) override;
	virtual Type getType() const override { return Type::GetPrizeCards; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	PlayerIdType pid;
	std::vector<unsigned int> idxVec;
};

////////////////////////////////////
// Play Basic Pet Card From Hand ///
////////////////////////////////////

class PlayerPlayPetCardToBench : public GameAction {
public:

	PlayerPlayPetCardToBench(const PlayerIdType &id, unsigned int handIdx) : pid(id), hIdx(handIdx)  {}
	virtual ~PlayerPlayPetCardToBench() = default;

	virtual void executeOn(GameState *gameState) override;
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;
	virtual Type getType() const override { return Type::PlayBasicPet; }

	PlayerIdType pid;
	unsigned int hIdx;
protected:
	bool suc = false; // T: Game state changed
};


////////////////////////////////////
////////////////////////////////////
////////////////////////////////////


////////////////////////////////////////
// Play Evolution Pet Card From Hand ///
////////////////////////////////////////

class PlayEvPetCard : public GameAction {
public:

	PlayEvPetCard(
		const PlayerIdType &id, 
		unsigned int handIdx, 
		bool toActive, 
		unsigned int benchIdx = 0) : 
		pid(id), hIdx(handIdx), isActive(toActive), bIdx(benchIdx) {}
	virtual ~PlayEvPetCard() = default;

	virtual void executeOn(GameState *gameState) override;
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;
	virtual Type getType() const override { return Type::PlayBasicPet; }

	PlayerIdType pid;
	unsigned int hIdx;
	bool isActive = false;
	unsigned int bIdx;

protected:
	bool suc = false; // T: Game state changed
};

////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////


///////////////////////
// Energy Attaching ///
///////////////////////

class PlayEnergyCard : public GameAction {
public:
	enum class PlaceType {
		Active,
		Bench
	};

	PlayEnergyCard(const PlayerIdType &id, unsigned int handIdx,  PlaceType pType, unsigned int bIdx = 0) 
		: 
		pid(id),
		hIdx(handIdx),
		placeType(pType),
		benchIdx(bIdx)
	{}
	virtual ~PlayEnergyCard() = default;

	virtual Type getType() const override { return Type::PlayEnergyCard; }
	virtual void executeOn(GameState *gameState) override;
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	PlayerIdType pid;
	unsigned int hIdx;
	PlaceType placeType;
	unsigned int benchIdx;
protected:
	bool suc = false;
};

/////////////////////////
/////////////////////////
/////////////////////////


/////////////////////////
// ALL ATTACK ACTIONS ///
/////////////////////////

class UseActiveMove : public GameAction {
public:
	UseActiveMove(const PlayerIdType &id, unsigned int moveIdx) : pid(id), midx(moveIdx) {}
	virtual ~UseActiveMove() = default;

	virtual void executeOn(GameState *gameState) override;
	virtual Type getType() const override { return Type::UseActiveMove; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	std::shared_ptr<GameAction> createAction(GameState *gstate, const std::shared_ptr<const ActionData> &adata);

	PlayerIdType pid;
	unsigned int midx;
};

class AttackAction : public GameAction {
public:
	virtual Type getType() const override final { return Type::Attack; }
};

class DefaultAttack: public AttackAction{ // Active Attack Active
public:
	DefaultAttack(
		const PlayerIdType &atkOwner, 
		//const PlayerIdType &defOwner, 
		unsigned int dmg) :
		attacker(atkOwner),
		//defender(defOwner),
		baseDmg(dmg) {}

	virtual ~DefaultAttack() = default;

	virtual void executeOn(GameState *gameState) override;
	virtual std::shared_ptr<GameAction> clone() const override;
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;

	unsigned int baseDmg = 0;
	PlayerIdType attacker;
protected:
	bool triggerWeak = false;
	bool triggerResist = false;
	unsigned int totalDmg = 0;
};

class PetKnockedOut : public AttackAction { // Active Attack Active
public:
	PetKnockedOut(const PlayerIdType &id, bool active, unsigned int benchIdx) :
	pid(id),
	isActive(active),
	bIdx(benchIdx)
	{}
	virtual ~PetKnockedOut() = default;

	virtual void executeOn(GameState *gameState) override;
	virtual std::shared_ptr<GameAction> clone() const override;
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;

	PlayerIdType pid;
	bool isActive = false;
	unsigned int bIdx;
};

class SwitchActiveWithBench : public GameAction {
public:
	SwitchActiveWithBench(const PlayerIdType &id, unsigned int bIdx) :
		pid(id),
		benchIdx(bIdx)
	{}
	virtual ~SwitchActiveWithBench() = default;

	virtual Type getType() const override { return Type::SwitchToBench; }
	virtual void executeOn(GameState *gameState) override;
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	PlayerIdType pid;
	unsigned int benchIdx;
protected:
	bool suc = false;
};


class TakeDamage : public GameAction {
public:
	TakeDamage(const std::shared_ptr<Holder> &taker, 
		int total, 
		int base,
		bool weakEnable,
		bool resistEnable) 
		: 
		dmgTaker(taker), 
		totalDmg(total), 
		baseDmg(base), 
		triggerWeak(weakEnable), 
		triggerResistance(resistEnable)
	{

	}

	virtual ~TakeDamage() = default;

	virtual void executeOn(GameState *gameState) override;
	virtual std::shared_ptr<GameAction> clone() const override;
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;

	std::shared_ptr<Holder> dmgTaker;
	int totalDmg = 0;
	int baseDmg = 0;
	bool triggerWeak = false;
	bool triggerResistance = false;
};

/////////////////////////
/////////////////////////
/////////////////////////


NS_GAME_END