#include "Card.h"
#include "GameManager.h"

using namespace std;

NS_GAME_BEGIN

PetCard::PetCard(Type t, const CardId &cid) : Card(t, cid) {};


PetCard::PetCard(Type t, const CardId &cid, const CPetDataPtr &data) : Card(t, cid) {
	setData(data);
}

PetCard::~PetCard(){}

/// Getters, Setters & Checkers

void PetCard::setData(const CDataPtr &dtaPtr) {
	petData = dynamic_pointer_cast<const PetCardData>(dtaPtr);
}

PetCard::CDataPtr PetCard::getData() const {
	return petData;
}

PetCard::CPetDataPtr PetCard::getPetData() const {
	return petData;
}


bool PetCard::isValid() const {
	return Card::isValid() && (petData && petData->id == id && petData->isValid());
}


NS_GAME_END