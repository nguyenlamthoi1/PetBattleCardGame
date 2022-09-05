#pragma once 

#include "../GameDefine.h"

#include <functional>

NS_GAME_BEGIN

class GameAction {
public:
	enum class Type {
		None,
		Custom,
		DrawCard
	};

	enum class State {
		Wait,
		Process,
		Done
	};
	GameAction() = default;
	virtual ~GameAction() = default;

	virtual void start() = 0;
	virtual Type getType() const = 0;

	bool suc = false;
	State state = State::Wait;
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

class DrawAction : public GameAction {
public:
	enum class DrawType {
		FromTop,
		FromBottom
	};

	DrawAction(DrawType drType, const PlayerIdType &id, unsigned int num);
	virtual ~DrawAction();

	virtual void start() override;

	virtual Type getType() const override { return Type::DrawCard; }

	PlayerIdType pid;
	unsigned int drawnNum = 0;
	DrawType drawType = DrawType::FromTop;
protected:
	void startDrawFromBottom();
	void startDrawTop();

};


class SetupAction : public GameAction {

};


NS_GAME_END