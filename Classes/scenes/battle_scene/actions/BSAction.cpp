#include "BSAction.h"
#include "../BattleScene.h"
#include "../BSHand.h"

#include <algorithm>

BATTLE_SCENE_NS_BEG

BSAction::BSAction() : state(State::Wait), bs(nullptr){
}

BSAction::~BSAction() {

}

void BSAction::start() {
	state = State::Processed;
}

void BSAction::end() {
	state = State::Done;
	pop();
}

void BSAction::pushedTo(BattleScene *battleScene){
	bs = battleScene;
	auto &pipeline = bs->pipeline;
	pipeline.push_back(this);
}

void BSAction::pop() {
	auto &pipeline = bs->pipeline;
	auto it = find(pipeline.cbegin(), pipeline.cend(), this);
	pipeline.erase(it);
}


/*
	DrawCardAction Class
*/

DrawCardAction::DrawCardAction(PlayerIdType id, size_t n) : playerId(id), drawnNum(n){

}

void DrawCardAction::start() {
	BSAction::start();
	
	auto &hand = bs->hands[playerId];
	hand->drawCards(drawnNum, [this]() {
		state = State::Done; 
		pop();
		});
}

void DrawCardAction::end() {

}

BATTLE_SCENE_NS_END