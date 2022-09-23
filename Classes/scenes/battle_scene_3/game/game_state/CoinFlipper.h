#pragma once

#include "../GameDefine.h"

NS_GAME_BEGIN

class CoinFlipper final {
public:
	using CFlipperPtr = std::shared_ptr<CoinFlipper>;

	using SideType = bool;
	static const SideType HEADS = true;
	static const SideType TAILS = false;

	static CFlipperPtr createShPtr(const PlayerIdType &id);

	CoinFlipper(const PlayerIdType &id);
	~CoinFlipper();
	bool init();

	std::vector<SideType> doFlip(unsigned int flipCount);

	void clear();
	CFlipperPtr clone() const;
private:
	PlayerIdType pid;
	std::vector<SideType> resVec; // Vector ket qua cua tat ca cac lan toss coin
};

NS_GAME_END