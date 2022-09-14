#include "BSPrizePile.h"

#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/LangSys.h"
#include "scenes/battle_scene_3/BattleScene.h"
#include "ui/UIHelper.h"

#include "scenes/battle_scene_3/BSCard.h"
#include "scenes/battle_scene_3/BSResources.h"

USING_NS_CC;
//BATTLE_SCENE_USE_NS;
using namespace std;

BATTLE_SCENE_NS_BEG

shared_ptr<BSPrizePile> BSPrizePile::create(BattleScene *scn, cocos2d::ui::Layout *layout) {
	auto ret = make_shared<BSPrizePile>(scn, layout);
	if (ret && ret->init())
		return ret;
	return nullptr;
}

BSPrizePile::BSPrizePile(BattleScene *scn, cocos2d::ui::Layout *layout) : btlScn(scn), root(layout) {
	cardVec.reserve(PRIZE_CARDS_NUM_PER_PLAYER);
	capacity = PRIZE_CARDS_NUM_PER_PLAYER;
}

BSPrizePile::~BSPrizePile(){}

bool BSPrizePile::init() {
	auto director = Director::getInstance();
	auto winSize = director->getWinSize();
	auto gm = GameManager::getInstance();
	auto resPool = gm->getPool();
	
	if (!btlScn || !root)
		return false;

	cardMarker = root->getChildByName("Card_Marker");
	numLabel = dynamic_cast<ui::Text*>(root->getChildByName("Content"));
	
	auto bsres = btlScn->getResources();
	for (unsigned int i = 0; i < capacity; ++i) {
		auto bsCard = bsres->getBSCard();
	}

	return true;
}

void BSPrizePile::drawFromDeck(const std::vector<CardId> &idvec) {
	auto &deck = btlScn->getDeck(pid);
	auto drawnVec = deck->drawTop(idVec);
}

//void BSHand::drawCards(size_t n, const vector<CardId> &idVec, bool hideCards) {
//	auto btlScn = BattleScene::getScn();
//	auto &deck = btlScn->getDeck(pid);
//	auto drawnVec = deck->drawTop(idVec);
//
//	size_t total = cards.size() + drawnVec.size(); // Tong so card sau khi draw
//	auto handSize = handLayout->getContentSize();
//
//	/// *Anchor Point cua card: (0.5, 0.5)
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
//	// * Sap xep nhung card da co tren tay truoc
//	size_t i = 0;
//	double d = cardW + cardSpace;
//	for (auto &card : cards) { // * Nhung card nay da duoc addChild
//		auto action = MoveTo::create(0.5f, Vec2(x + d * i, y));
//		card->runAction(action);
//		++i;
//	}
//
//	// * Duyet vong lap tren danh sach card duoc draw
//	auto startPos = Vec2(handSize.width + cardW + 30, handSize.height);
//	for (size_t index = 0; index < drawnVec.size(); ++index) { // * Nhung card nay chua duoc addChild
//		auto card = drawnVec[index];
//		handLayout->addChild(card);
//		card->setPosition(startPos);
//		card->setFlip(hideCards);
//
//		// Them hold touch
//		WidgetTouchNS::setWidgetTouchHold(card, [card, this, btlScn](WIDGET_TOUCH_HANDLER_PARAMS) {
//			auto data = card->getData();
//			btlScn->showCardDetail(data);
//			}, 0.3f);
//
//		// Them end Touch
//		WidgetTouchNS::setWidgetTouchEnded(card, [card, this, btlScn](WIDGET_TOUCH_HANDLER_PARAMS) {
//			btlScn->hideCardDetail();
//			});
//
//		card->setTouchEnabled(true);
//		card->setDragEnabled(false);
//		card->setSwallowTouches(true);
//
//		if (index == drawnVec.size() - 1) {
//			auto action = Sequence::create(MoveTo::create(0.8f + index * 0.2f, Vec2(x + d * i, y)), CallFunc::create([this]() {
//				dispatchEvent(EV_DRAW_ACTION_DONE);
//				}), nullptr);
//			card->runAction(action);
//		}
//		else {
//			auto action = MoveTo::create(0.8f + index * 0.2f, Vec2(x + d * i, y));
//			card->runAction(action);
//		}
//
//		++i;
//	}
//	cards.insert(cards.cend(), drawnVec.cbegin(), drawnVec.cend());
//
//}

BATTLE_SCENE_NS_END