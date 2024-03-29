#include "BSAction.h"
#include "../BattleScene.h"
#include "../BattleManager.h"
#include "../BSHand.h"
#include "../BSCoinFlipper.h"
#include "../BSNotifier.h"
#include "../BSBoard.h"

#include "../players/BSPlayer.h"


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

	//isPlayerIdInvalid(playerId);
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

/*
	SetupAction Class
*/

SetupAction::SetupAction(std::shared_ptr<BattleManager> &btlMgr, PlayerIdType id) : BSAction(btlMgr), playerId(id) {
	CCLOG("DrawCardAction::Ctor %p", this);
}

SetupAction::~SetupAction(){
	CCLOG("DrawCardAction::Dtor %p", this);
}

void SetupAction::start() {
	if (state != State::Wait)
		return;

	state = State::Processed;

	if (!btlMgr->playerStartSetup(playerId))
		return;

	auto btlScn = btlMgr->getBattleScene();
	auto player = btlScn->getPlayer(playerId); // PLAYER or OPPONENT

	if (playerId == PLAYER) {
		auto notifier = btlScn->getNotifier();
		notifier->showMsg("Drag a Pokemon to your Active Spot");
		player->startSetup();

		if (playPetListener)
			playPetListener->release();

		playPetListener = BattleManager::registEvt(BattleManager::PLAY_PET_CARD_EV, [this](EventCustom *evt) {
			auto data = static_cast<BattleManager::PlayPetEvData*>(evt->getUserData());
			if (data->suc) {
				auto btlScn = btlMgr->getBattleScene();
				auto notifier = btlScn->getNotifier();
				notifier->hideMsg(0.5f, [this, notifier]() {
					notifier->showMsgWithDone("You can play any basic pet card. Tap done button when you're ready",
						[]() -> bool {return true; },
						[this]() -> void {
							state = State::Done;
						},
						nullptr, true);
					});
			

				auto evDispatcher = Director::getInstance()->getEventDispatcher();
				evDispatcher->removeEventListener(playPetListener);
				playPetListener->release();
				playPetListener = nullptr;
			}
			});
		playPetListener->retain();
	}
	else if (playerId == OPPONENT) {
		
	}
	
}

bool SetupAction::checkSetup() {
	return btlMgr->getBattleScene()->getBoard(playerId)->hasActivePet();
}


void SetupAction::end(){}

BATTLE_SCENE_NS_END