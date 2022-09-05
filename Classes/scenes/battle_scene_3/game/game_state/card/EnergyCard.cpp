#include "Card.h"
#include "GameManager.h"

using namespace std;

NS_GAME_BEGIN

EnergyCard::EnergyCard(Type t, const CardId &cid) : Card(t, cid) {};


EnergyCard::EnergyCard(Type t, const CardId &cid, const CEnergyDataPtr &data) : Card(t, cid) {
	setData(data);
}

EnergyCard::~EnergyCard() {}

/// Getters, Setters & Checkers

void EnergyCard::setData(const CDataPtr &dtaPtr) {
	energyData = dynamic_pointer_cast<const EnergyCardData>(dtaPtr);
}

EnergyCard::CDataPtr EnergyCard::getData() const {
	return energyData;
}

EnergyCard::CEnergyDataPtr EnergyCard::getEnergyData() const {
	return energyData;
}


bool EnergyCard::isValid() const {
	return Card::isValid() && (energyData && energyData->id == id && energyData->isValid());
}


NS_GAME_END