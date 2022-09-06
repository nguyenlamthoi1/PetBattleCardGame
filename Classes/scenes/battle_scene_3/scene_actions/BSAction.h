#pragma once

#include "cocos2d.h"
#include "scenes/battle_scene/BSDefine.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>
#include <memory>

class EventHandler;

BATTLE_SCENE_NS_BEG

class BattleScene;

class BSAction {
public:
	friend class BattleScene;

	enum class ActionType{
		None,

		Custom,
		Sequence,

		Wait,
		Draw_Card,
		Setup
	};

	enum class State{
		Wait,
		Processed,
		Done
	};

	using ActionPtr = std::shared_ptr<BSAction>;

	BSAction();
	virtual ~BSAction();

	virtual void executeOn(BattleScene *btlScn) = 0;
	virtual ActionType getType() const = 0;

	State state = State::Wait;
};


class SequenceAction : public BSAction {
public:
	using ActionPtr = std::shared_ptr<BSAction>;

	static std::shared_ptr<SequenceAction> create(std::initializer_list<ActionPtr> list);
	
	SequenceAction() = default;
	SequenceAction(std::initializer_list<ActionPtr> list);
	virtual ~SequenceAction();

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::Sequence; }
protected:
	std::list<std::shared_ptr<BSAction>> actions;
};

class CustomAction : public BSAction{
public:
	using CustomFunction = std::function<void()>;
	CustomAction(const CustomFunction &f);
	virtual ~CustomAction();

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::Custom; }
protected:
	std::function<void()> doFunc;
};

class WaitAction : public BSAction {
public:
	static const std::string WAIT_ACTION_SCHEDULER;
	WaitAction(float t);
	virtual ~WaitAction();

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::Wait; }
protected:
	float waitTime = 0.0f;
};

class DrawCardAction : public BSAction {
public:
	using CardId = std::string;
	using CardList = std::initializer_list<CardId>;

	DrawCardAction() = default;
	DrawCardAction(const PlayerIdType &id);
	DrawCardAction(const PlayerIdType &id, size_t n, CardList list);
	DrawCardAction(const PlayerIdType &id, size_t n, const std::vector<CardId> &list);

	virtual ~DrawCardAction();

	virtual void setDraw(size_t, CardList list);
	virtual void setDraw(size_t, const std::vector<CardId> &list);

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::Custom; }
	
protected:
	PlayerIdType pid = PlayerIdInvalid;
	size_t drawnNum = 0;
	std::vector<CardId> drawnCards;
	std::shared_ptr<EventHandler> onDrawnDone;
};

//class FlipCoinAction : public BSAction {
//public:
//	enum class FlipType {
//		None, 
//		Flip_1,
//		Flip_Mul,
//		Flip_Until_Tails
//	};
//	static FlipCoinAction* createFlip1Coin(std::shared_ptr<BattleManager> &btlMgr, PlayerIdType id);
//	static FlipCoinAction* createFlipMulCoins(std::shared_ptr<BattleManager> &btlMgr, PlayerIdType id, unsigned int count);
//	static FlipCoinAction* createFlipUntilTails(std::shared_ptr<BattleManager> &btlMgr, PlayerIdType id);
//
//	FlipCoinAction(std::shared_ptr<BattleManager> &btlMgr, PlayerIdType id);
//	virtual ~FlipCoinAction();
//
//	virtual void start() override;
//	virtual void end() override;
//	virtual ActionType getType() override { return ActionType::Custom; }
//
//protected:
//	PlayerIdType playerId = PlayerIdInvalid;
//	FlipType flipType = FlipType::None;
//	unsigned int flipNum = 0;
//	bool untilTails = false;
//};
//
//class EndGameAction : public BSAction {
//public:
//	//static EndGameAction* createFoundWinner();
//	//static EndGameAction* createDrawMatch();
//};
//
//class SetupAction : public BSAction {
//public:
//	SetupAction(std::shared_ptr<BattleManager> &btlMgr, PlayerIdType id);
//	virtual ~SetupAction();
//
//	virtual void start() override;
//	virtual void end() override;
//	virtual ActionType getType() override { return ActionType::Setup; }
//
//protected:
//	PlayerIdType playerId = PlayerIdInvalid;
//	bool checkSetup();
//	cocos2d::EventListenerCustom *playPetListener = nullptr;
//	
//};

class GameOverAction : public BSAction {
public:
	
	GameOverAction(const PlayerIdType &pid = "");
	virtual ~GameOverAction();

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::Sequence; }
protected:
	PlayerIdType winnerId;
};

BATTLE_SCENE_NS_END