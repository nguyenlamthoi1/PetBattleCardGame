#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"


class LoadingScene : public cocos2d::Scene
{
public:
    static LoadingScene* create();
	
protected:
	LoadingScene();
	virtual ~LoadingScene();
    virtual bool init() override;

	virtual void onEnter() override;
	virtual void onExit() override;

	void showLogoUI();

	cocos2d::ui::Layout *layout = nullptr; // Layout chua cac obj
	cocos2d::ui::Text *bottomLabel = nullptr; // Layout chua cac obj

};

#endif // __LOADING_SCENE_H__
