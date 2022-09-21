#pragma once

#include "../GameDefine.h"

#include <memory>
#include <vector>

NS_GAME_BEGIN

class Holder;
class PetCard;
class Card;
class EnergyCard;
class GameState;

class Board {
public:
	using HolderPtr = std::shared_ptr<Holder>;
	using HolderPtrVec = std::vector<HolderPtr>;
	using BoardPtr = std::shared_ptr<Board>;

	static BoardPtr createShPtr(GameState *gameState, const const PlayerIdType &id);

	Board(const PlayerIdType &id);
	virtual ~Board();
	virtual bool init();

	void setGameState(GameState* gameState);

	BoardPtr clone() const;

	HolderPtr getActiveHolder() const { return active; }
	HolderPtr getBenchHolder(unsigned int idx) const;
	HolderPtr getFirstBenchHolderHasActive() const;
	std::vector<HolderPtr> getAllHolders() const;
	const std::vector<HolderPtr>& getBenchHolders() const;

	bool benchHasPet() const;
	bool hasActivePet() const;
	bool addBasicPetCardToActive(const std::shared_ptr<PetCard> &petCard);
	bool evolvePetAtActive(const std::shared_ptr<PetCard> &petCard);
	bool addBasicPetCardToBench(const std::shared_ptr<PetCard> &petCard);
	bool isBenchFull() const;
	bool evolvePetAtBench(const std::shared_ptr<PetCard> &petCard, unsigned int benchIdx);
protected:
	GameState* gstate = nullptr; // weak pointer
	PlayerIdType pid = PlayerIdInvalid;

	HolderPtr active;
	unsigned int benchCapacity = 5;
	//unsigned int curBench = 0;
	HolderPtrVec bench;
};

NS_GAME_END
