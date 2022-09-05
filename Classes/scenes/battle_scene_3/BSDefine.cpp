#include "BSDefine.h"

BATTLE_SCENE_NS_BEG

bool isPlayerIdInvalid(const PlayerIdType &id) {
	return id == PLAYER || id == OPPONENT;
}


BATTLE_SCENE_NS_END