#include "BSPrizePile.h"
#include "BSDeck.h"

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

const float BSPrizePile::CardScale = 0.25;

shared_ptr<BSPrizePile> BSPrizePile::create(BattleScene *scn, const PlayerIdType &id) {
	auto ret = make_shared<BSPrizePile>(scn, id);
	if (ret && ret->init())
		return ret;
	return nullptr;
}

BSPrizePile::BSPrizePile(BattleScene *scn, const PlayerIdType &id) : btlScn(scn), pid(id) {
	capacity = GConfig::PRIZE_CARDS_NUM_PER_PLAYER;
}

BSPrizePile::~BSPrizePile(){}

bool BSPrizePile::init() {
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

	prizeBoard = dynamic_cast<ui::Layout*>(pPanel->getChildByName("Prize_Board"));

	cardMarker = prizeBoard->getChildByName("Card_Marker");
	cardMarker->setPosition(cardMarker->getPosition() + Vec2(-3, 0));
	cardMarker->setScale(CardScale);

	numLabel = dynamic_cast<ui::Text*>(prizeBoard->getChildByName("Content"));
	
	return true;
}

const string BSPrizePile::EV_DRAW_ACTION_DONE = "EV_DRAW_ACTION_DONE";

void BSPrizePile::drawFromDeck(const std::vector<CardId> &idvec) {
	auto &deck = btlScn->getDeck(pid);
	auto btlScn = BattleScene::getScn();
	auto drawnVec = deck->drawTop(idvec);

	auto destPos = Vec2(0, 0);
	auto startPos = destPos +  Vec2(0, 120);

	for (size_t index = 0; index < cardVec.size(); ++index) { // * Nhung card nay chua duoc addChild
		if (index >= drawnVec.size())
			continue;

		auto card = drawnVec[index];
		cardMarker->addChild(card);

		card->setPosition(startPos);
		card->setFlip(true);

		card->setTouchEnabled(false);
		card->setDragEnabled(false);
		card->setSwallowTouches(true);

		if (index == drawnVec.size() - 1) {
			auto action = Sequence::create(
				MoveTo::create(0.8f + index * 0.2f, destPos), 
				CallFunc::create([this]() {
				dispatchEvent(EV_DRAW_ACTION_DONE);
				}), 
				nullptr);
			card->runAction(action);
		}
		else {
			auto action = MoveTo::create(0.5f + index * 0.2f, destPos);
			card->runAction(action);
		}
		cardVec[index] = card;
		++curCount;
	}
	//cardVec.insert(cardVec.cend(), drawnVec.cbegin(), drawnVec.cend());
	numLabel->setString(to_string(curCount));
	numLabel->setVisible(curCount > 0);
}

vector<BSCard*> BSPrizePile::draw(const vector<unsigned int> &idxVec) {
	vector<BSCard*> ret;

	for (const auto &idx : idxVec) {
		if (0 <= idx && idx < cardVec.size() && cardVec[idx]) {
			ret.push_back(cardVec[idx]);

			cardVec[idx]->stopAllActions();
			cardVec[idx]->removeFromParent();
			cardVec[idx] = nullptr;
			
			if (curCount > 0)
				curCount -= 1;
		}
	}

	numLabel->setString(to_string(curCount));
	numLabel->setVisible(curCount > 0);
	return ret;
}

void BSPrizePile::showSelectPanel() {

}


void BSPrizePile::addCard(BSCard *card, unsigned int idx, float delay) {
	if (idx < 0 || idx >= capacity)
		return;

	// Tra ve pile
	auto destPos = Vec2(0, 0);
	auto startPos = destPos + Vec2(0, 100);
	card->setPosition(startPos);
	auto action = MoveTo::create(delay, destPos);
	card->runAction(action);

	if(card->getParent())
		card->removeFromParent();

	cardMarker->addChild(card);
	cardVec[idx] = card;
	
}

BATTLE_SCENE_NS_END