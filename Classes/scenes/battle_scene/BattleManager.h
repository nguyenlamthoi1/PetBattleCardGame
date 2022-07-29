#ifndef __BATTLE_MANAGER_H__
#define __BATTLE_MANAGER_H__

#include "cocos2d.h"
#include "BSDefine.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <list>
#include <unordered_map>

BATTLE_SCENE_NS_BEG

class BattleScene;
class BMAction;

class BattleManager final {
public:
	friend class BattleScene;
	friend class BMAction;

	static const std::string BATTLE_MANAGER_SCHEDULER;

	BattleManager();
	~BattleManager();

	void start();
	void setupFirstActions();
	void startPipeline(); // * duoc call khi bat dau BattleScene, moi hanh dong cua scene se duoc thuc thi lan luot qua cac action
	void processPipeline(float dt); // * duoc call moi frame
	void pushAction(BMAction *action); 
private:
	std::list<BMAction*> actionPipeline;
};

BATTLE_SCENE_NS_END

#endif // __BATTLE_MANAGER_H__
