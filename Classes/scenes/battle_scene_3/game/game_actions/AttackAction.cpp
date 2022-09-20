#include "GameAction.h"

#include "../BattleMaster.h"
#include "../game_state/GameState.h"
#include "../game_state/Deck.h"
#include "../game_state/Hand.h"
#include "../game_state/Board.h"
#include "../game_state/DiscardPile.h"
#include "../game_state/PrizePile.h"
#include "../game_state/Holder.h"
#include "../game_state/Player.h"

#include "../game_state/card/Card.h"

#include "data/MoveData.h"

#include "../../BattleScene.h"
#include "../../scene_actions/BSAction.h"

#include "../player_actions/PlayerAction.h"

using namespace std;
USING_NS_CC;

NS_GAME_BEGIN

/*
	UseActiveMove Class
*/

shared_ptr<GameAction> UseActiveMove::createAction(GameState *gstate, const shared_ptr<const ActionData> &adata) {
	auto aid = adata->id;
	if (aid == "Attack_Default") {
		auto oId = gstate->getOpponentOf(pid);
		// Attacker
		auto baseDmg = adata->ovrValues.at(to_string(0));
		auto atkBoard = gstate->getBoard(pid);
		auto atkHolder = atkBoard->getActiveHolder();
		// Defender
		auto defBoard = gstate->getBoard(oId);
		auto defHolder = defBoard->getActiveHolder();
		
		return make_shared<MGame::DefaultAttack>(atkHolder, defHolder, baseDmg);
	}
	else if(false) {
		return nullptr;
	}
	return nullptr;
}


void UseActiveMove::executeOn(GameState *gstate) {
	state = State::Process;

	auto player = gstate->getPlayer(pid);
	bool check1 = /*!player->actionExceedsLimit(Player::TurnAction::Retreat)*/ true;
	bool check2 = gstate->isCurPlayer(pid);

	if (check1 && check2) { // Bat dau move
		auto board = gstate->getBoard(pid);
		auto activeHolder = board->getActiveHolder();
		auto petCardData = activeHolder->getPetCard()->getPetData();
		const auto &moveVec = petCardData->moveVec;
		
		std::vector<shared_ptr<GameAction>> actionVec;
		if (midx < moveVec.size()) { // Lay move tai vi tri
			const auto &actions = moveVec[midx]->actions; // Lay ra cac action cua move
			
			bool createSuc = true;
			for (const auto adata : actions) {
				auto gaction = createAction(gstate, adata);
				if (!gaction) {
					createSuc = false;
					break;
				}
				actionVec.push_back(gaction);
			}
			if (!createSuc)
				actionVec.clear();
			
			if (!actionVec.empty())
				gstate->replaceCurActionWithVec(actionVec);
		}
	}

	state = State::Done;
}
shared_ptr<BattleSceneNS::BSAction> UseActiveMove::getBSAction() const {
	return nullptr;
}

shared_ptr<GameAction> UseActiveMove::clone() const {
	return make_shared<UseActiveMove>(pid, midx);
}

/*
	DefaultAttack Class
*/

void DefaultAttack::executeOn(GameState *gstate) {
	state = State::Process;

	auto atkCard = attacker->getPetCard();
	auto atkData = atkCard->getPetData();
	auto atkType = atkData->type;

	auto defCard = defender->getPetCard();
	auto defData = defCard->getPetData();
	auto weakType = *(defData->weakSet.begin());

	int incDmg = 0;

	// Xet resistance
	bool triggerResistance = false;
	auto resistType = defData->resistanceMap.begin()->first;
	if (!resistType.empty() && atkType == resistType)
	{
		auto resistNum = defData->resistanceMap.begin()->second;
		triggerResistance = true;
		incDmg += resistNum; // * Nen la so am
	}

	bool triggerWeak = false;

	// Xet weakness
	if (!weakType.empty() && atkType == weakType) {
		triggerWeak = true;
		incDmg += baseDmg; // x2 Base Damge
	}

	/*gstate->replaceCurActionWith({
		make_shared<TakeDamage>(
			defender,
			baseDmg + incDmg,
			baseDmg,
			triggerWeak,
			triggerResistance
			)
		});*/

	state = State::Done;
}

shared_ptr<BattleSceneNS::BSAction> DefaultAttack::getBSAction() const {
	return nullptr;
}

shared_ptr<GameAction> DefaultAttack::clone() const {
	return make_shared<DefaultAttack>(attacker, defender, baseDmg);
}

void TakeDamage::executeOn(GameState *gameState) {

}

shared_ptr<GameAction> TakeDamage::clone() const {
	return nullptr;
}

shared_ptr<BattleSceneNS::BSAction> getBSAction() {
	return nullptr;
}

NS_GAME_END
