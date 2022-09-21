#include "BSCard.h"
#include "data/CardData.h"
#include "data/MoveData.h"
#include "GameManager.h"
#include "common/ResourcePool.h"
#include "common/Utilize.h"
#include "components/WidgetTouchComponent.h"
#include "ui/UIHelper.h"
#include "prefabs/BSPrefabs.h"

using namespace std;
USING_NS_CC;
BS_PREFABS_USE_NS;
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
	setContentSize(ORIGIN_CARD_SIZE);
	setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	setBackGroundColorOpacity(50);
	setBackGroundColor(Color3B::RED);

	
	return true;
}

bool BSCard::onTouchHold() {
	CCLOG("Hold");
	return true;
}

void BSCard::setDragHandler(const std::vector<cocos2d::Node*> &destNodes,
	const DragHandler &begF,
	const DragHandler &endF,
	const DragHandler &inF,
	const DragHandler &outF) {

	dragBeg = begF;
	dragEnd = endF;
	dragIn = inF;
	dragOut = outF;

	auto comp = DragComponent::getComp(this);
	if (!comp) { // Them Drag component
		comp = new DragComponent();
		DragComponent::setComp(this, comp);
	}

	comp->destinations.clear();
	comp->destinations.insert(comp->destinations.cend(), destNodes.begin(), destNodes.end());

	comp->dragBeginCallback = [this](Node * cardNode, Node *dest) {
		auto comp = DragComponent::getComp(this);
		comp->setDragContainer(this->getParent());
		if (dragBeg)
			dragBeg(cardNode, dest);
	};

	comp->dragInCallback = [this](Node * cardNode, Node *dest) {
		if (dragIn)
			dragIn(cardNode, dest);
	};

	comp->dragOutCallback = [this](Node * cardNode, Node *dest) {
		if (dragOut)
			dragOut(cardNode, dest);
	};

	comp->dragEndCallback = [this](Node *cardNode, Node *dest) {
		if (dragEnd)
			dragEnd(cardNode, dest);
	};

	this->setTouchEnabled(true);
	this->setSwallowTouches(true);
}

void BSCard::setDragEnabled(bool enabled) {
	WidgetTouchNS::setEnableDragComponent(this, enabled);
}




/*
	PetCard Class
*/

PetCard::~PetCard() {
	returnToPool();
}

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

