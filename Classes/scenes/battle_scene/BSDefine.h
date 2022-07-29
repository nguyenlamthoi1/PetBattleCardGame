#ifndef __BS_DEFINE_H__
#define __BS_DEFINE_H__

#include "cocos2d.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

#define BATTLE_SCENE_NS_BEG namespace BattleSceneNS {
#define BATTLE_SCENE_NS_END }
//#define BATTLE_SCENE_NS_BEG 
//#define BATTLE_SCENE_NS_END 

#define BATTLE_SCENE_USE_NS using namespace BattleSceneNS // Ket thuc bang ';'

#define BATTLE_MANAGER_NS_BEG namespace BattleManagerNS {
#define BATTLE_MANAGER_NS_END }
#define BATTLE_MANAGER_NS_USE using namespace BattleManagerNS // Ket thuc bang ';'

BATTLE_SCENE_NS_BEG
using PlayerIdType = unsigned char;
constexpr PlayerIdType PLAYER = 77;
constexpr PlayerIdType OPPONENT = PLAYER + 1; // OPPONENT > PLAYER
#define PlayerIdInvalid -1

BATTLE_SCENE_NS_END

#endif // __BS_DEFINE_H__
