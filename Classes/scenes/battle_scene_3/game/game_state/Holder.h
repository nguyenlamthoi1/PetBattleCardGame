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
	HolderPtr clone() const;
	void copyFrom(const std::shared_ptr<Holder> &withHolder);

	Holder(const PlayerIdType &id);
	virtual ~Holder();
	virtual bool init();
	//void setGameState(GameState *gameState) { gstate = gameState; }


	PlayerIdType getOwnerId() const { return pid; }
	unsigned int getPlayedTurn() const { return onPlayedTurn; }
	PetCardPtr getPetCard() const { return petCard; }
	bool hasPet() const { return petCard != nullptr; }
	bool addBasicPetCard(const std::shared_ptr<PetCard> &petCard, unsigned int tCount);
	bool canEvolveTo(const std::shared_ptr<const PetCard> &petCard) const;
	bool evolvePetCardTo(const std::shared_ptr<const PetCard> &petCard, unsigned int tCount);
	
	bool enoughEnergies(const std::map<std::string, unsigned int> &rqMap) const;

	bool canAttachEnergy() const;
	bool attachEnergyCard(const std::shared_ptr<const EnergyCard> &energyCard);

	bool takeDmg(unsigned int dmg);
	bool isKnockedOut() const { return dmgCounter >= maxHp; }
	void setDmgCounter(unsigned int dmg) { dmgCounter = dmg; }
	unsigned int getCurDmg() const { return dmgCounter; }

	void removePetAndAllCards(std::vector<std::shared_ptr<const Card>> &vec);
	
	std::shared_ptr<const PetCard> removePetCard();
	void removeAllEvPetCards(std::vector<std::shared_ptr<const PetCard>> &vec);
	void removeAllEnergyCards(std::vector<std::shared_ptr<const EnergyCard>> &vec);
	
	struct HolderData{
		PetCardPtr petCard;
		std::vector<PetCardPtr> evPetCards; // Danh sach card tien hoa cua petCard
		std::vector<EnergyCardPtr> energyCards;
		std::unordered_map<std::string, unsigned int> totalEnergy;

		unsigned int onPlayedTurn = 0;
		unsigned int dmgCounter = 0;
		unsigned int maxHp = 0;
	};
	
	bool switchWithHolder(const std::shared_ptr<Holder> &withHolder);
	void updateWithNewHolderData(const HolderData &data);
	
	void onKnockedOut();
protected:
	///GameState *gstate = nullptr;

	PlayerIdType pid = PlayerIdInvalid; /// TAG_CLONE - OK

	PetCardPtr petCard; /// TAG_CLONE - OK
	std::vector<PetCardPtr> evPetCards;  /// TAG_CLONE - OK
	std::vector<EnergyCardPtr> energyCards; /// TAG_CLONE - OK
	std::unordered_map<std::string, unsigned int> totalEnergy; /// TAG_CLONE - OK

	unsigned int onPlayedTurn = 0; /// TAG_CLONE - OK
	unsigned int dmgCounter = 0; /// TAG_CLONE - OK
	unsigned int maxHp = 0; /// TAG_CLONE - OK
};

NS_GAME_END