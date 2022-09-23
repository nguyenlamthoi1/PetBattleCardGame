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

	//void setGameState(GameState* gameState);

	BoardPtr clone() const;

	HolderPtr getActiveHolder() const { return active; }
	HolderPtr getBenchHolder(unsigned int idx) const;
	HolderPtr getFirstBenchHolderHasActive() const;
	std::vector<HolderPtr> getAllHolders() const;
	const std::vector<HolderPtr>& getBenchHolders() const;
	unsigned int getFreeBenchCount() const;

	bool benchHasPet() const;
	bool hasActivePet() const;
	bool addBasicPetCardToActive(const std::shared_ptr<PetCard> &petCard, unsigned int tCount);
	bool addBasicPetCardToBench(const std::shared_ptr<PetCard> &petCard, unsigned int tCount);
	bool isBenchFull() const;
	bool evolvePetAtBench(const std::shared_ptr<PetCard> &petCard, unsigned int benchIdx);
protected:
	PlayerIdType pid = PlayerIdInvalid; /// TAG_CLONE - OK
	HolderPtr active; /// TAG_CLONE - OK
	unsigned int benchCapacity = 5; /// TAG_CLONE - OK
	HolderPtrVec bench; /// TAG_CLONE - OK
};

NS_GAME_END
