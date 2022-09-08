#include "BSHand.h"
#include "BSCard.h"
#include "BSDeck.h"
//#include "BSBoard.h"
//#include "CardHolder.h"
#include "data/CardData.h"

#include "BattleScene.h"
#include "GameManager.h"
#include "common/ResourcePool.h"
#include "common/Utilize.h"
#include "components/WidgetTouchComponent.h"
#include "common/Utilize.h"
#include "ui/UIHelper.h"

using namespace std;
USING_NS_CC;
BATTLE_SCENE_NS_BEG

BSHand::HandPtr BSHand::createShPtr(ui::Layout *layout, const PlayerIdType &id) {
	auto ret = make_shared<BSHand>(layout, id);
	if (ret && ret->init())
		return ret;
	return nullptr;
}

BSHand::BSHand(cocos2d::ui::Layout *layout, PlayerIdType id) : handLayout(layout), pid(id){
	CCLOG("BSHand::Ctor %p", this);
}

BSHand::~BSHand() {
	CCLOG("BSHand::Dtor %p", this);
}

bool BSHand::init() {
	if (!handLayout)
		return false;

	auto dir = Director::getInstance();
	auto visibleSize = dir->getVisibleSize();

	constexpr float CARD_HEIGHT = 142.0f;
	handLayout->setContentSize(Size(visibleSize.width, CARD_HEIGHT));
	handLayout->setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	handLayout->setBackGroundColor(Color3B(150, 200, 255));
	handLayout->setBackGroundColorOpacity(100);
	handLayout->setAnchorPoint(Vec2(0, 0));
	return true;
}

void BSHand::drawCards(size_t n, const vector<CardId> &idVec, bool hideCards) {
	auto btlScn = BattleScene::getScn();
	auto &deck = btlScn->getDeck(pid);
	auto drawnVec = deck->drawTop(idVec);

	size_t total = cards.size() + drawnVec.size(); // Tong so card sau khi draw
	auto handSize = handLayout->getContentSize();

	/// *Anchor Point cua card: (0.5, 0.5)

	auto cardW = BSCard::CARD_SIZE.width;
	float cardSpace = 5.0f; // Khoang cach giua nhung la bai
	const float minCardSpace = -cardW / 2; // cardSpace >= -cardW / 2 (Neu cardSpace < 0 thi 2 card se dinh vao nhau)
	auto totalW = total * cardW + (total - 1) * cardSpace; // Tong width cua tat ca card tren hand

	if (totalW > handSize.width)  // Neu vuot qua hand width thi giam cardSpace
	{
		totalW = handSize.width;
		cardSpace = (totalW - total * cardW) / (total - 1);
	}

	auto x = (handSize.width - totalW) / 2.0 + cardW / 2.0;
	decltype(x) y = handSize.height / 2;

	// * Sap xep nhung card da co tren tay truoc
	size_t i = 0;
	double d = cardW + cardSpace;
	for (auto &card : cards) { // * Nhung card nay da duoc addChild
		auto action = MoveTo::create(0.5f, Vec2(x + d * i, y));
		card->runAction(action);
		++i;
	}

	//auto board = btlScn->getBoard(pid);
	//vector<Node*> destVec;
	//destVec.push_back(board->activeHolder);
	//destVec.insert(destVec.cend(), board->benchHolders.cbegin(), board->benchHolders.cend());

	// * Duyet vong lap tren danh sach card duoc draw
	auto startPos = Vec2(handSize.width + cardW + 30, handSize.height);
	for (size_t index = 0; index < drawnVec.size(); ++index) { // * Nhung card nay chua duoc addChild
		auto card = drawnVec[index];
		handLayout->addChild(card);
		card->setPosition(startPos);
		card->setFlip(hideCards);

		// Them hold touch
		WidgetTouchNS::setWidgetTouchHold(card, [card, this, btlScn](WIDGET_TOUCH_HANDLER_PARAMS) {
			auto data = card->getData();
			btlScn->showCardDetail(data);
			}, 0.3f);

		// Them end Touch
		WidgetTouchNS::setWidgetTouchEnded(card, [card, this, btlScn](WIDGET_TOUCH_HANDLER_PARAMS) {
			btlScn->hideCardDetail();
			});

		card->setTouchEnabled(true);
		card->setDragEnabled(false);
		card->setSwallowTouches(true);

		if (index == drawnVec.size() - 1) {
			auto action = Sequence::create(MoveTo::create(0.8f + index * 0.2f, Vec2(x + d * i, y)), CallFunc::create([this]() {
				dispatchEvent(EV_DRAW_ACTION_DONE);
				}), nullptr);
			card->runAction(action);
		}
		else {
			auto action = MoveTo::create(0.8f + index * 0.2f, Vec2(x + d * i, y));
			card->runAction(action);
		}

		++i;
	}
	cards.insert(cards.cend(), drawnVec.cbegin(), drawnVec.cend());
}

