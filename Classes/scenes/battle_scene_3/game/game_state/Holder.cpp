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
	if (!pcard)
		return false;

	auto data = pcard->getPetData();
	bool isBasic = data->isBasicCard();
	bool suc = false;
	if (isBasic && !petCard) {
		petCard = pcard;
		suc = true;
	}
	
	if (suc) {
		onPlayedTurn = gstate->getTurnCount();

		// Dispatach Event OnPlay
		// TODO
	}
	return suc;
}

bool Holder::evolvePetCard(const std::shared_ptr<PetCard> &petCard) {
	return false;
}

bool Holder::attachEnergyCard(const std::shared_ptr<EnergyCard> &energyCard) {
	energyCards.push_back(energyCard);
	return true;
}

NS_GAME_END