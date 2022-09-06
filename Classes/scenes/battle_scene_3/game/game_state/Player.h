#pragma once

#include "../GameDefine.h"

NS_GAME_BEGIN

class GameState;

class Player {
public:
	enum class TurnAction {
		PlayPetCard, // No limit
		EvolvePet, // No limit
		AttachEnergy, // Limited
		Retreat, // Limited
		UseItem, // No Limit
		UseSupporter // Limited
	};

	using PlayerPtr = std::shared_ptr<Player>;
	static PlayerPtr createShPtr(const PlayerIdType &id);
	Player(const PlayerIdType &id);

	virtual ~Player();
	virtual bool init();

	PlayerPtr clone() const;

	void resetDoneCount();
	void updateActionCount(TurnAction action, int count);
	bool actionExceedsLimit(TurnAction action) const;
protected:
	PlayerIdType pid = PlayerIdInvalid;

	struct TurnActionInfo {
		unsigned int doneCount = 0;
		unsigned int maxCount = 0;
	};
	std::unordered_map<TurnAction, TurnActionInfo> limitTurnActionMap;
};

NS_GAME_END
