#include "Card.h"
#include "GameManager.h"

using namespace std;

NS_GAME_BEGIN

Card::CardShPtr Card::createShPtr(const CardId &cid) {
	CardShPtr ret; /// Nullptr
	auto dtaMgr = GM_DATA_MGR;
	auto cdata = dtaMgr->getCardData(cid);
	if (cdata && cdata->isValid()) {
		switch (cdata->type) {
		case CardData::Type::Pet:
			ret = make_shared<PetCard>(Type::Pet, cdata->id, dynamic_pointer_cast<const PetCardData>(cdata));
			break;
		case CardData::Type::Energy:
			ret = make_shared<EnergyCard>(Type::Energy, cdata->id, dynamic_pointer_cast<const EnergyCardData>(cdata));
			break;
		}
	}
	return ret;
}

Card::Card(Type t, CardId cid) : type(t), id(cid) {}

Card::~Card(){}

/// Getters, Setters & Checkers
Card::Type Card::getType() const {
	return type;
}

bool Card::isType(Type checkType) const {
	return type == checkType;
}

bool Card::isValid() const {
	return type != Type::None && !id.empty();
}

Card::CardId Card::getId() const {
	return id;
}

NS_GAME_END