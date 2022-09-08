#pragma once 

#include "../GameDefine.h"

#include <functional>

namespace BattleSceneNS {
	class BSAction;
}

NS_GAME_BEGIN

class GameState;

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

	virtual void executeOn(const std::shared_ptr<GameState> &state) = 0;
	virtual Type getType() const = 0;
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const = 0;
	virtual std::shared_ptr<GameAction> clone() const;

	bool suc = false;
	State state = State::Wait;

protected:
	
};

//class CustomAction : public GameAction {
//public:
//	using CustomFunction = std::function<void()>;
//	CustomAction(const CustomFunction &f);
//	virtual ~CustomAction();
//
//	virtual void start() override;
//	virtual Type getType() const override { return Type::Custom; }
//protected:
//	std::function<void()> doFunc;
//};

class Card;

class FirstDrawAction : public GameAction {
public:
	using CardId = std::string;

	FirstDrawAction(const PlayerIdType &id, unsigned int num);
	virtual ~FirstDrawAction();

	virtual void executeOn(const std::shared_ptr<GameState> &state) override;
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

	virtual void executeOn(const std::shared_ptr<GameState> &state) override;
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

	virtual void executeOn(const std::shared_ptr<GameState> &state) override;
	virtual Type getType() const override { return Type::StartSetup; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;
};

class StartSetupActivePet : public GameAction {
public:

	StartSetupActivePet(const PlayerIdType &id);
	virtual ~StartSetupActivePet();

	virtual void executeOn(const std::shared_ptr<GameState> &gameState) override;
	virtual Type getType() const override { return Type::StartSetupActive; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	PlayerIdType pid;
	unsigned int handIdx = 0;

protected:
};

class StartSetupBenchPet : public GameAction {
public:

	StartSetupBenchPet(const PlayerIdType &id);
	virtual ~StartSetupBenchPet();

	virtual void executeOn(const std::shared_ptr<GameState> &gameState) override;
	virtual Type getType() const override { return Type::StartSetupBench; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	PlayerIdType pid;
protected:
};

class SetupActivePet : public GameAction {
public:

	SetupActivePet(const PlayerIdType &id, unsigned int idx);
	virtual ~SetupActivePet();

	virtual void executeOn(const std::shared_ptr<GameState> &gameState) override;
	virtual Type getType() const override { return Type::SetupActive; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	PlayerIdType pid;
	unsigned int handIdx = 0;

protected:
	// Result
	std::shared_ptr<BattleSceneNS::BSAction> bsAction;
};

class SetupActiveBench : public GameAction {
public:

	SetupActiveBench(const PlayerIdType &id, unsigned int handIdx, unsigned int benchIdx);
	virtual ~SetupActiveBench();

	virtual void executeOn(const std::shared_ptr<GameState> &gameState) override;
	virtual Type getType() const override { return Type::SetupActive; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	PlayerIdType pid;
	unsigned int handIdx = 0;
	unsigned int benchIdx = 0;

protected:
	// Result
	std::shared_ptr<BattleSceneNS::BSAction> bsAction;
};

//--------------//


class EndTurnAction : public GameAction {
	
};

class GameOverAction : public GameAction {
public:
	GameOverAction(const PlayerIdType &wid = "");
	virtual ~GameOverAction();

	virtual void executeOn(const std::shared_ptr<GameState> &state) override;
	virtual Type getType() const override { return Type::GameOver; }
	virtual std::shared_ptr<BattleSceneNS::BSAction> getBSAction() const override;
	virtual std::shared_ptr<GameAction> clone() const override;

	bool isWinnerFound() const;
	bool isDrawnGame() const;

	const PlayerIdType winnerId;
};


NS_GAME_END