const string BSHand::EV_DRAW_ACTION_DONE = "EV_DRAW_ACTION_DONE";


void BSHand::drawCards(size_t n, std::function<void()> onDrawDone) {
	
	//auto &deck = btlScn->decks[ownerId];
	//auto drawnVec = deck->drawTop(n);
	//
	//size_t total = cards.size() + drawnVec.size(); // Tong so card sau khi draw
	//auto handSize.width = this->getContentSize().width; // Chieu dai cua hand
	//auto handSize.height = this->getContentSize().height; // Chieu cao cua hand

	//// *Anchor Point cua card: (0.5, 0.5)

	//auto cardW = BSCard::CARD_SIZE.width;
	//float cardSpace = 5.0f; // Khoang cach giua nhung la bai
	//const float minCardSpace = -cardW / 2; // cardSpace >= -cardW / 2 (Neu cardSpace < 0 thi 2 card se dinh vao nhau)
	//auto totalW = total * cardW + (total - 1) * cardSpace; // Tong width cua tat ca card tren hand
	//
	//if (totalW > handSize.width)  // Neu vuot qua hand width thi giam cardSpace
	//{
	//	totalW = handSize.width;
	//	cardSpace = (totalW - total * cardW) / (total - 1);
	//}
	//
	//auto x = (handSize.width - totalW) / 2.0 + cardW / 2.0;
	//decltype(x) y = handSize.height / 2;

	//size_t i = 0;
	//double d = cardW + cardSpace;
	//for (auto &card : cards) { // * Nhung card nay da duoc addChild
	//	auto action = MoveTo::create(0.5f, Vec2(x + d * i, y));
	//	card->runAction(action);
	//	++i;
	//}

	//auto board = btlScn->boards[ownerId];
	//vector<Node*> destVec;
	//destVec.push_back(board->activeHolder);
	//destVec.insert(destVec.cend(), board->benchHolders.cbegin(), board->benchHolders.cend());

	//// * Duyet vong lap tren danh sach card duoc draw
	//auto startPos = Vec2(handSize.width + cardW + 30, handSize.height);
	//for (size_t index = 0; index < drawnVec.size(); ++index) { // * Nhung card nay chua duoc addChild
	//	auto card = drawnVec[index];
	//	this->addChild(card);
	//	card->setPosition(startPos);

	//	// Them hold touch
	//	WidgetTouchNS::setWidgetTouchHold(card, [card, this](WIDGET_TOUCH_HANDLER_PARAMS) {
	//		btlScn->showCardDetails(card->getData());
	//		}, 0.3f);

	//	// Them end Touch
	//	WidgetTouchNS::setWidgetTouchEnded(card, [card, this](WIDGET_TOUCH_HANDLER_PARAMS) {
	//		btlScn->hideCardDetails();
	//		});

	//	// Them Drag component
	//	card->setDragHandler(destVec,
	//		[this](Node * cardNode, Node *dest) { // Drag Begin Callback
	//			btlScn->hideCardDetails();
	//		},
	//		[this](Node *cardNode, Node *dest) { // Drag End Callback
	//			if (!dest) {
	//				auto comp = DragComponent::getComp(cardNode);
	//				auto card = dynamic_cast<BSCard*>(cardNode);
	//				onDragBack(card);
	//			}
	//			else {
	//				auto card = dynamic_cast<BSCard*>(cardNode);
	//				bool suc = onDragEnd(card, dest);
	//				if (!suc)
	//					onDragBack(card);
	//				else
	//					onDragSucceeded();
	//			}
	//		}
	//		);
	//	card->setTouchEnabled(true);
	//	card->setSwallowTouches(true);
	//	//--

	//	if (index == drawnVec.size() - 1) {
	//		auto action = Sequence::create(MoveTo::create(0.8f + index * 0.2f, Vec2(x + d * i, y)), CallFunc::create(onDrawDone), nullptr);
	//		card->runAction(action);
	//	}
	//	else {
	//		auto action = MoveTo::create(0.8f + index * 0.2f, Vec2(x + d * i, y));
	//		card->runAction(action);
	//	}
	//
	//	++i;
	//}
	//cards.insert(cards.cend(), drawnVec.cbegin(), drawnVec.cend());
}

