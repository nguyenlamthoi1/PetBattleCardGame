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

using BManagerPtr = std::shared_ptr<BattleManager>;

class BattleScene final : public cocos2d::Scene
{
public:
	friend class BSHand;
	friend class BSDeck;
	friend class BSBoard;

	static BattleScene* getScn();
    static BattleScene* create();

	BattleScene();
	virtual ~BattleScene();

    virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;

	void start();
	
	//--Graphic Data member-begin--

	//BManagerPtr bm;
	std::unordered_map<PlayerIdType, std::shared_ptr<PlayerData>> playerData;
	std::unordered_map<PlayerIdType, BSHand*> hands;
	std::unordered_map<PlayerIdType, BSDeck*> decks;
	std::unordered_map<PlayerIdType, BSBoard*> boards;

	std::unordered_map<PlayerIdType, std::shared_ptr<BSPlayer>> players;

	cocos2d::ui::Layout *ui = nullptr;
	std::unordered_map<PlayerIdType, cocos2d::ui::Layout*> playerPanels;
	cocos2d::ui::Layout* getBattleSceneUI() { return ui; }

	//--Data member-end--

	
	//--Action pipeline members-begin--

	std::list<BSAction*> pipeline;
	void startPipeline();
	void clearPipeline();
	void updatePipeline(float dt);
	void pushActions(std::initializer_list<BSAction*> list);

	//--Action pipeline-end--

};

BATTLE_SCENE_NS_END

#endif // __BATTLE_SCENE_H__
