#pragma once

#include "../GameDefine.h"

#include <vector>
#include <memory>
#include <string>

NS_GAME_BEGIN

class Card;
class PetCard;
class EnergyCard;
class GameState;
class Board;

class Holder {
public:
	friend class Board;
	using PetCardPtr = std::shared_ptr<const PetCard>;
	using EnergyCardPtr = std::shared_ptr<const EnergyCard>;
	using HolderPtr = std::shared_ptr<Holder>;

	static HolderPtr createShPtr(const PlayerIdType &id);

	Holder(const PlayerIdType &id);
	virtual ~Holder();
	virtual bool init();
	void setGameState(GameState *gameState) { gstate = gameState; }

	bool hasPet() const { return petCard != nullptr; }
	bool addBasicPetCard(const std::shared_ptr<PetCard> &petCard);
	bool evolvePetCard(const std::shared_ptr<PetCard> &petCard);
protected:
	GameState *gstate = nullptr;

	PlayerIdType pid = PlayerIdInvalid;

	PetCardPtr petCard;
	std::vector<PetCardPtr> evPetCards; // Danh sach card tien hoa cua petCard
	unsigned int onPlayedTurn = 0;

	EnergyCardPtr energyCards;
};

NS_GAME_END