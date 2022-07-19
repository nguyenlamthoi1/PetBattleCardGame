#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>


class BattleScene : public cocos2d::Scene
{
public:
    static BattleScene* create();
protected:
	BattleScene();
	virtual ~BattleScene();

    virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	
	cocos2d::ui::Layout *ui = nullptr;
};

#endif // __BATTLE_SCENE_H__
