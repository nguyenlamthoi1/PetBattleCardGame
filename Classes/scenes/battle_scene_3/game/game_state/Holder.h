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


	PlayerIdType getOwnerId() const { return pid; }
	unsigned int getPlayedTurn() const { return onPlayedTurn; }
	PetCardPtr getPetCard() const { return petCard; }
	bool hasPet() const { return petCard != nullptr; }
	bool addBasicPetCard(const std::shared_ptr<PetCard> &petCard);
	bool canEvolveTo(const std::shared_ptr<const PetCard> &petCard) const;
	bool evolvePetCardTo(const std::shared_ptr<const PetCard> &petCard);
	
	bool enoughEnergies(const std::map<std::string, unsigned int> &rqMap) const;

	bool canAttachEnergy() const;
	bool attachEnergyCard(const std::shared_ptr<const EnergyCard> &energyCard);

	bool takeDmg(unsigned int dmg);
	bool isKnockedOut() const { return dmgCounter >= maxHp; }
	void setDmgCounter(unsigned int dmg) { dmgCounter = dmg; }
	unsigned int getCurDmg() const { return dmgCounter; }

	void removePetAndAllCards(std::vector<std::shared_ptr<const Card>> &vec);
	void onKnockedOut();
protected:
	GameState *gstate = nullptr;

	PlayerIdType pid = PlayerIdInvalid;

	PetCardPtr petCard;
	std::vector<PetCardPtr> evPetCards; // Danh sach card tien hoa cua petCard
	std::vector<EnergyCardPtr> energyCards;
	std::unordered_map<std::string, unsigned int> totalEnergy;

	unsigned int onPlayedTurn = 0;
	unsigned int dmgCounter = 0;
	unsigned int maxHp = 0;
};

NS_GAME_END