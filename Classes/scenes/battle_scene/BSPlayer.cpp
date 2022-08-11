#include "BSPlayer.h"

using namespace std;
USING_NS_CC;

BATTLE_SCENE_NS_BEG

/*
	PlayerAction Class
*/

BSPlayer::PlayerAction::PlayerAction(PlayerActionId aid) : id(aid) {
	switch (id) {
	case PlayerActionId::PLAY_BASIC_PET:
		maxCount = ACTION_PLAY_BASIC_PET_MAX_COUNT_PER_TURN;
		break;
	case PlayerActionId::EVOLVE_PET:
		maxCount = ACTION_EVOLVE_COUNT_PER_TURN;
		break;
	case PlayerActionId::ATTACH_ENERGY:
		maxCount = ACTION_ATTACH_ENERGY_MAX_COUNT_PER_TURN;
		break;
	case PlayerActionId::RETREAT_PET:
		maxCount = ACTION_RETREAT_PET_MAX_COUNT_PER_TURN;
		break;
	default:
		maxCount = UINT_MAX;
	}
	doneCount = 0;
	doneLimited = maxCount != UINT_MAX;
}


BSPlayer::BSPlayer(PlayerIdType i) : id(i) {
	CCLOG("BSPlayer::Ctor %p", this);

	actionMap.insert(pair<PlayerActionId, shared_ptr<PlayerAction>>(PlayerActionId::PLAY_BASIC_PET, new PlayerAction(PlayerActionId::PLAY_BASIC_PET)));
	actionMap.insert(pair<PlayerActionId, shared_ptr<PlayerAction>>(PlayerActionId::EVOLVE_PET, new PlayerAction(PlayerActionId::EVOLVE_PET)));
	actionMap.insert(pair<PlayerActionId, shared_ptr<PlayerAction>>(PlayerActionId::ATTACH_ENERGY, new PlayerAction(PlayerActionId::ATTACH_ENERGY)));
	actionMap.insert(pair<PlayerActionId, shared_ptr<PlayerAction>>(PlayerActionId::RETREAT_PET, new PlayerAction(PlayerActionId::RETREAT_PET)));
}

BSPlayer::~BSPlayer() {
	CCLOG("BSPlayer::Dtor %p", this);
}

shared_ptr<BSPlayer::PlayerAction> BSPlayer::getActionData(PlayerActionId id) {
	auto foundIt = actionMap.find(id);
	if (foundIt != actionMap.end())
		return foundIt->second;
	return nullptr;
}


BATTLE_SCENE_NS_END