#include "Holder.h"
#include "card/Card.h"
#include "data/CardData.h"
#include "GameState.h"

using namespace std;

NS_GAME_BEGIN

Holder::HolderPtr Holder::createShPtr(const PlayerIdType &id) {
	auto ret = make_shared<Holder>(id);
	if (!ret || !ret->init())
		ret.reset(); // nullptr
	return ret;
}

Holder::Holder(const PlayerIdType &id) : pid(id) {}

Holder::~Holder(){}

bool Holder::init() {
	return true;
}

bool Holder::addBasicPetCard(const std::shared_ptr<PetCard> &pcard) {
	if (!pcard || petCard)
		return false;

	auto data = pcard->getPetData();
	bool isBasic = data->isBasicCard();
	bool suc = false;
	if (isBasic) {
		petCard = pcard;
		suc = true;
	}
	
	if (suc) {
		onPlayedTurn = gstate->getTurnCount();
		dmgCounter = 0;
		maxHp = data->hp;

		// Dispatach Event OnPlay
		// TODO
	}
	return suc;
}

bool Holder::canEvolveTo(const std::shared_ptr<const PetCard> &toCard) const {
	if (!toCard || !petCard)
		return false;
	auto toCardData = toCard->getPetData();
	auto fromCardData = petCard->getPetData();
	if (toCardData->evolveFrom != fromCardData->name)
		return false;
	return true;
}

bool Holder::evolvePetCardTo(const std::shared_ptr<const PetCard> &toCard) {
	if(!toCard || !petCard)
		return false;

	bool suc = false;
	
	auto toCardData = toCard->getPetData();
	auto fromCardData = petCard->getPetData();

	if (toCardData->evolveFrom == fromCardData->name) { // * Co the tien hoa
		evPetCards.push_back(petCard);
		petCard = toCard;
		suc = true;
	}

	if (suc)
	{
		onPlayedTurn = gstate->getTurnCount();
		maxHp = toCardData->hp;
	}

	return suc;
}

bool Holder::attachEnergyCard(const std::shared_ptr<const EnergyCard> &energyCard) {
	if (!energyCard || !petCard)
		return false;

	energyCards.push_back(energyCard);
	auto eData = energyCard->getEnergyData();
	auto eType = eData->eType;
	auto eNum = eData->eNum;
	totalEnergy[eType] += eNum;
	return true;
}

bool Holder::canAttachEnergy() const {
	return petCard != nullptr;
}

bool Holder::enoughEnergies(const std::map<std::string, unsigned int> &rqMap) const {
	bool enough = true;
	for (const auto & itr : rqMap) {
		auto eType = itr.first;
		auto eNum = itr.second;
		enough &= totalEnergy.at(eType) >= eNum;
	}
	return enough;
}

NS_GAME_END