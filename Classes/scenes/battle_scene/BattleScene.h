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
class BSNotifier;

namespace BATTLE_SCENE_Z {
	const float DETAILED_CARD = 1000;
};

class BattleScene final : public cocos2d::Scene
{
public:
	friend class BSHand;
	friend class BSDeck;
	friend class BSBoard;

	static BattleScene* BattleScene::getScene();
	static BattleScene* getScn();
    static BattleScene* create();
	std::shared_ptr<BattleManager> getBattleManager() { return btlMgr; }

	std::shared_ptr<BSPlayer> getPlayer(PlayerIdType id) { return players[id]; }
	std::shared_ptr<PlayerData> getPlayerData(PlayerIdType id) { return playerData[id]; }
	std::shared_ptr<BSBoard> getBoard(PlayerIdType id) { return boards[id]; }
	std::shared_ptr<BSCoinFlipper> getCoinFlipper() { return coinFlipper; }
	std::shared_ptr<BSNotifier> getNotifier() { return notifier; }

private:
	BattleScene();
	virtual ~BattleScene();

	virtual bool init() override;
	void initTopLayer();

	virtual void onEnter() override;
	virtual void onExit() override;

	void start();

private:
	std::unordered_map<PlayerIdType, std::shared_ptr<PlayerData>> playerData;

public:

	//--Graphic Data member-begin--

	std::unordered_map<PlayerIdType, BSHand*> hands;
	std::unordered_map<PlayerIdType, std::shared_ptr<BSDeck>> decks;
	std::unordered_map<PlayerIdType, std::shared_ptr<BSBoard>> boards;

	std::shared_ptr<BSCoinFlipper> coinFlipper;
	std::shared_ptr<BSNotifier> notifier;

	std::unordered_map<PlayerIdType, std::shared_ptr<BSPlayer>> players;

	cocos2d::ui::Layout* getBattleSceneUI() { return root; }
	cocos2d::ui::Layout *root = nullptr; // * Scene load tu csb
	cocos2d::ui::Layout *topLayout = nullptr;

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

private:
	std::shared_ptr<BattleManager> btlMgr;
	std::string oppId;
};

BATTLE_SCENE_NS_END

#endif // __BATTLE_SCENE_H__
