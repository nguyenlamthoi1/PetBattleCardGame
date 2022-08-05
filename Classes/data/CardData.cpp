#include "CardData.h"


CardData::CardData(CardId cid, Type t) : id(cid), type(t) {

}

CardData::~CardData() {

}

/*
	PetCardData Class
*/

PetCardData::PetCardData(CardId cid) : CardData(cid, Type::Pet) {

}

PetCardData::~PetCardData() {

}

/*
	EnergyCardData Class
*/

EnergyCardData::EnergyCardData(CardId cid, std::string name, std::string type, unsigned char num, std::string img) :
	CardData(cid, Type::Energy),
	eName(name),
	eType(type),
	eNum(num),
	image(img)
{

}

EnergyCardData::~EnergyCardData() {

}