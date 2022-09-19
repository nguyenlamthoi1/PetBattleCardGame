#include "ActionParser.h"
#include "MoveData.h"
#include "scenes/battle_scene_3/game/game_actions/GameAction.h"

using namespace std;

//shared_ptr<MGame::GameAction> ActionParser::create(const shared_ptr<const ActionData> &adata){
//	auto aid = adata->id;
//	if (aid == "Attack_Default") {
//		auto baseDmg = adata->ovrValues.at(to_string(0));
//		return make_shared<MGame::DefaultAttack>();
//	}
//	else if(false) {
//		return nullptr;
//	}
//	return nullptr;
//}
