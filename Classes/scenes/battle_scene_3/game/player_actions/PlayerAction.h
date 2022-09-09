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
		SetupActivePet
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
	PA_DoForMe() = default;
	virtual ~PA_DoForMe() = default;
};

class PA_SetupActive : public PlayerAction {
public:
	PA_SetupActive(const PlayerIdType &id, unsigned int hIdx) : pid(id), handIdx(hIdx) {};
	virtual ~PA_SetupActive() = default;
	virtual Type getType() const override { return Type::SetupActivePet; }
	PlayerIdType pid;
	unsigned int handIdx = 0;
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