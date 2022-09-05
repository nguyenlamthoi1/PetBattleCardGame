#include "BSPlayer.h"

using namespace std;
USING_NS_CC;



BATTLE_SCENE_NS_BEG

/*
	BSPlayer Class
*/

BSPlayer::BSPlayer(BattleScene *scn, PlayerIdType pid) : btlScn(scn), id(pid) {
	initActionDoneLimitMap();
}

BSPlayer::~BSPlayer(){}

/*
	Player Actions
*/

void BSPlayer::initActionDoneLimitMap() {
	actionDoneLimit.clear();
	actionDoneLimit[TurnAction::AttachEnergy] = make_shared<TurnActionData>(TurnActionData({0, GConfig::ATTACH_ENERGY_TURN_LIMIT}));
	actionDoneLimit[TurnAction::EvolvePet] = make_shared<TurnActionData>(TurnActionData({ 0, GConfig::EVOLVE_PET_TURN_LIMIT }));
	actionDoneLimit[TurnAction::PlayBasicPet] = make_shared<TurnActionData>(TurnActionData({ 0, GConfig::PLAY_BASIC_PET_TURN_LIMIT }));
	actionDoneLimit[TurnAction::PlayItem] = make_shared<TurnActionData>(TurnActionData({ 0, GConfig::PLAY_ITEM_TURN_LIMIT }));
	actionDoneLimit[TurnAction::PlaySupporter] = make_shared<TurnActionData>(TurnActionData({ 0, GConfig::PLAY_SPT_TURN_LIMIT }));
	actionDoneLimit[TurnAction::Retreat] = make_shared<TurnActionData>(TurnActionData({ 0, GConfig::RETREAT_PET_TURN_LIMIT }));
}

void BSPlayer::resetTurnActionCounts() {
	for (auto &itr : actionDoneLimit)
		itr.second->doneCount = 0;
}
void BSPlayer::incActionDoneCount(const TurnAction &action, int incFactor) {
	auto itr = actionDoneLimit.find(action);
	if (itr != actionDoneLimit.cend()) 
		itr->second->doneCount += incFactor;
}
bool BSPlayer::isActionCountReachLimit(const TurnAction &action) {
	auto itr = actionDoneLimit.find(action);
	if (itr != actionDoneLimit.cend())
		return itr->second->doneCount >= itr->second->doneLimit;
	return true;
}

void BSPlayer::startSetup() {

}

void BSPlayer::startTurn() {
	resetTurnActionCounts();
}

BATTLE_SCENE_NS_END