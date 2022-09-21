#include "Holder.h"
#include "card/Card.h"
#include "data/CardData.h"
#include "GameState.h"
#include "DiscardPile.h"

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
		auto itr = totalEnergy.find(eType);
		enough &= (itr!= totalEnergy.cend()) && (totalEnergy.at(eType) >= eNum);
	}
	return enough;
}

bool Holder::takeDmg(unsigned int dmg) {
	dmgCounter += dmg;
	return true;
}

void Holder::onKnockedOut() {
}

void Holder::removePetAndAllCards(std::vector<std::shared_ptr<const Card>> &vec) {
	vec.push_back(petCard);
	petCard = nullptr;

	vec.insert(vec.cend(), evPetCards.begin(), evPetCards.end());
	evPetCards.clear();

	vec.insert(vec.cend(), energyCards.begin(), energyCards.end());
	energyCards.clear();
	totalEnergy.clear();

	onPlayedTurn = 0;
	dmgCounter = 0;
	maxHp = 0;
}
std::shared_ptr<const PetCard> Holder::removePetCard() {
	auto ret = petCard;
	petCard = nullptr;

	dmgCounter = 0;
	onPlayedTurn = 0;
	maxHp = 0;

	return ret;
}
void Holder::removeAllEvPetCards(std::vector<std::shared_ptr<const PetCard>> &vec) {
	vec.insert(vec.cend(), evPetCards.begin(), evPetCards.end());
	evPetCards.clear();
}
void Holder::removeAllEnergyCards(std::vector<std::shared_ptr<const EnergyCard>> &vec) {
	vec.insert(vec.cend(), energyCards.begin(), energyCards.end());
	energyCards.clear();
	totalEnergy.clear();
}

void Holder::switchWithHolder(const std::shared_ptr<Holder> &withHolder) {
	if (!withHolder->hasPet())
		return;

	// Luu thong tin cu
	HolderData oldData1;
	oldData1.petCard = petCard;
	oldData1.evPetCards = evPetCards;
	oldData1.energyCards = energyCards;
	oldData1.totalEnergy = totalEnergy;
	oldData1.onPlayedTurn = onPlayedTurn;
	oldData1.dmgCounter = dmgCounter;
	oldData1.maxHp = maxHp;

	// Cap nhat thong tin moi
	petCard = withHolder->petCard;
	evPetCards = withHolder->evPetCards;
	energyCards = withHolder->energyCards;
	totalEnergy = withHolder->totalEnergy;
	onPlayedTurn = withHolder->onPlayedTurn;
	dmgCounter = withHolder->dmgCounter;
	maxHp = withHolder->maxHp;

	withHolder->petCard = oldData1.petCard;
	withHolder->evPetCards = oldData1.evPetCards;
	withHolder->energyCards = oldData1.energyCards;
	withHolder->totalEnergy = oldData1.totalEnergy;
	withHolder->onPlayedTurn = oldData1.onPlayedTurn;
	withHolder->dmgCounter = oldData1.dmgCounter;
	withHolder->maxHp = oldData1.maxHp;

	//withHolder->updateWithNewHolderData(oldData1);
	
}

void Holder::updateWithNewHolderData(const HolderData &data) {
	// Cap nhat thong tin moi
	petCard = data.petCard;
	evPetCards = data.evPetCards;
	energyCards = data.energyCards;
	totalEnergy = data.totalEnergy;
	onPlayedTurn = data.onPlayedTurn;
	dmgCounter = data.dmgCounter;
	maxHp = data.maxHp;
}



NS_GAME_END