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

namespace MGame {
	class BattleMaster;
}

BATTLE_SCENE_NS_BEG

class BSResources;
class BSDeck;
//class BSAction;
//class BSBoard;
//class BSPlayer;
//class BSCard;
//class BSCoinFlipper;
//class BSNotifier;
class BSHand;
class BSAction;

//namespace MGame {
//	class BattleManager;
//}


namespace BATTLE_SCENE_Z {
	const float DETAILED_CARD = 1000;
};

class BattleScene final : public cocos2d::Scene
{
public:
	static BattleScene* BattleScene::getScene();
	static BattleScene* getScn();
	static BattleScene* create();
private:
	static BattleScene *curBtlScn;
public:
	static std::shared_ptr<BSResources> getResources();

	cocos2d::Node* getScnRoot() const { return root; } // * Root node load tu file csb

	//std::shared_ptr<BattleManager> getBattleManager() { return btlMgr; }

	std::shared_ptr<PlayerData> getPlayerData(const PlayerIdType &id);
	std::shared_ptr<BSHand> getHand(const PlayerIdType &id);
	std::shared_ptr<BSDeck> getDeck(const PlayerIdType &id);
	
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
	void startGame();
	///Loading Layout functions

private:
	std::shared_ptr<BSResources> bsres;

	std::unordered_map<PlayerIdType, std::shared_ptr<PlayerData>> playerData; // * Thong tin data cua moi nguoi choi, bao gom: deck, card back, loai coin duoc dung, avatar,..

	PlayerIdType pid = PlayerIdInvalid; // Id cua nguoi choi
	PlayerIdType oid = PlayerIdInvalid; // Id cua doi thu


	///Graphic Data member///
	cocos2d::Node *root = nullptr; // * Scene load tu csb

	std::unordered_map<PlayerIdType, cocos2d::ui::Layout*> playerPanels; // Panel chua cac graphic objects cua moi nguoi choi
	cocos2d::ui::Layout *topLayout = nullptr;
	cocos2d::ui::Layout *loadingLayout = nullptr;

	/// Hands
	std::unordered_map<PlayerIdType, std::shared_ptr<BSHand>> hands;
	std::unordered_map<PlayerIdType, std::shared_ptr<BSDeck>> decks;
//	std::unordered_map<PlayerIdType, BSHand*> hands;
//	std::unordered_map<PlayerIdType, std::shared_ptr<BSBoard>> boards;
//
//	std::shared_ptr<BSCoinFlipper> coinFlipper;
//	std::shared_ptr<BSNotifier> notifier;
//
//	std::unordered_map<PlayerIdType, std::shared_ptr<BSPlayer>> players;
//

	///Action pipline members///
	const static std::string PIPELINE_SCHEDULER;
	enum class PipelineState{
		Wait,
		Process,
	};
	PipelineState pipState = PipelineState::Wait;
	using ActionPtr = std::shared_ptr<BSAction>;
	std::list<ActionPtr> pipeline;

public:
	void updatePipeline(float dt);
	void startPipeline();
	void stopPipeline();
	void clearPipeline();
	void pushAction(const ActionPtr &ptr);
	void pushActions(std::initializer_list<ActionPtr> list);

	///--Action pipeline-end--

	/// Utilize functions
private:
	void checkClean();
};

BATTLE_SCENE_NS_END