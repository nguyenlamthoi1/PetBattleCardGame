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
class BattleManager;
class CardHolder;
class PetCard;
class EnergyCard;

//namespace PlayerAction {
//	class Action {
//	public:
//		enum class Type {
//			None,
//			PlayPetCard,
//			PlayEnergy,
//			RetreatPet,
//			PlayItem,
//			PlaySupporter
//		};
//
//		Action(Type aType, PlayerIdType pId) : type(aType), id(pId) {}
//		virtual ~Action() = default;
//		Type getType() const { return type; }
//
//	protected:
//		Type type = Type::None;
//		PlayerIdType id = PlayerIdInvalid;
//	};
//
//	class PlayPet : public Action{
//	public:
//		std::shared_ptr<PlayPet> create(PlayerIdType id);
//		PlayPet(PlayerIdType id) : Action(Type::PlayPetCard, id){}
//		virtual ~PlayPet(){}
//	};
//}




class BSPlayer {
public:
	friend class BattleScene;
	friend class CardHolder;
	friend class BattleManager;

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

	// Player Action
	virtual void doSetup();

protected:
	void drawFromDeck(unsigned int num);

protected:
	std::shared_ptr<PlayerAction> getActionData(PlayerActionId id);
	std::unordered_map<PlayerActionId, std::shared_ptr<PlayerAction>> actionMap;

	// Utilize
};


class BSOpponent : public BSPlayer {

};

BATTLE_SCENE_NS_END

#endif // __BS_PLAYER_H__
