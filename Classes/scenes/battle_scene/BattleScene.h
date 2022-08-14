#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"

#include "BSDefine.h"
#include "data/PlayerData.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>
#include <list>

class PlayerData;

BATTLE_SCENE_NS_BEG

class BattleManager;
class BSHand;
class BSDeck;
class BSAction;
class BSBoard;
class BSPlayer;
class BSCard;
class BSCoinFlipper;

namespace BATTLE_SCENE_Z {
	const float DETAILED_CARD = 1000;
};

class BattleScene final : public cocos2d::Scene
{
public:
	friend class BSHand;
	friend class BSDeck;
	friend class BSBoard;

	static BattleScene* getScn();
    static BattleScene* create();

	std::shared_ptr<BSPlayer> getPlayer(PlayerIdType id) { return players[id]; }
	std::shared_ptr<PlayerData> getPlayerData(PlayerIdType id) { return playerData[id]; }
private:
	BattleScene();
	virtual ~BattleScene();

	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;

	void start();

	std::unordered_map<PlayerIdType, std::shared_ptr<PlayerData>> playerData;
public:

	//--Graphic Data member-begin--

	std::unordered_map<PlayerIdType, BSHand*> hands;
	std::unordered_map<PlayerIdType, std::shared_ptr<BSDeck>> decks;
	std::unordered_map<PlayerIdType, BSBoard*> boards;

	std::shared_ptr<BSCoinFlipper> coinFlipper;

	std::unordered_map<PlayerIdType, std::shared_ptr<BSPlayer>> players;

	cocos2d::ui::Layout* getBattleSceneUI() { return root; }
	cocos2d::ui::Layout *root = nullptr; // * Scene load tu csb

	std::unordered_map<PlayerIdType, cocos2d::ui::Layout*> playerPanels;

	//--Data member-end--




	
	//--Action pipeline members-begin--

	std::list<BSAction*> pipeline; CHECK_CLEAN_FLAG
	void startPipeline();
	void clearPipeline();
	void updatePipeline(float dt);
	void pushActions(std::initializer_list<BSAction*> list);

	//--Action pipeline-end--

	BSCard *detailedCard = nullptr; CHECK_CLEAN_FLAG
	void showCardDetails(const std::shared_ptr<const CardData> &card);
	void hideCardDetails();
	void clearCardDetails();


	// Utilize functions
	void checkClean();

public:
	std::shared_ptr<BattleManager> getBattleManager() { return btlMgr; }
	static BattleScene* BattleScene::getScene();
private:
	std::shared_ptr<BattleManager> btlMgr;
};

BATTLE_SCENE_NS_END

#endif // __BATTLE_SCENE_H__
