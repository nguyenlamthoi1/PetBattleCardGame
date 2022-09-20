#include "BSPlayer.h"
#include "BSCard.h"
#include "data/CardData.h"
#include "data/PlayerData.h"
#include "BattleScene.h"
#include "BSResources.h"

#include "common/Utilize.h"
#include <new>

USING_NS_CC;
using namespace std;

BATTLE_SCENE_NS_BEG

BSPlayer::PlayerPtr BSPlayer::createShPtr(BattleScene *btlScn, const PlayerIdType &id) {
	auto ret = make_shared<BSPlayer>(btlScn, id);
	if (!ret || !ret->init())
		ret.reset(); // nullptr
	return ret;
}

BSPlayer::BSPlayer(BattleScene *scn, const PlayerIdType &id) :
	btlScn(scn),
	pid(id),
	limitTurnActionMap({
	{TurnAction::PlayPetCard, {0, GConfig::PLAY_BASIC_PET_TURN_LIMIT}},
	{TurnAction::EvolvePet, {0, GConfig::EVOLVE_PET_TURN_LIMIT}},
	{TurnAction::AttachEnergy, {0, GConfig::ATTACH_ENERGY_TURN_LIMIT}},
	{TurnAction::Retreat, {0, GConfig::RETREAT_PET_TURN_LIMIT}},
	{TurnAction::UseItem, {0, GConfig::PLAY_ITEM_TURN_LIMIT}},
	{TurnAction::UseSupporter, {0, GConfig::PLAY_SPT_TURN_LIMIT}},
	{TurnAction::Attack, {0, GConfig::USE_ACTIVE_MOVE}},
		})
{

}

BSPlayer::~BSPlayer() {}

bool BSPlayer::init() {

	return true;
}

BSPlayer::PlayerPtr BSPlayer::clone() const {
	//TODO
	return nullptr;
}

void BSPlayer::resetDoneCount() {
	for (auto &actionInfo : limitTurnActionMap) {
		auto &data = actionInfo.second;
		data.doneCount = 0;
	}
}

void BSPlayer::updateActionCount(TurnAction action, int count) {
	auto itr = limitTurnActionMap.find(action);
	if (itr != limitTurnActionMap.cend())
		itr->second.doneCount += count;
}

bool BSPlayer::actionExceedsLimit(TurnAction action) const {
	auto itr = limitTurnActionMap.find(action);
	if (itr != limitTurnActionMap.cend())
		return itr->second.doneCount >= itr->second.maxCount;
	return false;
}


BATTLE_SCENE_NS_END