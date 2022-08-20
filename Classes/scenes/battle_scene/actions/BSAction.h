#ifndef __BS_ACTION_H__
#define __BS_ACTION_H__

#include "cocos2d.h"
#include "scenes/battle_scene/BSDefine.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

BATTLE_SCENE_NS_BEG

class BattleScene;
class BattleManager;

class BSAction {
public:
	friend class BattleScene;

	enum class ActionType{
		None,
		Custom,
		Wait,
		Draw_Card,
		Setup
	};

	enum class State{
		Wait,
		Processed,
		Done
	};
	BSAction();
	BSAction(std::shared_ptr<BattleManager> &bm);
	virtual ~BSAction();
	virtual void start() = 0;
	virtual void end() = 0;
	virtual ActionType getType() = 0;

	void pushedTo(BattleScene *bs); // * Khong virtual, tat ca subClass se deu su dung function nay
	void pop();

	State state;
	int pipIdx = -1;
protected:
	BattleScene *bs;
	std::shared_ptr<BattleManager> btlMgr;
};


class CustomAction : public BSAction{
public:
	CustomAction(std::function<void()> f);
	virtual ~CustomAction();

	virtual void start() override;
	virtual void end() override;
	virtual ActionType getType() override { return ActionType::Draw_Card; }
protected:
	std::function<void()> doFunc;
};


class WaitAction : public BSAction {
public:
	static const std::string WAIT_ACTION_SCHEDULER;
	WaitAction(float t);
	virtual ~WaitAction();

	virtual void start() override;
	virtual void end() override;
	virtual ActionType getType() override { return ActionType::Draw_Card; }
protected:
	float waitTime = 0.0f;
};


class DrawCardAction : public BSAction {
public:
	DrawCardAction(std::shared_ptr<BattleManager> &btlMgr, PlayerIdType id, size_t n);
	virtual ~DrawCardAction();

	virtual void start() override;
	virtual void end() override;
	virtual ActionType getType() override { return ActionType::Custom; }

protected:
	PlayerIdType playerId = PlayerIdInvalid;
	size_t drawnNum = 0;
};

class FlipCoinAction : public BSAction {
public:
	enum class FlipType {
		None, 
		Flip_1,
		Flip_Mul,
		Flip_Until_Tails
	};
	static FlipCoinAction* createFlip1Coin(std::shared_ptr<BattleManager> &btlMgr, PlayerIdType id);
	static FlipCoinAction* createFlipMulCoins(std::shared_ptr<BattleManager> &btlMgr, PlayerIdType id, unsigned int count);
	static FlipCoinAction* createFlipUntilTails(std::shared_ptr<BattleManager> &btlMgr, PlayerIdType id);

	FlipCoinAction(std::shared_ptr<BattleManager> &btlMgr, PlayerIdType id);
	virtual ~FlipCoinAction();

	virtual void start() override;
	virtual void end() override;
	virtual ActionType getType() override { return ActionType::Custom; }

protected:
	PlayerIdType playerId = PlayerIdInvalid;
	FlipType flipType = FlipType::None;
	unsigned int flipNum = 0;
	bool untilTails = false;
};

class EndGameAction : public BSAction {
public:
	//static EndGameAction* createFoundWinner();
	//static EndGameAction* createDrawMatch();
};

class SetupAction : public BSAction {
public:
	SetupAction(std::shared_ptr<BattleManager> &btlMgr, PlayerIdType id);
	virtual ~SetupAction();

	virtual void start() override;
	virtual void end() override;
	virtual ActionType getType() override { return ActionType::Setup; }

protected:
	PlayerIdType playerId = PlayerIdInvalid;
	bool checkSetup();
	cocos2d::EventListenerCustom *playPetListener = nullptr;
	
};


BATTLE_SCENE_NS_END

#endif // __BS_ACTION_H__
