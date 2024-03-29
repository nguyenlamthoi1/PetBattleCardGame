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
		EndTurn,
		ActiveUseMove,
		AttachEnergy,
		EvolvePet,
		Select_Prize_Cards,
		PlayPetCardToBench,
		SelectBench,
		RetreatWith
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

class PA_AttachEnergy : public PlayerAction {
public:
	enum class PlaceType {
		Active,
		Bench
	};
	PA_AttachEnergy(
		const PlayerIdType &id, 
		unsigned int handIdx, 
		PlaceType pType,
		unsigned int bIdx) :
		pid(id), 
		hidx(handIdx),
		placeType(pType),
		benchIdx(bIdx) {};
	virtual ~PA_AttachEnergy() = default;
	virtual Type getType() const override { return Type::AttachEnergy; }
	PlayerIdType pid;
	unsigned int hidx;
	PlaceType placeType;
	unsigned int benchIdx;
};

class PA_EvPetCard : public PlayerAction {
public:
	PA_EvPetCard(
		const PlayerIdType &id,
		unsigned int handIdx,
		bool toActive,
		unsigned int bIdx) :
		pid(id),
		hidx(handIdx),
		isActive(toActive),
		benchIdx(bIdx) {};
	virtual ~PA_EvPetCard() = default;
	virtual Type getType() const override { return Type::EvolvePet; }

	PlayerIdType pid;
	unsigned int hidx;
	bool isActive;
	unsigned int benchIdx;
};

class PA_UseMove : public PlayerAction {
public:
	PA_UseMove(const PlayerIdType &id, unsigned int moveIdx) : pid(id), mIdx(moveIdx) {};
	virtual ~PA_UseMove() = default;
	virtual Type getType() const override { return Type::ActiveUseMove; }
	PlayerIdType pid;
	unsigned int mIdx;
};

class PA_PlayPetCardToBench : public PlayerAction {
public:
	PA_PlayPetCardToBench(const PlayerIdType &id, unsigned int hIdx) : pid(id), handIdx(hIdx) {};
	virtual ~PA_PlayPetCardToBench() = default;
	virtual Type getType() const override { return Type::PlayPetCardToBench; }

	PlayerIdType pid;
	unsigned int handIdx = 0;
};

class PA_SelectPrizeCards : public PlayerAction {
public:

	PA_SelectPrizeCards(const PlayerIdType &id, std::initializer_list<unsigned int> idxVec);
	PA_SelectPrizeCards(const PlayerIdType &id, const std::vector<unsigned int> &idxVec);

	virtual ~PA_SelectPrizeCards() = default;
	virtual Type getType() const override { return Type::Select_Prize_Cards; }

	PlayerIdType pid;
	std::vector<unsigned int> idxVec;
};

class PA_SelectBench : public PlayerAction {
public:

	PA_SelectBench(const PlayerIdType &id, unsigned int bIdx) : pid(id), benchIdx(bIdx) {}
	virtual ~PA_SelectBench() = default;
	virtual Type getType() const override { return Type::SelectBench; }

	PlayerIdType pid;
	unsigned int benchIdx = 0;
};

class PA_RetreatWith : public PlayerAction {
public:

	PA_RetreatWith(const PlayerIdType &id, unsigned int bIdx) : pid(id), benchIdx(bIdx) {}
	virtual ~PA_RetreatWith() = default;
	virtual Type getType() const override { return Type::RetreatWith; }

	PlayerIdType pid;
	unsigned int benchIdx = 0;
};


NS_GAME_END