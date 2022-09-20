#include "BSDiscard.h"
#include "BSDeck.h"
#include "common/Utilize.h"
#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/LangSys.h"
#include "scenes/battle_scene_3/BattleScene.h"
#include "ui/UIHelper.h"
#include "CardHolder.h"

#include "scenes/battle_scene_3/BSCard.h"
#include "scenes/battle_scene_3/BSResources.h"

USING_NS_CC;
//BATTLE_SCENE_USE_NS;
using namespace std;

BATTLE_SCENE_NS_BEG

const float BSDiscardPile::CardScale = 0.25;

shared_ptr<BSDiscardPile> BSDiscardPile::create(BattleScene *scn, const PlayerIdType &id) {
	auto ret = make_shared<BSDiscardPile>(scn, id);
	if (ret && ret->init())
		return ret;
	return nullptr;
}

BSDiscardPile::BSDiscardPile(BattleScene *scn, const PlayerIdType &id) : btlScn(scn), pid(id) {
}

BSDiscardPile::~BSDiscardPile(){}

bool BSDiscardPile::init() {
	auto director = Director::getInstance();
	auto winSize = director->getWinSize();
	auto gm = GameManager::getInstance();
	auto resPool = gm->getPool();
	
	if (!btlScn)
		return false;

	auto scnroot = btlScn->getScnRoot();
	string prefix = pid == PLAYER ? "P1" : "P2";
	pPanel = dynamic_cast<ui::Layout*>(scnroot->getChildByName(prefix + "_Panel")); // P1_Panel or P2_Panel
	if (!pPanel)
		return false;

	discardBoard = dynamic_cast<ui::Layout*>(pPanel->getChildByName("Discard_Board"));

	cardMarker = discardBoard->getChildByName("Card_Marker");
	cardMarker->setPosition(cardMarker->getPosition() + Vec2(-3, 0));
	cardMarker->setScale(CardScale);

	numLabel = dynamic_cast<ui::Text*>(discardBoard->getChildByName("Content"));
	
	return true;
}

const string BSDiscardPile::EV_DRAW_ACTION_DONE = "EV_DRAW_ACTION_DONE";

//void BSDiscardPile::drawFromDeck(const std::vector<CardId> &idvec) {
//	auto &deck = btlScn->getDeck(pid);
//	auto btlScn = BattleScene::getScn();
//	auto drawnVec = deck->drawTop(idvec);
//
//	auto destPos = Vec2(0, 0);
//	auto startPos = destPos +  Vec2(0, 120);
//
//	for (size_t index = 0; index < cardVec.size(); ++index) { // * Nhung card nay chua duoc addChild
//		if (index >= drawnVec.size())
//			continue;
//
//		auto card = drawnVec[index];
//		cardMarker->addChild(card);
//
//		card->setPosition(startPos);
//		card->setFlip(true);
//
//		card->setTouchEnabled(false);
//		card->setDragEnabled(false);
//		card->setSwallowTouches(true);
//
//		if (index == drawnVec.size() - 1) {
//			auto action = Sequence::create(
//				MoveTo::create(0.8f + index * 0.2f, destPos), 
//				CallFunc::create([this]() {
//				dispatchEvent(EV_DRAW_ACTION_DONE);
//				}), 
//				nullptr);
//			card->runAction(action);
//		}
//		else {
//			auto action = MoveTo::create(0.5f + index * 0.2f, destPos);
//			card->runAction(action);
//		}
//		cardVec[index] = card;
//		++curCount;
//	}
//	//cardVec.insert(cardVec.cend(), drawnVec.cbegin(), drawnVec.cend());
//	numLabel->setString(to_string(curCount));
//	numLabel->setVisible(curCount > 0);
//}

//vector<BSCard*> BSDiscardPile::draw(const vector<unsigned int> &idxVec) {
//	vector<BSCard*> ret;
//
//	for (const auto &idx : idxVec) {
//		if (0 <= idx && idx < cardVec.size() && cardVec[idx]) {
//			ret.push_back(cardVec[idx]);
//
//			cardVec[idx]->stopAllActions();
//			cardVec[idx]->removeFromParent();
//			cardVec[idx] = nullptr;
//			
//			if (curCount > 0)
//				curCount -= 1;
//		}
//	}
//
//	numLabel->setString(to_string(curCount));
//	numLabel->setVisible(curCount > 0);
//	return ret;
//}


void BSDiscardPile::pushCardFromHolder(BSCard *card, CardHolder *holder, float delay,const std::function<void()> &onDone) {
	// Tra ve pile
	cocos2d::Node *fromNode;
	auto startPos = Utilize::mnode::getLocalPos(holder, cardMarker);
	card->setPosition(startPos);

	auto destPos = Vec2(0, 0);
	card->runAction(MoveTo::create(delay, destPos));

	cardMarker->addChild(card);
	cardVec.push_back(card);
}

void BSDiscardPile::pushCardsFromHolder(const std::vector<BSCard*> &addedVec, CardHolder *holder,  const std::function<void()> &onDone) {
	unsigned int i = 0;
	float delay = 0.5f;
	for (const auto card : addedVec) {
		// Tra ve pile
		cocos2d::Node *fromNode;
		auto startPos = Utilize::mnode::getLocalPos(holder, cardMarker);
		card->setPosition(startPos);

		auto destPos = Vec2(0, 0);

		if (i == addedVec.size() - 1) {
			card->runAction(
				Sequence::create(
					MoveTo::create(delay * i, destPos),
					CallFunc::create([onDone](){
						if (onDone)
							onDone();
					}),
					nullptr)
				
			);
		}
		else
			card->runAction(MoveTo::create(delay * i, destPos));

		cardMarker->addChild(card);
		cardVec.push_back(card);
		++i;
	}
}
BATTLE_SCENE_NS_END