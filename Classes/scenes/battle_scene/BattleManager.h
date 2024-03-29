#ifndef __BATTLE_MANAGER_H__
#define __BATTLE_MANAGER_H__

#include "BSDefine.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <list>
#include <unordered_map>

BATTLE_SCENE_NS_BEG

class BattleScene;
class BSPlayer;
class BSAction;
class PlayerAction;

class PetCard;
class EnergyCard;
class CardHolder;

class BattleManager final {
public:
	friend class BattleScene;

	enum class GameState {
		NONE,
		START,
		SET_UP,
		PLAY
	};

	static const std::string BATTLE_MANAGER_SCHEDULER;
	static const std::string BATTLE_MANAGER_ACTION_SCHEDULER;

	BattleScene* getBattleScene() { return btlScn; }

	BattleManager(BattleScene *scn);
	~BattleManager();

	// Getters
	unsigned int getCurTurn();

private:
	void startGame();
	void endGame();
	void doGameLoop(float dt);
	void changeState(GameState fromState, GameState ntoState);

	BattleScene *btlScn = nullptr;
	std::unordered_map<PlayerIdType, std::shared_ptr<BSPlayer>> players;
	
	GameState gameState = GameState::NONE;
	unsigned int curTurn = 0;
	PlayerIdType curTurnId = PlayerIdInvalid;

	// Game Actions
	enum class PipelineState {
		NONE,
		WAIT,
		DOING
	};
	PipelineState pipState = PipelineState::NONE;
	//std::vector<std::shared_ptr<BSAction>> pipeline;
	std::list<std::shared_ptr<BSAction>> pipeline;
	void startPipeline(std::initializer_list<BSAction*> actionList);

	/*
		- Moi action "nen" tu handle su ket thuc cua minh(vi du: tu dong gan actionState = DONE). Tat ca action duoc handle theo kieu nay se duoc pipeline tu dong xoa tai frame ke tiep.
		- Neu action muon pipeline xu ly action ket thuc cung 1 frame -> tu dong call BattleManager::onActionEnd
	*/
	void updatePipeline(float dt); 

	void onActionEnd(std::shared_ptr<BSAction> endedaAction);

///PlayerAction///
public:
	bool playerTryPlayPetCard(PlayerIdType id, PetCard *card, CardHolder *holder);
	bool playerTryPlayEnergyCard(PlayerIdType id, EnergyCard *card, CardHolder *holder);
	bool playerStartSetup(PlayerIdType id);

///Events///
public:
	static const std::string PLAY_PET_CARD_EV;

	struct PlayPetEvData {
		PlayerIdType id = PlayerIdInvalid;
		PetCard *card = nullptr;
		CardHolder *holder = nullptr;
		bool suc = false;
	};

	static cocos2d::EventListenerCustom* registEvt(const std::string evt, const std::function<void(cocos2d::EventCustom*)> &listener);
private:
	static void dispatchEvt(const std::string s, void *evtData);
	// Checker
};

BATTLE_SCENE_NS_END

#endif // __BATTLE_MANAGER_H__