//void BSHand::onDragBack(BSCard *cardNode) {
//	auto comp = MyComponentNS::getComponent<DragComponent>(cardNode, COMPONENT_KEY::DRAG);
//	if (comp) {
//		auto cardWidget = dynamic_cast<ui::Widget*>(cardNode);
//		auto wOldPos = comp->getOrgWorldPos();
//		auto orgParent = comp->getOrgParent();
//		cardWidget->removeFromParent();
//		orgParent->addChild(cardWidget);
//		auto lOldPos = orgParent->convertToNodeSpaceAR(wOldPos);
//		cardWidget->setZOrder(comp->getOrgZ());
//		cardWidget->setTouchEnabled(false);
//		cardWidget->runAction(
//			Sequence::create(MoveTo::create(0.2f, lOldPos),
//				CallFunc::create([cardWidget]() {cardWidget->setTouchEnabled(true); }),
//				nullptr));
//	}
//}
//
//bool BSHand::onDragEnd(BSCard *card, cocos2d::Node *dest) {
//	auto holder = dynamic_cast<CardHolder*>(dest);
//	auto data = card->getData();
//	bool suc = false;
//	switch (data->type) {
//	case CardData::Type::Pet: 
//	{
//		auto petCard = dynamic_cast<PetCard*>(card);
//		//bool suc = holder->tryAddPetCard(petCard);
//		suc = btlScn->getBattleManager()->playerTryPlayPetCard(ownerId, petCard, holder);
//	}
//		break;
//	case CardData::Type::Energy:
//	{
//		auto energyCard = dynamic_cast<EnergyCard*>(card);
//		//bool suc = holder->tryAddEnergyCard(energyCard);
//		suc = btlScn->getBattleManager()->playerTryPlayEnergyCard(ownerId, energyCard, holder);
//	}
//		break;
//	}
//
//	if (suc) {
//		Utilize::mvector::removeElement(cards, card);
//		updateCardPositions();
//	}
//
//	return suc;
//}
//
//void BSHand::updateCardPositions() {
//	size_t total = cards.size(); // Tong so card hien tai
//	auto handSize.width = this->getContentSize().width; // Chieu dai cua hand
//	auto handSize.height = this->getContentSize().height; // Chieu cao cua hand
//
//	// *Anchor Point cua card: (0.5, 0.5)
//
//	auto cardW = BSCard::CARD_SIZE.width;
//	float cardSpace = 5.0f; // Khoang cach giua nhung la bai
//	const float minCardSpace = -cardW / 2; // cardSpace >= -cardW / 2 (Neu cardSpace < 0 thi 2 card se dinh vao nhau)
//	auto totalW = total * cardW + (total - 1) * cardSpace; // Tong width cua tat ca card tren hand
//
//	if (totalW > handSize.width)  // Neu vuot qua hand width thi giam cardSpace
//	{
//		totalW = handSize.width;
//		cardSpace = (totalW - total * cardW) / (total - 1);
//	}
//
//	auto x = (handSize.width - totalW) / 2.0 + cardW / 2.0;
//	decltype(x) y = handSize.height / 2;
//
//	size_t i = 0;
//	double d = cardW + cardSpace;
//	for (auto &card : cards) { // * Nhung card nay da duoc addChild
//		auto action = MoveTo::create(0.3f, Vec2(x + d * i, y));
//		card->runAction(action);
//		++i;
//	}
//}
//
//void BSHand::onDragSucceeded() {
//
//}

void BSHand::setEnableDragSetupActive(bool enabled) {
	auto btlScn = BattleScene::getScene();
	//auto baord = btlScn->getBoa
	for (auto &card : cards) {
		//auto board = 

		//// Them Drag component
		//card->setDragHandler(destVec,
		//	[this](Node * cardNode, Node *dest) { // Drag Begin Callback
		//		btlScn->hideCardDetails();
		//	},
		//	[this](Node *cardNode, Node *dest) { // Drag End Callback
		//		if (!dest) {
		//			auto comp = DragComponent::getComp(cardNode);
		//			auto card = dynamic_cast<BSCard*>(cardNode);
		//			onDragBack(card);
		//		}
		//		else {
		//			auto card = dynamic_cast<BSCard*>(cardNode);
		//			bool suc = onDragEnd(card, dest);
		//			if (!suc)
		//				onDragBack(card);
		//			else
		//				onDragSucceeded();
		//		}
		//	}
		//	);
	}
	
}

void BSHand::disableDragAll() {

}

BATTLE_SCENE_NS_END