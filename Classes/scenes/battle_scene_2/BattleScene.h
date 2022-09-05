#pragma once

#include "BSDefine.h"

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"
#include "ui/UIImageView.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

class PlayerData;

BATTLE_SCENE_NS_BEG

//class BattleManager;
//class BSHand;
//class BSDeck;
//class BSAction;
//class BSBoard;
//class BSPlayer;
//class BSCard;
//class BSCoinFlipper;
//class BSNotifier;

namespace BATTLE_SCENE_Z {
	const float DETAILED_CARD = 1000;
};

class BattleScene final : public cocos2d::Scene
{
public:
	//friend class BSHand;
	//friend class BSDeck;
	//friend class BSBoard;

	static BattleScene* BattleScene::getScene();
	static BattleScene* getScn();
	static BattleScene* create();

	cocos2d::Node* getScnRoot() const { return root; } // * Root node load tu file csb

	//std::shared_ptr<BattleManager> getBattleManager() { return btlMgr; }

	std::shared_ptr<PlayerData> getPlayerData(const PlayerIdType &id) { return playerData[id]; }
	//std::shared_ptr<BSPlayer> getPlayer(PlayerIdType id) { return players[id]; }
	//std::shared_ptr<BSBoard> getBoard(PlayerIdType id) { return boards[id]; }
	//std::shared_ptr<BSCoinFlipper> getCoinFlipper() { return coinFlipper; }
	//std::shared_ptr<BSNotifier> getNotifier() { return notifier; }

private:
	BattleScene();
	virtual ~BattleScene();

	virtual bool init() override;
	void initTopLayer();

	virtual void onEnterTransitionDidFinish() override;
	virtual void onExit() override;

	void startLoadingAndInit();
	void doLoading();
	void initGame();
	///Loading Layout functions

private:
	std::unordered_map<PlayerIdType, std::shared_ptr<PlayerData>> playerData; // * Thong tin data cua moi nguoi choi, bao gom: deck, card back, loai coin duoc dung, avatar,..

	///Graphic Data member///
	cocos2d::Node *root = nullptr; // * Scene load tu csb

	std::unordered_map<PlayerIdType, cocos2d::ui::Layout*> playerPanels; // Panel chua cac graphic objects cua moi nguoi choi
	cocos2d::ui::Layout *topLayout = nullptr;
	cocos2d::ui::Layout *loadingLayout = nullptr;


//	std::unordered_map<PlayerIdType, BSHand*> hands;
//	std::unordered_map<PlayerIdType, std::shared_ptr<BSDeck>> decks;
//	std::unordered_map<PlayerIdType, std::shared_ptr<BSBoard>> boards;
//
//	std::shared_ptr<BSCoinFlipper> coinFlipper;
//	std::shared_ptr<BSNotifier> notifier;
//
//	std::unordered_map<PlayerIdType, std::shared_ptr<BSPlayer>> players;
//
//

//
//	//--Data member-end--
//	
	//--Action pipeline members-begin--

	//std::list<BSAction*> pipeline; CHECK_CLEAN_FLAG
	//void startPipeline();
	//void clearPipeline();
	//void updatePipeline(float dt);
	//void pushActions(std::initializer_list<BSAction*> list);

	//--Action pipeline-end--




	/// Utilize functions
	void checkClean();

private:
//	std::shared_ptr<BattleManager> btlMgr;
};

BATTLE_SCENE_NS_END