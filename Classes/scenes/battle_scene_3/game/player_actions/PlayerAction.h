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
		
	};

	PlayerAction(Type t, const PlayerIdType &id) : type(t), pid(id) {}
	PlayerAction() = default;
	virtual ~PlayerAction() = default;
	virtual bool isValid() { return type != Type::None && !pid.empty(); }

	PlayerIdType pid;
	Type type = Type::None;
};


class PA_Setup : public PlayerAction {

};

class PA_PlayPetCardFromHand : public PlayerAction {

};

class PA_AttachEnergyFromHand : public PlayerAction {

};

class PA_PlayItemCardFromHand : public PlayerAction {

};

class PA_AttachItemCardFromHand : public PlayerAction {

};

class PA_PlaySupporterCardFromHand : public PlayerAction {

};

class PA_SelectCardsFromHand : public PlayerAction {

};

class PA_SelectCardsFromDiscard : public PlayerAction {

};

class PA_SelectCardsFromDeck : public PlayerAction {

};

class PA_SelectPetOnBoard : public PlayerAction {

};

NS_GAME_END