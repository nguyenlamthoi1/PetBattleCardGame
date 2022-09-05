//#pragma once
//
//#include "../GameDefine.h"
//
//#include <vector>
//#include <memory>
//#include <string>
//
//NS_GAME_BEGIN
//
//class Player {
//public:
//	using PlayerPtr = std::shared_ptr<Player>;
//
//	static PlayerPtr createShPtr(const PlayerIdType &id);
//
//	Player(const PlayerIdType &id);
//	virtual ~Player();
//	virtual bool init();
//
//protected:
//	PlayerIdType pid = PlayerIdInvalid;
//
//	PetCardPtr petCard;
//	EnergyCardPtr energyCards;
//};
//
//NS_GAME_END