#include "CardData.h"

CardData::CardData(CardId cid, Type t) : id(cid), type(t) {

}

CardData::~CardData() {

}

/*
	PetCardDataClass
*/

PetCardData::PetCardData(CardId cid) : CardData(cid, Type::Pet) {

}

PetCardData::~PetCardData() {

}