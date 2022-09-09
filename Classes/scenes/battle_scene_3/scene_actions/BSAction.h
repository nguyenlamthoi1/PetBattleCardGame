#pragma once

#include "cocos2d.h"
#include "../BSDefine.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>
#include <memory>

class EventHandler;

namespace MGame {
	class BattleMaster;
	class PlayerAction;
};

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
		
		StartSetup,
		StartSetupActive,
		SetupActive,
		StartSetupBench,
		SetupBench
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



class WaitInputPlayer : public BSAction {
public:
	virtual bool onReceivePlayerInput(const std::shared_ptr<MGame::BattleMaster> &bm, const std::shared_ptr<MGame::PlayerAction> &pAction) { return false; }
};

//-----------//
//SET UP FLOW//
//-----------//

class StartSetupAction : public BSAction {
public:
	const static std::string START_SETUP_TXT;

	StartSetupAction();
	virtual ~StartSetupAction();

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::StartSetup; }
};

class StartSetupActive : public WaitInputPlayer {
public:
	const static std::string PLAYER_SETUP_TXT;
	const static std::string OPP_SETUP_TXT;

	StartSetupActive(const PlayerIdType &pid);
	virtual ~StartSetupActive();

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { 
		return ActionType::StartSetupActive; 
	}
	virtual bool onReceivePlayerInput(const std::shared_ptr<MGame::BattleMaster> &bm, const std::shared_ptr<MGame::PlayerAction> &pAction) override;
protected:
	PlayerIdType pid;
};

class StartSetupBench : public BSAction {
public:
	StartSetupBench(const PlayerIdType &pid);
	virtual ~StartSetupBench();

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::StartSetupBench; }
protected:
	PlayerIdType pid;
};

class DoSetupPetActive : public BSAction {
public:
	DoSetupPetActive(const PlayerIdType &pid, unsigned int handIdx);
	virtual ~DoSetupPetActive();

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::SetupActive; }
protected:
	PlayerIdType pid;
	unsigned int handIdx;
};

class DoSetupPetBench : public BSAction {
public:
	DoSetupPetBench(const PlayerIdType &pid, unsigned int handIdx, unsigned int benchIdx);
	virtual ~DoSetupPetBench();

	virtual void executeOn(BattleScene *btlScn) override;
	virtual ActionType getType() const override { return ActionType::SetupBench; }
protected:
	PlayerIdType pid;
	unsigned int handIdx = 0;
	unsigned int benchIdx = 0;
};


//-----------//
//-----------//
//-----------//


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