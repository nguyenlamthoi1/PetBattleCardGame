#include "BSAction.h"
#include "../BattleScene.h"
#include "../BSHand.h"
#include "../BSNotifier.h"

#include "../game/BattleMaster.h"
#include "../game/player_actions/PlayerAction.h"

#include "GameManager.h"

//#include "../BattleManager.h"
//#include "../BSCoinFlipper.h"
//#include "../BSBoard.h"

//#include "../players/BSPlayer.h"


#include <algorithm>

using namespace std;
USING_NS_CC;

BATTLE_SCENE_NS_BEG

/*
	BSAction Class
*/

BSAction::BSAction() : state(State::Wait) {}

BSAction::~BSAction() {}

void BSAction::executeOn(BattleScene *btlScn) { state = State::Done; }

/*
	CustomAction Class
*/

CustomAction::CustomAction(const std::function<void()> &f) : doFunc(f) {}

CustomAction::~CustomAction() {}

void CustomAction::executeOn(BattleScene *btlScn) {
	if (state != State::Wait)
		return;
	state = State::Processed;
	if (doFunc)
		doFunc();
	state = State::Done;
}

/*
	SequenceAction Class
*/

std::shared_ptr<SequenceAction> SequenceAction::create(std::initializer_list<ActionPtr> list) {
	auto ret = std::shared_ptr<SequenceAction>(new SequenceAction(list));
	return ret;
}

SequenceAction::SequenceAction(initializer_list<ActionPtr> list) {
	actions.insert(actions.cend(), list.begin(), list.end());
}

SequenceAction::~SequenceAction() {}

void SequenceAction::executeOn(BattleScene *btlScn) {
	if (state != State::Wait)
		return;
	state = State::Processed;
	
	/*for (const auto &action : actions)
		btlScn->pushAction(action);*/

	state = State::Done;
}

/*
	WaitAction Class
*/

const string WaitAction::WAIT_ACTION_SCHEDULER = "WAIT_ACTION_SCHEDULER";

WaitAction::WaitAction(float t) : waitTime(t) {}
WaitAction::~WaitAction() {}

void WaitAction::executeOn(BattleScene *btlScn) {
	if (state != State::Wait)
		return;

	state = State::Processed;
	Director::getInstance()->getScheduler()->schedule([this](float t) {
		state = State::Done;
		}, this, 0, 0, waitTime, false, WAIT_ACTION_SCHEDULER);
}

/*
	DrawCardAction Class
*/

DrawCardAction::DrawCardAction(const PlayerIdType &id, size_t n, CardList list) : pid(id), drawnNum(n) {
	drawnCards.reserve(GConfig::CARD_NUM_IN_DECK_MAX);
	drawnCards.insert(drawnCards.end(), list.begin(), list.end());
}

DrawCardAction::DrawCardAction(const PlayerIdType &id, size_t n, const std::vector<CardId> &list) : pid(id), drawnNum(n) {
	drawnCards.reserve(GConfig::CARD_NUM_IN_DECK_MAX);
	drawnCards.insert(drawnCards.end(), list.begin(), list.end());
}


DrawCardAction::DrawCardAction(const PlayerIdType &id) : pid(id) {}

DrawCardAction::~DrawCardAction() {}

void DrawCardAction::setDraw(size_t n, CardList list) {
	drawnNum = n;
	drawnCards.insert(drawnCards.cend(), list.begin(), list.end());
}

void DrawCardAction::setDraw(size_t n, const vector<CardId> &list) {
	drawnNum = n;
	drawnCards.insert(drawnCards.cend(), list.begin(), list.end());
}

void DrawCardAction::executeOn(BattleScene *btlScn) {
	if (state != State::Wait)
		return;

	state = State::Processed;
	auto hand = btlScn->getHand(pid);
	bool isPlayerId = pid == PLAYER_ID;
	hand->drawCards(drawnNum, drawnCards, !isPlayerId);
	onDrawnDone = hand->addEventHandler(BSHand::EV_DRAW_ACTION_DONE, [this, hand](const std::shared_ptr<MEvent>&) {
		state = State::Done;
		hand->removeHandler(onDrawnDone);
		});
}


