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
		Draw_Card
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

class EndGameAction : public BSAction {
public:
	//static EndGameAction* createFoundWinner();
	//static EndGameAction* createDrawMatch();
};

BATTLE_SCENE_NS_END

#endif // __BS_ACTION_H__
