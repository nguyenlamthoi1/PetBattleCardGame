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

BATTLE_SCENE_NS_BEG
class BattleManager;
class BSHand;
class BSDeck;
class BSAction;
class PlayerData;

using BManagerPtr = std::shared_ptr<BattleManager>;

class BattleScene final : public cocos2d::Scene
{
public:
	friend class BSHand;
	friend class BSDeck;

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
	std::unordered_map<PlayerIdType, ::PlayerData*> playerData;
	std::unordered_map<PlayerIdType, BSHand*> hands;
	std::unordered_map<PlayerIdType, BSDeck*> decks;


	cocos2d::ui::Layout *ui = nullptr;

	//--Data member-end--

	
	//--Action pipeline members-begin--

	std::list<BSAction*> pipeline;
	void startPipeline();
	void clearPipeline();
	void updatePipeline(float dt);

	//--Action pipeline-end--

};

BATTLE_SCENE_NS_END

#endif // __BATTLE_SCENE_H__