/*
	FlipCoinAction
*/

//FlipCoinAction* FlipCoinAction::createFlip1Coin(shared_ptr<BattleManager> &bm, PlayerIdType id) {
//	auto ret = new FlipCoinAction(bm, id);
//	ret->flipNum = 1;
//	ret->flipType = FlipType::Flip_1;
//	return ret;
//}
//
//FlipCoinAction* FlipCoinAction::createFlipMulCoins(shared_ptr<BattleManager> &bm, PlayerIdType id, unsigned int count) {
//	auto ret = new FlipCoinAction(bm, id);
//	ret->flipNum = count;
//	ret->flipType = FlipType::Flip_Mul;
//	return ret;
//}
//
//FlipCoinAction* FlipCoinAction::createFlipUntilTails(shared_ptr<BattleManager> &bm, PlayerIdType id) {
//	return nullptr;
//}
//
//FlipCoinAction::FlipCoinAction(shared_ptr<BattleManager> &bm, PlayerIdType id) : BSAction(bm), playerId(id) {}
//
//FlipCoinAction::~FlipCoinAction() {}
//
//void FlipCoinAction::start() {
//	if (state != State::Wait)
//		return;
//
//	state = State::Processed;
//
//	auto btlScn = btlMgr->getBattleScene();
//	auto flipper = btlScn->getCoinFlipper();
//
//	flipper->registFlipEndCallbackOnce([this]() {state = State::Done; });
//
//	switch (flipType) {
//	case FlipType::Flip_1:
//		flipper->startFlip1Coin(playerId);
//		break;
//	case FlipType::Flip_Mul:
//		flipper->startFlipMulCoins(playerId, flipNum);
//		break;
//	}
//}
//
//void FlipCoinAction::end() {}
//
///*
//	SetupAction Class
//*/
//
//SetupAction::SetupAction(std::shared_ptr<BattleManager> &btlMgr, PlayerIdType id) : BSAction(btlMgr), playerId(id) {
//	CCLOG("DrawCardAction::Ctor %p", this);
//}
//
//SetupAction::~SetupAction(){
//	CCLOG("DrawCardAction::Dtor %p", this);
//}
//
//void SetupAction::start() {
//	if (state != State::Wait)
//		return;
//
//	state = State::Processed;
//
//	if (!btlMgr->playerStartSetup(playerId))
//		return;
//
//	auto btlScn = btlMgr->getBattleScene();
//	auto player = btlScn->getPlayer(playerId); // PLAYER or OPPONENT
//
//	if (playerId == PLAYER) {
//		auto notifier = btlScn->getNotifier();
//		notifier->showMsg("Drag a Pokemon to your Active Spot");
//		player->startSetup();
//
//		if (playPetListener)
//			playPetListener->release();
//
//		playPetListener = BattleManager::registEvt(BattleManager::PLAY_PET_CARD_EV, [this](EventCustom *evt) {
//			auto data = static_cast<BattleManager::PlayPetEvData*>(evt->getUserData());
//			if (data->suc) {
//				auto btlScn = btlMgr->getBattleScene();
//				auto notifier = btlScn->getNotifier();
//				notifier->hideMsg(0.5f, [this, notifier]() {
//					notifier->showMsgWithDone("You can play any basic pet card. Tap done button when you're ready",
//						[]() -> bool {return true; },
//						[this]() -> void {
//							state = State::Done;
//						},
//						nullptr, true);
//					});
//			
//
//				auto evDispatcher = Director::getInstance()->getEventDispatcher();
//				evDispatcher->removeEventListener(playPetListener);
//				playPetListener->release();
//				playPetListener = nullptr;
//			}
//			});
//		playPetListener->retain();
//	}
//	else if (playerId == OPPONENT) {
//		
//	}
//	
//}
//
//bool SetupAction::checkSetup() {
//	return btlMgr->getBattleScene()->getBoard(playerId)->hasActivePet();
//}
//
//
//void SetupAction::end(){}

