#pragma once

#include "../GameDefine.h"


NS_GAME_BEGIN

/*
	Abstract Class chua thong tin ve hanh dong cua nguoi choi lua choi nhu: Play Pet Card, Attach Energy Card,..
	Thong tin ve player action duoc gui den BattleMaster de update GameState
	* Khi AI ra quyet dinh cua minh, phai tao 1 PlayerAction gui den BattleMaster
*/

class PlayerAction {
public:
	enum class Type {
		None,
		DoForMe,
		SetupActivePet,
		SetupBenchPet,
		EndTurn
	};

	PlayerAction(Type t, const PlayerIdType &id) : type(t), pid(id) {}
	PlayerAction() = default;
	virtual ~PlayerAction() = default;
	virtual bool isValid() const { return type != Type::None && !pid.empty(); }
	virtual Type getType() const = 0;
	PlayerIdType pid;
	Type type = Type::None;
};

/*
	- Class dac biec, khi player gui den BattleMaster object class nay, BattleMaster se tu dong thuc thi 1 Possible Move cho player
*/
class PA_DoForMe : public PlayerAction {
public:
	PA_DoForMe(const PlayerIdType &id) : pid(id) {}
	virtual ~PA_DoForMe() = default;
	virtual Type getType() const override { return Type::DoForMe; }
	PlayerIdType pid;
};

class PA_SetupActive : public PlayerAction {
public:
	PA_SetupActive(const PlayerIdType &id, unsigned int hIdx) : pid(id), handIdx(hIdx) {};
	virtual ~PA_SetupActive() = default;
	virtual Type getType() const override { return Type::SetupActivePet; }
	PlayerIdType pid;
	unsigned int handIdx = 0;
};

class PA_SetupBench : public PlayerAction {
public:
	PA_SetupBench(const PlayerIdType &id, unsigned int hIdx) : pid(id), handIdx(hIdx) {};
	virtual ~PA_SetupBench() = default;
	virtual Type getType() const override { return Type::SetupBenchPet; }
	PlayerIdType pid;
	unsigned int handIdx = 0;
};

class PA_EndTurn : public PlayerAction {
public:
	PA_EndTurn(const PlayerIdType &id) : pid(id) {};
	virtual ~PA_EndTurn() = default;
	virtual Type getType() const override { return Type::EndTurn; }
	PlayerIdType pid;
};

//class PA_PlayPetCardFromHand : public PlayerAction {
//
//};
//
//class PA_AttachEnergyFromHand : public PlayerAction {
//
//};
//
//class PA_PlayItemCardFromHand : public PlayerAction {
//
//};
//
//class PA_AttachItemCardFromHand : public PlayerAction {
//
//};
//
//class PA_PlaySupporterCardFromHand : public PlayerAction {
//
//};
//
//class PA_SelectCardsFromHand : public PlayerAction {
//
//};
//
//class PA_SelectCardsFromDiscard : public PlayerAction {
//
//};
//
//class PA_SelectCardsFromDeck : public PlayerAction {
//
//};
//
//class PA_SelectPetOnBoard : public PlayerAction {
//
//};

NS_GAME_END