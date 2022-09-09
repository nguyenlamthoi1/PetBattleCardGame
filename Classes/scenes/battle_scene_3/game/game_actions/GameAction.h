#pragma once 

#include "../GameDefine.h"

#include <functional>

namespace BattleSceneNS {
	class BSAction;
}

NS_GAME_BEGIN

class GameState;
class PlayerAction;

class GameAction {
public:
	enum class Type {
		None,
		Custom,

		StartSetup,
		StartSetupActive,
		StartSetupBench,
		SetupActive,
		SetupBench,
		EndSetup,

		TurnStart,

		ChooseTurnAction,
		PlayBasicPet,
		EvolvePet,
		RetreatPet,
		AttackPet, // Quan trong
		useAbility,
		useItemCard, // Su dung giong nhu spter card hoac attach vao Pet
		
		discard_item_card,
		
		UseAttack, 

		// Action ho tro
		branchAction, // Quan trong
		healPet, // *
		flipCoin, // *

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

		UpdateBetweenTurn, // *
		SwitchPlayerTurn, //*
		GameOver //*
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


//--Setup Flow--//

class StartSetupAction: public GameAction {
public:
	StartSetupAction();
	virtual ~StartSetupAction();

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
	PlayerIdType firstIdx;
};

class OnTurnStart : public GameAction {
public:

	OnTurnStart(const PlayerIdType &id) : pid(id) {};
	virtual ~OnTurnStart() = default;

	virtual void executeOn(GameState *gameState) override;
	virtual Type getType() const override { return Type::EndSetup; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

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


NS_GAME_END