/*
	GameOverAction Class
*/

GameOverAction::GameOverAction(const PlayerIdType &pid) : winnerId(pid) {}

GameOverAction::~GameOverAction(){}

void GameOverAction::executeOn(BattleScene *btlScn) {
	if (state != State::Wait)
		return;

	state = State::Processed;

	auto foundWinner = !winnerId.empty();
	if (foundWinner) {
		CCLOG("GameOver: %s won", winnerId.c_str());
	}
	else {
		CCLOG("GameOver: no winner", winnerId.c_str());
	}

	state = State::Done;
}


//-----------//
//SET UP FLOW//
//-----------//

StartSetupAction::StartSetupAction() {}

StartSetupAction::~StartSetupAction() {}

const string StartSetupAction::START_SETUP_TXT = "TXT_BS_START_SET_UP";

void StartSetupAction::executeOn(BattleScene *btlScn) {
	if (state != State::Wait)
		return;

	state = State::Processed;

	auto lang = GM_LANG;
	auto notifier = btlScn->getNotifier();
	notifier->showMsgAndHideAfter(lang->getString(START_SETUP_TXT), 1.5f);

	state = State::Done;
}

//---//

const string StartSetupActive::PLAYER_SETUP_TXT = "TXT_BS_START_PLAYER_SET_UP";
const string StartSetupActive::OPP_SETUP_TXT = "TXT_BS_START_OPPONENT_SET_UP";

StartSetupActive::StartSetupActive(const PlayerIdType &id) : pid(id) {}

StartSetupActive::~StartSetupActive() {}

void StartSetupActive::executeOn(BattleScene *btlScn) {
	if (state != State::Wait)
		return;

	state = State::Processed;

	auto lang = GM_LANG;

	if (btlScn->getPlayerId() == pid) { // Player Action
		auto notifier = btlScn->getNotifier();
		notifier->showMsgAndHideAfter(lang->getString(PLAYER_SETUP_TXT), 1.5f);
	
		auto hand = btlScn->getHand(pid);
		hand->setEnableDragSetupActive(true);
	}
	else { // Opponent Action
		auto notifier = btlScn->getNotifier();
		notifier->showMsgAndHideAfter(lang->getString(OPP_SETUP_TXT), 1.5f);
	
		//TODO: AI_MAKE_DECISION
	}

	//state = State::Done; // Doi den khi co Player Input
}

bool StartSetupActive::onReceivePlayerInput(const shared_ptr<MGame::BattleMaster> & bm, const shared_ptr<MGame::PlayerAction> &pAction) {
	if (pAction->getType() == MGame::PlayerAction::Type::SetupActivePet) {
		auto error =  bm->onPlayerChooseAction(pAction);
		return error != ActionError::Failed;
	}
	return false;
}


//---//

StartSetupBench::StartSetupBench(const PlayerIdType &id) : pid(id) {}

StartSetupBench::~StartSetupBench() {}

void StartSetupBench::executeOn(BattleScene *btlScn) {
	if (state != State::Wait)
		return;

	state = State::Processed;

	//TODO

	state = State::Done;
}

//---//

DoSetupPetActive::DoSetupPetActive(const PlayerIdType &id, unsigned int hidx) : pid(id), handIdx(hidx){}

DoSetupPetActive::~DoSetupPetActive() {}

void DoSetupPetActive::executeOn(BattleScene *btlScn) {
	if (state != State::Wait)
		return;

	state = State::Processed;

	//TODO

	state = State::Done;
}

DoSetupPetBench::DoSetupPetBench(const PlayerIdType &id, unsigned int handIdx, unsigned int bIdx) : pid(id), benchIdx(bIdx) {}

DoSetupPetBench::~DoSetupPetBench() {}

void DoSetupPetBench::executeOn(BattleScene *btlScn) {
	if (state != State::Wait)
		return;

	state = State::Processed;

	//TODO

	state = State::Done;
}



BATTLE_SCENE_NS_END