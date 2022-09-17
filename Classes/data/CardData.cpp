#include "CardData.h"

using namespace std;

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

///--///

bool CardData::checkSelectType(GConfig::SelectType type, const std::shared_ptr<const CardData> &data) {
	
	bool ret = false;
	switch (type) {
	case(GConfig::SelectType::Pet_Any): 
	{
		ret = dynamic_pointer_cast<const PetCardData>(data) != nullptr;
		break; 
	}
	case(GConfig::SelectType::Basic_Energy_Any): {
		ret = dynamic_pointer_cast<const EnergyCardData>(data) != nullptr;
		break;
	}
	case(GConfig::SelectType::Basic_Energy_Fire): {
		auto edata = dynamic_pointer_cast<const EnergyCardData>(data);
		ret = edata && edata->eType == CardDefine::Energy::Fire;
		break;
	}
	}
	return ret;
}
