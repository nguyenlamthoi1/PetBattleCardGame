#pragma once

#include "../GameDefine.h"

NS_GAME_BEGIN

class Holder;

class Board {
public:
	using HolderPtr = std::shared_ptr<Holder>;
	using HolderPtrVec = std::vector<HolderPtr>;
	using BoardPtr = std::shared_ptr<Board>;

	static BoardPtr createShPtr(const PlayerIdType &id);

	Board(const PlayerIdType &id);
	virtual ~Board();
	virtual bool init();

protected:
	PlayerIdType pid = PlayerIdInvalid;

	HolderPtr active;

	unsigned int benchCapacity;
	HolderPtrVec bench;
};

NS_GAME_END
