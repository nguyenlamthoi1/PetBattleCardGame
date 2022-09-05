#include "GameAction.h"

#include "../BattleMaster.h"
#include "../game_state/GameState.h"
#include "../game_state/Deck.h"
#include "../game_state/Hand.h"
#include "../game_state/Board.h"
#include "../game_state/card/Card.h"

#include "../../BattleScene.h"
#include "../../scene_actions/BSAction.h"

using namespace std;
USING_NS_CC;
using namespace BattleSceneNS;

NS_GAME_BEGIN


/*
	CustomAction Class
*/

//CustomAction::CustomAction(const CustomFunction &f){}
//
//CustomAction::~CustomAction(){}
//
//void CustomAction::start() {
//	state = State::Process;
//	doFunc();
//	state = State::Done;
//}


/*
	DrawAction Class
*/
DrawAction::DrawAction(DrawType drType, const PlayerIdType &id, unsigned int num) : drawType(drType), pid(id), drawnNum(num) {
	CCLOG("DrawAction::ctor called %d", this);

}

DrawAction::~DrawAction() {
	CCLOG("DrawAction::dtor called %d", this);
};

void DrawAction::start() {
	state = State::Process;

	auto gstate = BattleMaster::get()->getGameState();
	
	auto deck = gstate->getDeck(pid);

	if (drawType == DrawType::FromTop) {
		Deck::CardVector cardsVec;
		deck->popTop(drawnNum, cardsVec);

		// Push Animation
		auto bs = BattleScene::getScn();
		vector<string> cardIdsVec;
		for (const auto &card : cardsVec) // *Tao vector chua card id
			cardIdsVec.emplace_back(card->getId());

		bs->pushActions({ 
			make_shared<BattleSceneNS::DrawCardAction>(pid, drawnNum, cardIdsVec),
			make_shared<BattleSceneNS::WaitAction>(1.0f)
			});
	}
	else if(drawType == DrawType::FromBottom){
		Deck::CardVector cardsVec;
		deck->popBottom(drawnNum, cardsVec);

		// Push Animation
		auto bs = BattleScene::getScn();
		vector<string> cardIdsVec;
		for (const auto &card : cardsVec) // *Tao vector chua card id
			cardIdsVec.emplace_back(card->getId());

		bs->pushActions({
			make_shared<BattleSceneNS::DrawCardAction>(pid, drawnNum, cardIdsVec),
			make_shared<BattleSceneNS::WaitAction>(1.0f)
			});
	}
	
	state = State::Done;
}

void DrawAction::startDrawFromBottom() {

}

void DrawAction::startDrawTop() {

}

NS_GAME_END