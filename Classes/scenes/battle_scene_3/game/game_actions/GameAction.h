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
		DrawCard,
		Setup,

		GameOver
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
	virtual Type getType() const override { return Type::DrawCard; }
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
	virtual Type getType() const override { return Type::DrawCard; }
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

class StartSetup : public GameAction {
	StartSetup(const PlayerIdType &id);
	virtual ~StartSetup();

	PlayerIdType pid;
	virtual void executeOn(const std::shared_ptr<GameState> &state) override;
	virtual Type getType() const override { return Type::DrawCard; }
};

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