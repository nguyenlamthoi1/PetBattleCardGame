#include "Player.h"

using namespace std;

NS_GAME_BEGIN

Player::PlayerPtr Player::createShPtr(const PlayerIdType &id) {
	auto ret = make_shared<Player>(id);
	if (!ret || !ret->init())
		ret.reset(); // nullptr
	return ret;
}

Player::Player(const PlayerIdType &id) : 
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

Player::~Player() {}

bool Player::init() {

	return true;
}

Player::PlayerPtr Player::clone() const {
	//TODO
	return nullptr;
}

void Player::resetDoneCount() {
	for (auto &actionInfo : limitTurnActionMap) {
		auto &data = actionInfo.second;
		data.doneCount = 0;
	}
}

void Player::updateActionCount(TurnAction action, int count) {
	auto itr = limitTurnActionMap.find(action);
	if (itr != limitTurnActionMap.cend())
		itr->second.doneCount += count;
}

bool Player::actionExceedsLimit(TurnAction action) const {
	auto itr = limitTurnActionMap.find(action);
	if (itr != limitTurnActionMap.cend())
		return itr->second.doneCount >= itr->second.maxCount;
	return false;
}

NS_GAME_END