void PetCard::returnToPool() {
	if (weakEnergyMarker) {
		weakEnergyMarker->removeAllChildren();
	}

	if (resistEnergyMarker) {
		resistEnergyMarker->removeAllChildren();
	}

	if (retreatEnergyMarker) {
		retreatEnergyMarker->removeAllChildren();
	}

	if (moveListView) {
		moveListView->removeAllChildren();
	}

	if (cardNode) {
		cardNode->removeFromParent();
		PoolVector::returnNode(cardNode);
		cardNode = nullptr;
	}
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
	cardNode->setScale(1.0f); // *TODO: Nen tinh toan ra gia tri scale, sao cho kich thuong card bang dung voi CARD_SIZE
	cardNode->setPosition(layoutSize.width / 2, layoutSize.height / 2);

	cardLayout = dynamic_cast<ui::Layout*>(cardNode->getChildByName("Card_Layout"));
	petImg = dynamic_cast<ui::ImageView*>(cardLayout->getChildByName("Pokemon_Image"));
	outerImg = dynamic_cast<ui::ImageView*>(cardLayout->getChildByName("Outer_Background"));


	nameText = static_cast<ui::Text*>(Utilize::getChildByName(cardNode, "Pokemon_Name"));
	nameText->setString("");
	hpText = static_cast<ui::Text*>(Utilize::getChildByName(cardNode, "Hp_Value_Text"));
	hpText->setString("0");

	evSquare = dynamic_cast<ui::ImageView*>(Utilize::getChildByName(cardNode, "Evo_Square"));
	evText = static_cast<ui::Text*>(Utilize::getChildByName(cardNode, "Evo_Text"));
	evArrow = static_cast<ui::ImageView*>(Utilize::getChildByName(cardNode, "Evo_Arrow"));
	evFromText = static_cast<ui::Text*>(Utilize::getChildByName(cardNode, "Pre_Evo_Name"));
	//evFromImg = static_cast<ui::ImageView*>(Utilize::getChildByName(cardNode, "Evo_Img"));
	evFromImg = static_cast<ui::ImageView*>(cardLayout->getChildByName("Evo_Square")->getChildByName("Evo_Img"));

	cardBack = dynamic_cast<ui::Layout*>(cardNode->getChildByName("Card_Back"));
	cardBack->setVisible(!flippedDown);

	/*auto td = static_cast<ui::Layout*>(Utilize::getChildByName(cardNode, "Touch_Detector"));
	WidgetTouchNS::setWidgetTouchHold(td, bind(&PetCard::onTouchHold, this));
	td->setTouchEnabled(false);
	td->setSwallowTouches(false);*/

	bottomPanel = dynamic_cast<ui::Layout*>(cardLayout->getChildByName("Bottom_Panel"));
	
	weakPanel = dynamic_cast<ui::ImageView*>(bottomPanel->getChildByName("Weakness_Panel"));
	weakEnergyMarker = weakPanel->getChildByName("EnergyMarker");
	
	resistPanel = dynamic_cast<ui::ImageView*>(bottomPanel->getChildByName("Resistance_Panel"));
	resistEnergyMarker = resistPanel->getChildByName("EnergyMarker");

	retreatPanel = dynamic_cast<ui::ImageView*>(bottomPanel->getChildByName("Retreat_Panel"));
	retreatEnergyMarker = retreatPanel->getChildByName("EnergyMarker");


	moveListView = dynamic_cast<ui::ListView*>(cardLayout->getChildByName("ListView"));
	
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
	evSquare->setVisible(evolved);
	evFromText->setVisible(evolved);
	// Retreat Panel
	auto &retreatMap = data->retreatMap;
	for (const auto itr : retreatMap) {
		auto eType = itr.first;
		auto eNum = itr.second;
		auto eItem = EnergyItem::create(eType, eNum);
		retreatEnergyMarker->addChild(eItem);
	}
	updateRetreatEnergySpritePanel();

	// Weakness Panel
	auto &weakSet = data->weakSet;
	if (!weakSet.empty()) {
		auto eType = *(weakSet.cbegin());

		auto eItem = EnergyItem::create(eType, 1);
		weakEnergyMarker->addChild(eItem);

		auto text = dynamic_cast<ui::Text*>(weakPanel->getChildByName("Weakness_Text_1"));
		text->setString("x2");
	}
	else {
		weakEnergyMarker->setVisible(false);
		auto text = dynamic_cast<ui::Text*>(weakPanel->getChildByName("Weakness_Text_1"));
		text->setVisible(false);
	}

	// Resist Panel
	auto &resistMap = data->resistanceMap;
	if (!resistMap.empty()) {
		auto eType = resistMap.cbegin()->first;
		auto eNum = resistMap.cbegin()->second;

		auto eItem = EnergyItem::create(eType, 1);
		resistEnergyMarker->addChild(eItem);

		auto text = dynamic_cast<ui::Text*>(resistPanel->getChildByName("Resistance_Text_1"));
		text->setString(to_string(eNum));
	}
	else {
		resistEnergyMarker->setVisible(false);
		auto text = dynamic_cast<ui::Text*>(resistPanel->getChildByName("Resistance_Text_1"));
		text->setVisible(false);
	}

	// Moves
	auto moveVec = data->moveVec;
	for (const auto &moveData : moveVec) {
		auto moveItem = PetCardMove::create(moveData);
		moveListView->addChild(moveItem);
	}

	return true;
}

void PetCard::updateRetreatEnergySpritePanel() {
	auto children = retreatEnergyMarker->getChildren();
	unsigned int i = 0;
	constexpr float spaceX = 10.0f;
	for (const auto &child : children) {
		auto eChild = dynamic_cast<EnergyItem*>(child);
		auto eSize = eChild->getEnergySize();
		child->setPositionX((eSize.width / 2 + spaceX) * i);
		child->setPositionY(0);
		++i;
	}
}

std::shared_ptr<const CardData> PetCard::getData() const {
	return data;
}

void PetCard::setNormalSize() {
	this->setScale(1.0f);
	cardNode->setScale(1.0f);
}

bool PetCard::isBasic() const {
	return data->evStage < 1;
}

void PetCard::setFlip(bool flip, bool anim) {
	auto prevFlipped = flippedDown;
	flippedDown = flip;
	flipTime = 0.5f;
	if (anim) {
		if (prevFlipped && flip) {
			cardBack->setVisible(true);
			cardLayout->setVisible(false);

			auto orgScale = cardNode->getScale();
			cardNode->setScale(orgScale * 0.7f);
			cardNode->runAction(Sequence::create(
				ScaleTo::create(0.5f, 0.0f, orgScale),
				CallFunc::create([this](){
					cardBack->setVisible(false);
					cardLayout->setVisible(true);
					}),
				ScaleTo::create(0.5f, orgScale),
				nullptr));
		}
		else if (!prevFlipped && !flip) {
			cardBack->setVisible(false);
			cardLayout->setVisible(true);

			auto orgScale = cardNode->getScale();
			cardNode->setScale(orgScale * 0.7f);
			cardNode->runAction(Sequence::create(
				ScaleTo::create(0.5f, 0.0f, orgScale),
				CallFunc::create([this]() {
					cardBack->setVisible(true);
					cardLayout->setVisible(false);
					}),
				ScaleTo::create(0.5f, orgScale),
						nullptr));
		}
	}
	else {
	
		cardBack->setVisible(flip);
	}
}


