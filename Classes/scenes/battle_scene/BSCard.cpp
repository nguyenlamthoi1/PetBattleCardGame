#include "BSCard.h"
#include "data/CardData.h"
#include "GameManager.h"
#include "common/ResourcePool.h"
#include "common/Utilize.h"
#include "components/WidgetTouchComponent.h"
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

const Size BSCard::ORIGIN_CARD_SIZE(390, 567);
const float BSCard::CARD_SCALE_DOWN = 1.0f / 4;
Size BSCard::CARD_SIZE = Size(ORIGIN_CARD_SIZE.width * CARD_SCALE_DOWN, ORIGIN_CARD_SIZE.height * CARD_SCALE_DOWN);


BSCard* BSCard::createWithData(const std::shared_ptr<const CardData> &data) {
	if (!data)
		return nullptr;

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
	setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	setBackGroundColorOpacity(50);
	setBackGroundColor(Color3B::RED);

	
	return true;
}

bool BSCard::onTouchHold() {
	CCLOG("Hold");
	return true;
}


/*
	PetCard Class
*/

PetCard* PetCard::createWithData(const std::shared_ptr<const CardData> &data) {
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
	ui::Helper::doLayout(cardNode);
	
	if (!cardNode)
		return false;
	this->addChild(cardNode);
	auto layoutSize = this->getContentSize();
	cardNode->setScale(CARD_SCALE_DOWN); // *TODO: Nen tinh toan ra gia tri scale, sao cho kich thuong card bang dung voi CARD_SIZE
	cardNode->setPosition(layoutSize.width / 2, layoutSize.height / 2);
	nameText = static_cast<ui::Text*>(Utilize::getChildByName(cardNode, "Pokemon_Name"));
	nameText->setString("");
	hpText = static_cast<ui::Text*>(Utilize::getChildByName(cardNode, "Hp_Value_Text"));
	hpText->setString("0");

	auto td = static_cast<ui::Layout*>(Utilize::getChildByName(cardNode, "Touch_Detector"));
	WidgetTouchNS::setWidgetTouchHold(td, bind(&PetCard::onTouchHold, this));
	td->setTouchEnabled(true);

	return true;
}

bool PetCard::initWithData(const std::shared_ptr<const CardData> &dta) {
	if (!init())
		return false;

	data = dynamic_pointer_cast<const PetCardData>(data);
	if (!data || !data->isValid()) // Khoi tao thanh cong 1 empty card
		return true;
}

BATTLE_SCENE_NS_END