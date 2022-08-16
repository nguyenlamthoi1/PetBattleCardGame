#include "BSAction.h"
#include "../BattleScene.h"
#include "../BattleManager.h"
#include "../BSHand.h"
#include "../BSCoinFlipper.h"

#include <algorithm>

using namespace std;
USING_NS_CC;

BATTLE_SCENE_NS_BEG

BSAction::BSAction() : state(State::Wait), bs(nullptr){
}

BSAction::BSAction(shared_ptr<BattleManager> &bm) : state(State::Wait), bs(nullptr), btlMgr(bm) {

}


BSAction::~BSAction() {
	CCLOG("BSAction::Dtor %p", this);
}

void BSAction::start() {
	state = State::Processed;
}

void BSAction::end() {
	state = State::Done;
	pop();
}

void BSAction::pushedTo(BattleScene *battleScene){
	//bs = battleScene;
	//auto &pipeline = bs->pipeline;
	//pipeline.push_back(this);
}

void BSAction::pop() {
	/*auto &pipeline = bs->pipeline;
	auto it = find(pipeline.cbegin(), pipeline.cend(), this);
	pipeline.erase(it);*/
}

/*
	CustomAction Class
*/

CustomAction::CustomAction(std::function<void()> f) : doFunc(f) {
	CCLOG("CustomAction::Ctor %p", this);
}

CustomAction::~CustomAction() {
	CCLOG("CustomAction::Dtor %p", this);
}

void CustomAction::start() {
	if (state != State::Wait)
		return;
	state = State::Processed;
	if (doFunc)
		doFunc();
	state = State::Done;
}

void CustomAction::end() {}

/*
	WaitAction Class
*/

const string WaitAction::WAIT_ACTION_SCHEDULER = "WAIT_ACTION_SCHEDULER";

WaitAction::WaitAction(float t) : waitTime(t) {}
WaitAction::~WaitAction() {}

void WaitAction::start() {
	if (state != State::Wait)
		return;

	state = State::Processed;
	Director::getInstance()->getScheduler()->schedule([this](float t) {
		state = State::Done;
		}, this, 0, 0, waitTime, false, WAIT_ACTION_SCHEDULER);
}
void WaitAction::end() {}


/*
	DrawCardAction Class
*/

DrawCardAction::DrawCardAction(std::shared_ptr<BattleManager> &bm, PlayerIdType id, size_t n) : BSAction(bm), playerId(id), drawnNum(n){
	CCLOG("DrawCardAction::Ctor %p", this);
}

DrawCardAction::~DrawCardAction() {
	CCLOG("DrawCardAction::Dtor %p", this);
}

void DrawCardAction::start() {
	if (state != State::Wait)
		return;

	state = State::Processed;
	auto btlScn = btlMgr->getBattleScene();
	auto &hand = btlScn->hands[playerId];
	hand->drawCards(drawnNum, [this]() {
		CCLOG("Action draw end");
		state = State::Done; 
		});
}

void DrawCardAction::end() {}

/*
	FlipCoinAction
*/

FlipCoinAction* FlipCoinAction::createFlip1Coin(shared_ptr<BattleManager> &bm, PlayerIdType id) {
	auto ret = new FlipCoinAction(bm, id);
	ret->flipNum = 1;
	ret->flipType = FlipType::Flip_1;
	return ret;
}

FlipCoinAction* FlipCoinAction::createFlipMulCoins(shared_ptr<BattleManager> &bm, PlayerIdType id, unsigned int count) {
	auto ret = new FlipCoinAction(bm, id);
	ret->flipNum = count;
	ret->flipType = FlipType::Flip_Mul;
	return ret;
}

FlipCoinAction* FlipCoinAction::createFlipUntilTails(shared_ptr<BattleManager> &bm, PlayerIdType id) {
	return nullptr;
}

FlipCoinAction::FlipCoinAction(shared_ptr<BattleManager> &bm, PlayerIdType id) : BSAction(bm), playerId(id) {}

FlipCoinAction::~FlipCoinAction() {}

void FlipCoinAction::start() {
	if (state != State::Wait)
		return;

	state = State::Processed;

	auto btlScn = btlMgr->getBattleScene();
	auto flipper = btlScn->getCoinFlipper();

	flipper->registFlipEndCallbackOnce([this]() {state = State::Done; });

	switch (flipType) {
	case FlipType::Flip_1:
		flipper->startFlip1Coin(playerId);
		break;
	case FlipType::Flip_Mul:
		flipper->startFlipMulCoins(playerId, flipNum);
		break;
	}
}

void FlipCoinAction::end() {}

BATTLE_SCENE_NS_END