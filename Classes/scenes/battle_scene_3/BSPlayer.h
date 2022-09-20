#pragma once

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"
#include "BSDefine.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

BATTLE_SCENE_NS_BEG

class BattleScene;
class BSCard;

class BSPlayer {
public:
	enum class TurnAction {
		PlayPetCard, // No limit
		EvolvePet, // No limit
		AttachEnergy, // Limited
		Retreat, // Limited
		UseItem, // No Limit
		UseSupporter, // Limited
		Attack // Limited
	};

	using PlayerPtr = std::shared_ptr<BSPlayer>;
	static PlayerPtr createShPtr(BattleScene *btlScn, const PlayerIdType &id);
	BSPlayer(BattleScene *btlScn, const PlayerIdType &id);

	virtual ~BSPlayer();
	virtual bool init();

	PlayerPtr clone() const;

	void resetDoneCount();
	void updateActionCount(TurnAction action, int count);
	bool actionExceedsLimit(TurnAction action) const;
protected:
	BattleScene *btlScn = nullptr;

	PlayerIdType pid = PlayerIdInvalid;

	struct TurnActionInfo {
		unsigned int doneCount = 0;
		unsigned int maxCount = 0;
	};
	std::unordered_map<TurnAction, TurnActionInfo> limitTurnActionMap;
};

BATTLE_SCENE_NS_END
