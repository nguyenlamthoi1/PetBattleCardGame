#include "BSCard.h"
#include "data/CardData.h"
#include "GameManager.h"
#include "common/ResourcePool.h"
#include "common/Utilize.h"
#include "ui/UIHelper.h"

using namespace std;
USING_NS_CC;
BATTLE_SCENE_NS_BEG

//BSCard* BSCard::create() {
//	auto card = new (nothrow) BSCard();
//	if (card && card->init()) {
//		card->autorelease();
//		return card;
//	}
//	delete card;
//	card = nullptr;
//	return card;
//}

const cocos2d::Size BSCard::CARD_SIZE(390, 567);

BSCard* BSCard::createWithData(const CardData *data) {
	BSCard *ret = nullptr;
	switch (data->type) {
	case CardData::Type::Pet:
		ret = PetCard::createWithData(data);
		break;
	}
	return ret;
}

BSCard::BSCard() {

}

BSCard::~BSCard() {

}

bool BSCard::init() {
	if (!ui::Layout::init())
		return false;

	setAnchorPoint(Vec2(0.5, 0.5));
	setContentSize(CARD_SIZE);

	return true;
}

/*
	PetCard Class
*/



PetCard* PetCard::create(const PetCardData* data) {
	auto card = new (nothrow) PetCard();
	if (card && card->initWithData(data)) {
		card->autorelease();
		return card;
	}
	delete card;
	card = nullptr;
	return card;
}

PetCard* PetCard::create() {
	auto card = new (nothrow) PetCard();
	if (card && card->init()) {
		card->autorelease();
		return card;
	}
	delete card;
	card = nullptr;
	return card;
}


bool PetCard::init() {
	if (!BSCard::init())
		return false;
	auto pool = GM_POOL;
	
	cardNode = static_cast<ui::Layout*>(pool->tryGetNodeCsb("ccstd_csb/cards/pokemon_card.csb"));
	if (!cardNode)
		return false;
	this->addChild(cardNode);

	nameText = static_cast<ui::Text*>(Utilize::getChildByName(cardNode, "Pokemon_Name"));
	nameText->setString("");
	hpText = static_cast<ui::Text*>(Utilize::getChildByName(cardNode, "Hp_Value_Text"));
	hpText->setString("0");

	return true;
}

bool PetCard::initWithData(const CardData * dta) {
	if (!init())
		return false;

	data = dynamic_cast<const PetCardData*>(dta);
	if (!data) // Khoi tao thanh cong 1 empty card
		return true;
}

BATTLE_SCENE_NS_END