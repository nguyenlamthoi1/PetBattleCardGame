#pragma once

#include "../GameDefine.h"

#include <vector>
#include <memory>
#include <string>

NS_GAME_BEGIN

class Card;
class PetCard;
class EnergyCard;

class Holder {
public:
	using PetCardPtr = std::shared_ptr<const PetCard>;
	using EnergyCardPtr = std::shared_ptr<const EnergyCard>;
	using HolderPtr = std::shared_ptr<Holder>;

	static HolderPtr createShPtr(const PlayerIdType &id);

	Holder(const PlayerIdType &id);
	virtual ~Holder();
	virtual bool init();

protected:
	PlayerIdType pid = PlayerIdInvalid;

	PetCardPtr petCard;
	EnergyCardPtr energyCards;
};

NS_GAME_END