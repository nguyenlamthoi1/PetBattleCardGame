#include "MoveHolder.h"

#include "GameManager.h"
#include "common/ResourceLoader.h"
#include "common/ResourcePool.h"
#include "common/LangSys.h"
#include "ui/UIHelper.h"

#include "scenes/battle_scene_3/BattleScene.h"
#include "scenes/battle_scene_3/BSCard.h"
#include "scenes/battle_scene_3/BSResources.h"
#include "scenes/battle_scene_3/BSPrizePile.h"
#include "scenes/battle_scene_3/CardHolder.h"
#include "scenes/battle_scene_3/prefabs/BSPrefabs.h"


#include "data/CardData.h"


#include "scenes/battle_scene_3/game/player_actions/PlayerAction.h"


USING_NS_CC;
using namespace std;
BS_PREFABS_USE_NS;

BATTLE_SCENE_NS_BEG

MoveHolder* MoveHolder::create() {
	auto ret = new (nothrow) MoveHolder();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	delete ret;
	ret = nullptr;
	return ret;
}

BSMoveSelector::BSMoveSelector(BattleScene *scn) : btlScn(scn) {}

BSMoveSelector::~BSMoveSelector(){
	cleanUI();
}

bool BSMoveSelector::init() {
	if (!btlScn)
		return false;

	auto scnroot = btlScn->getScnRoot();
	root = scnroot->getChildByName("Attack_Selector");
	if (!root)
		return false;

	
	return true;
}

	//}

BATTLE_SCENE_NS_END