PetCardMove* PetCardMove::create(const shared_ptr<const MoveData> &data) {
	auto ret = new (nothrow) PetCardMove(data);
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	ret = nullptr;
	return ret;
}


PetCardMove::PetCardMove(const shared_ptr<const MoveData> &dta) : data(dta) {}

PetCardMove::~PetCardMove() {
	cleanUI();
}

bool PetCardMove::init() {
	root = GM_POOL->tryGetNodeCsb("ccstd_csb/battle_scene/prefabs/CardMoveInfo.csb");
	if (!root)
		return false;

	container = dynamic_cast<ui::Layout*>(root->getChildByName("Container"));
	moveNameLb = dynamic_cast<ui::Text*>(container->getChildByName("Name"));
	dmgLb = dynamic_cast<ui::Text*>(container->getChildByName("DmgText"));
	energyPanel = dynamic_cast<ui::Layout*>(container->getChildByName("EnergyPanel"));
	desBoard = dynamic_cast<ui::Layout*>(container->getChildByName("Move_Board"));

	auto moveName = data->name;
	moveNameLb->setString(moveName);
	auto dmgText = data->damageText;
	dmgLb->setString(dmgText);

	setMoveDes(data->content);

	this->setContentSize(root->getContentSize());
	this->addChild(root);

	auto &costMap = data->costMap;
	for (const auto &itr : costMap) {
		auto eType = itr.first;
		auto eNum = itr.second;
		auto eItem = EnergyItem::create(eType, eNum);
		energyPanel->addChild(eItem);
	}
	updateEnergyPanel();
	return true;
}

void PetCardMove::cleanUI() {
	energyPanel->removeAllChildren();
	if (root) {
		if (root->getReferenceCount() > 1)
		{
			root->removeFromParent(); // Release
			PoolVector::returnNode(root);
		}
		root = nullptr;
	}
}

void PetCardMove::setMoveDes(const std::string &s) {
	auto desText = dynamic_cast<ui::Text*>(desBoard->getChildByName("Text"));
	desText->setString(s);
	//TODO
}

void PetCardMove::updateEnergyPanel() {
	auto children = energyPanel->getChildren();
	unsigned int i = 0;
	constexpr float spaceX = 10.0f;
	for (const auto &child : children) {
		auto eChild = dynamic_cast<EnergyItem*>(child);
		auto eSize = eChild->getEnergySize();
		child->setPositionX((eSize.width / 2 + spaceX) * i);
		child->setPositionY(0);
		++i;
	}
}


/*
	Energy Card Class
*/

EnergyCard::~EnergyCard() {
	returnToPool();
}


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

void EnergyCard::returnToPool() {
	if (cardNode) {
		cardNode->removeFromParent();
		PoolVector::returnNode(cardNode);
		cardNode = nullptr;
	}

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

	cardNode->setScale(1.0f); // *TODO: Nen tinh toan ra gia tri scale, sao cho kich thuong card bang dung voi CARD_SIZE
	cardNode->setPosition(layoutSize.width / 2, layoutSize.height / 2);

	image = static_cast<ui::ImageView*>(Utilize::getChildByName(cardNode, "Energy_Image"));

	
	nameText = static_cast<ui::Text*>(Utilize::getChildByName(cardNode, "Energy_Name"));
	nameText->setString("Energy");

	cardBack = dynamic_cast<ui::Layout*>(cardNode->getChildByName("Card_Back"));
	cardBack->setVisible(!flippedDown);
	
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

std::shared_ptr<const CardData> EnergyCard::getData() const {
	return data;
}

void EnergyCard::setNormalSize() {
	this->setScale(1.0f);
	cardNode->setScale(1.0f);
}

void EnergyCard::setFlip(bool flip, bool anim ) {
	flippedDown = flip;
	flipTime = 0.5f;
	if (anim) {
		//TODO
	}
	else {
		cardBack->setVisible(flip);
	}
}
BATTLE_SCENE_NS_END