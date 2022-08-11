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

class CardHolder;

class BSPlayer final {
public:
	friend class BattleScene;
	friend class CardHolder;

	enum class PlayerActionId {
		NONE,
		PLAY_BASIC_PET,
		EVOLVE_PET,
		ATTACH_ENERGY,
		RETREAT_PET
	};
	class PlayerAction {
	public:
		PlayerActionId id = PlayerActionId::NONE;
		unsigned int doneCount = 0;
		unsigned int maxCount = 0;
		bool doneLimited = false;
		PlayerAction(PlayerActionId actionId);
	};


	BSPlayer(PlayerIdType i);
	~BSPlayer();

	PlayerIdType id = PlayerIdInvalid;

private:
	std::shared_ptr<PlayerAction> getActionData(PlayerActionId id);
	std::unordered_map<PlayerActionId, std::shared_ptr<PlayerAction>> actionMap;

	// Utilize
};

BATTLE_SCENE_NS_END

#endif // __BS_PLAYER_H__
