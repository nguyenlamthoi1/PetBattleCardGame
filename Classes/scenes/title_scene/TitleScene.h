#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"


class TitleScene : public cocos2d::Scene
{
public:

    static TitleScene* create();
	
protected:
	TitleScene();
	virtual ~TitleScene();

    virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
};

#endif // __TITLE_SCENE_H__
