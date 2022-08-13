#include "BSAction.h"
#include "../BattleScene.h"
#include "../BattleManager.h"
#include "../BSHand.h"

#include <algorithm>

using namespace std;

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
	BSAction::start();

	if (doFunc)
		doFunc();
	state = State::Done;
}

void CustomAction::end() {

}

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
	BSAction::start();
	auto btlScn = btlMgr->getBattleScene();
	auto &hand = btlScn->hands[playerId];
	hand->drawCards(drawnNum, [this]() {
		CCLOG("Action draw end");
		state = State::Done; 
		});
	state = State::Done;
}

void DrawCardAction::end() {

}

BATTLE_SCENE_NS_END