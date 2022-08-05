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

class BSAction {
public:
	friend class BattleScene;

	enum class ActionType{
		NONE,
		Draw_Card
	};

	enum class State{
		Wait,
		Processed,
		Done
	};
	BSAction();
	virtual ~BSAction();
	virtual void start() = 0;
	virtual void end() = 0;
	virtual ActionType getType() = 0;

	void pushedTo(BattleScene *bs); // * Khong virtual, tat ca subClass se deu su dung function nay
	void pop();

	State state;
protected:
	BattleScene *bs;
};


class CustomAction : public BSAction{
public:

};

class DrawCardAction : public BSAction {
public:
	DrawCardAction(PlayerIdType id, size_t n);

	virtual void start() override;
	virtual void end() override;
	virtual ActionType getType() override { return ActionType::Draw_Card; }
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
