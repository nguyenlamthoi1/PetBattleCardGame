#ifndef __BS_PLAYER_H__
#define __BS_PLAYER_H__

#include "cocos2d.h"
#include "BSDefine.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

BATTLE_SCENE_NS_BEG

class BSPlayer final {
public:
	friend class BattleScene;

	BSPlayer(PlayerIdType i);
	~BSPlayer();

	PlayerIdType id = PlayerIdInvalid;
private:
};

BATTLE_SCENE_NS_END

#endif // __BS_PLAYER_H__
