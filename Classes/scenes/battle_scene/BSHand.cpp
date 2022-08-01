#include "BSHand.h"
#include "BSCard.h"
#include "BattleScene.h"
#include "GameManager.h"
#include "BSDeck.h"
#include "common/ResourcePool.h"
#include "common/Utilize.h"
#include "components/WidgetTouchComponent.h"
#include "ui/UIHelper.h"

using namespace std;
USING_NS_CC;
BATTLE_SCENE_NS_BEG

BSHand* BSHand::create(BattleScene *scn, PlayerIdType id) {
	auto hand = new (nothrow) BSHand(scn, id);
	if (hand && hand->init()) {
		hand->autorelease();
		return hand;
	}
	delete hand;
	hand = nullptr;
	return hand;
}

BSHand::BSHand(BattleScene *scn, PlayerIdType id) : btlScn(scn), ownerId(id){
}

BSHand::~BSHand() {

}

bool BSHand::init() {
	if (!ui::Layout::init())
		return false;

	auto dir = Director::getInstance();
	auto visibleSize = dir->getVisibleSize();

	setContentSize(Size(visibleSize.width, BSCard::CARD_SIZE.height));
	setBackGroundColorType(ui::Layout::BackGroundColorType::SOLID);
	setBackGroundColor(Color3B(150, 200, 255));
	setBackGroundColorOpacity(100);
	setAnchorPoint(Vec2(0, 0));
	setPosition(Vec2(0, -50));
	//btlScn->addChild(this);

	return true;
}

void BSHand::drawCards(size_t n, std::function<void()> onDrawDone) {
	auto &deck = btlScn->decks[ownerId];
	auto drawnVec = deck->drawTop(n);
	
	size_t total = cards.size() + drawnVec.size(); // Tong so card sau khi draw
	auto handWidth = this->getContentSize().width; // Chieu dai cua hand
	auto handHeight = this->getContentSize().height; // Chieu cao cua hand

	// *Anchor Point cua card: (0.5, 0.5)

	auto cardW = BSCard::CARD_SIZE.width;
	float cardSpace = 5.0f; // Khoang cach giua nhung la bai
	const float minCardSpace = -cardW / 2; // cardSpace >= -cardW / 2 (Neu cardSpace < 0 thi 2 card se dinh vao nhau)
	auto totalW = total * cardW + (total - 1) * cardSpace; // Tong width cua tat ca card tren hand
	
	if (totalW > handWidth)  // Neu vuot qua hand width thi giam cardSpace
	{
		totalW = handWidth;
		cardSpace = (totalW - total * cardW) / (total - 1);
	}
	
	auto x = (handWidth - totalW) / 2.0 + cardW / 2.0;
	decltype(x) y = handHeight / 2;

	size_t i = 0;
	double d = cardW + cardSpace;
	for (auto &card : cards) { // * Nhung card nay da duoc addChild
		auto action = MoveTo::create(0.5f, Vec2(x + d * i, y));
		card->runAction(action);
		++i;
	}

	// * Duyet vong lap tren danh sach card duoc draw
	auto startPos = Vec2(handWidth + cardW + 30, handHeight);
	for (size_t index = 0; index < drawnVec.size(); ++index) { // * Nhung card nay chua duoc addChild
		auto card = drawnVec[index];
		this->addChild(card);
		card->setPosition(startPos);

		// Them Drag component
		auto dragComp = new DragComponent();
		dragComp->useCenter = true;
		WidgetTouchNS::setDragComponent(card, dragComp);
		//--

		if (index == drawnVec.size() - 1) {
			auto action = Sequence::create(MoveTo::create(0.8f + index * 0.2f, Vec2(x + d * i, y)), CallFunc::create(onDrawDone), nullptr);
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

BATTLE_SCENE_NS_END