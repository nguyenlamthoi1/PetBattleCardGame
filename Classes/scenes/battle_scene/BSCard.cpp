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
	case CardData::Type::Energy:
		ret = EnergyCard::createWithData(data);
		break;
	}
	return ret;
}


BSCard::BSCard() {
	CCLOG("BSCard::Ctor %p", this);
}

BSCard::~BSCard() {
	CCLOG("BSCard::Dtor %p", this);
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

const std::string PetCard::EMPTY_PET_IMG = "pet_collection_1/000.png";

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

	auto cardLayout = cardNode->getChildByName("Card_Layout");
	petImg = dynamic_cast<ui::ImageView*>(cardLayout->getChildByName("Pokemon_Image"));
	outerImg = dynamic_cast<ui::ImageView*>(cardLayout->getChildByName("Outer_Background"));


	nameText = static_cast<ui::Text*>(Utilize::getChildByName(cardNode, "Pokemon_Name"));
	nameText->setString("");
	hpText = static_cast<ui::Text*>(Utilize::getChildByName(cardNode, "Hp_Value_Text"));
	hpText->setString("0");

	evText = static_cast<ui::Text*>(Utilize::getChildByName(cardNode, "Evo_Text"));
	evArrow = static_cast<ui::ImageView*>(Utilize::getChildByName(cardNode, "Evo_Arrow"));
	evFromText = static_cast<ui::Text*>(Utilize::getChildByName(cardNode, "Pre_Evo_Name"));
	//evFromImg = static_cast<ui::ImageView*>(Utilize::getChildByName(cardNode, "Evo_Img"));
	evFromImg = static_cast<ui::ImageView*>(cardLayout->getChildByName("Evo_Square")->getChildByName("Evo_Img"));

	/*auto td = static_cast<ui::Layout*>(Utilize::getChildByName(cardNode, "Touch_Detector"));
	WidgetTouchNS::setWidgetTouchHold(td, bind(&PetCard::onTouchHold, this));
	td->setTouchEnabled(false);
	td->setSwallowTouches(false);*/

	return true;
}

bool PetCard::initWithData(const std::shared_ptr<const CardData> &dta) {
	if (!init())
		return false;

	data = dynamic_pointer_cast<const PetCardData>(dta);
	if (!data || !data->isValid()) // Khoi tao thanh cong 1 empty card
		return true;

	if(!data->petImg.empty()) 
		petImg->loadTexture(data->petImg, ui::Widget::TextureResType::PLIST);
	else
		petImg->loadTexture(EMPTY_PET_IMG, ui::Widget::TextureResType::PLIST);

	if (!data->outerImg.empty()) 
		outerImg->loadTexture(data->outerImg, ui::Widget::TextureResType::PLIST);

	hpText->setString(to_string(data->hp));
	nameText->setString(data->name);

	bool evolved = data->evStage >= 1;
	if (evolved) {
		evText->setString("Stage" + to_string(data->evStage));
		evFromText->setString("Evolves from " + data->evolveFrom);
		evFromImg->loadTexture(data->preEvImg, ui::Widget::TextureResType::PLIST);
	}
	else {
		evText->setString("Basic");
	}
	evArrow->setVisible(evolved);
	return true;
}

std::shared_ptr<const CardData> PetCard::getData() {
	return data;
}

void PetCard::setNormalSize() {
	this->setScale(1.0f);
	cardNode->setScale(1.0f);
}

bool PetCard::isBasic() const {
	return data->evStage < 1;
}



/*
	Energy Card Class
*/

EnergyCard* EnergyCard::createWithData(const std::shared_ptr<const CardData> &data) {
	auto card = new (nothrow) EnergyCard();
	if (card && card->initWithData(data)) {
		card->autorelease();
		return card;
	}
	delete card;
	card = nullptr;
	return card;
}

EnergyCard* EnergyCard::create() {
	auto card = new (nothrow) EnergyCard();
	if (card && card->init()) {
		card->autorelease();
		return card;
	}
	delete card;
	card = nullptr;
	return card;
}


bool EnergyCard::init() {
	if (!BSCard::init())
		return false;
	auto pool = GM_POOL;

	cardNode = static_cast<ui::Layout*>(pool->tryGetNodeCsb("ccstd_csb/cards/energy_card.csb"));
	ui::Helper::doLayout(cardNode);

	if (!cardNode)
		return false;
	this->addChild(cardNode);
	auto layoutSize = this->getContentSize();

	cardNode->setScale(CARD_SCALE_DOWN); // *TODO: Nen tinh toan ra gia tri scale, sao cho kich thuong card bang dung voi CARD_SIZE
	cardNode->setPosition(layoutSize.width / 2, layoutSize.height / 2);

	image = static_cast<ui::ImageView*>(Utilize::getChildByName(cardNode, "Energy_Image"));

	
	nameText = static_cast<ui::Text*>(Utilize::getChildByName(cardNode, "Energy_Name"));
	nameText->setString("Energy");

	/*auto td = static_cast<ui::Layout*>(Utilize::getChildByName(cardNode, "Touch_Detector"));
	WidgetTouchNS::setWidgetTouchHold(td, bind(&PetCard::onTouchHold, this));
	td->setTouchEnabled(false);
	td->setSwallowTouches(false);*/

	return true;
}

bool EnergyCard::initWithData(const std::shared_ptr<const CardData> &dta) {
	if (!init())
		return false;

	data = dynamic_pointer_cast<const EnergyCardData>(dta);
	if (!data || !data->isValid()) // Khoi tao thanh cong 1 empty card
		return true;

	if (!data->image.empty())
		image->loadTexture(data->image, ui::Widget::TextureResType::PLIST);

}


std::shared_ptr<const CardData> EnergyCard::getData() {
	return data;
}

void EnergyCard::setNormalSize() {
	this->setScale(1.0f);
	cardNode->setScale(1.0f);
}

BATTLE_SCENE_NS_END