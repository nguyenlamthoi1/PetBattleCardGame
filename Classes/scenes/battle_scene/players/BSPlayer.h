#ifndef __BS_PLAYER_H__
#define __BS_PLAYER_H__

#include "cocos2d.h"

#include "../BSDefine.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

BATTLE_SCENE_NS_BEG

class CardHolder;
class BattleManager;
class CardHolder;
class PetCard;
class EnergyCard;

/*
	- BSPlayer la base class cho cac class dieu khien nguoi choi(Gamer), va dieu khien doi thu AI(Opponent)
*/
class BSPlayer {
public:
	friend class BattleScene;
	friend class CardHolder;
	friend class BattleManager;

	

	BSPlayer(BattleScene *scn, PlayerIdType i);
	virtual ~BSPlayer();

	PlayerIdType getId() const { return id; }

	/// Player Action
	virtual void startSetup() = 0;
	virtual void startTurn() = 0;
	virtual void playPetCardFromHand();
	virtual void 
	

protected:

	BattleScene *btlScn = nullptr;
	PlayerIdType id = PlayerIdInvalid;

	/// Data moi luot
	enum class TurnAction { // Hanh dong nguoi choi moi luot
		PlayBasicPet,
		EvolvePet,
		AttachEnergy,
		Retreat,
		UseAbility,
		PlaySupporter,
		PlayItem
	};
	struct TurnActionData {
		unsigned int doneCount = 0;
		unsigned int doneLimit = 0;
		TurnActionData() = default;
	};
	std::unordered_map<TurnAction, std::shared_ptr<TurnActionData>> actionDoneLimit;
	void initActionDoneLimitMap();
	void resetTurnActionCounts(); // Reset so lan thuc hien tat ca action moi luot
	void incActionDoneCount(const TurnAction &action, int incFactor = 1);
	bool isActionCountReachLimit(const TurnAction &action);
	///
};

/*
	- BSGamer la class dieu khien nguoi choi Gamer
*/
class BSGamer : public BSPlayer {
public:
	friend class BattleScene;
	friend class CardHolder;
	friend class BattleManager;

	BSGamer(BattleScene *scn, PlayerIdType i);
	virtual ~BSGamer();

	virtual void startSetup() override;
	virtual void startTurn() override;

protected:

};

/*
	- BSOpponent la abstract base class dieu khien doi thu
	- Nhung giai thuat dieu khien Opponent se duoc trien khai bang cach ke thua tu class nay
*/

class BSOpponent : public BSPlayer {
public:
	friend class BattleScene;
	friend class CardHolder;
	friend class BattleManager;

	BSOpponent(BattleScene *scn, PlayerIdType i);
	virtual ~BSOpponent();
};


/*
	- BSControlledOpponent la class dieu khien doi thu dua  bang Command Typing(go lenh dieu khien)
	- Chu yeu dung test game
*/

class BSControlledOpponent : public BSOpponent{
public:
	BSControlledOpponent(BattleScene *scn, PlayerIdType i);
	virtual ~BSControlledOpponent();

	virtual void startSetup() override;
	virtual void startTurn() override;
};

BATTLE_SCENE_NS_END

#endif // __BS_PLAYER_H__
