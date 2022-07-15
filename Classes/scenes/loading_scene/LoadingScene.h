#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"

namespace EVENT_CUSTOM {
	struct EC_LoadStepFinishedData;
	struct EC_LoadingFinishedData;
}

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

	void playScene();
	void startLoading();

	// Callbacks
	void onFileLoaded(EVENT_CUSTOM::EC_LoadStepFinishedData*);
	void onLoadingFinished(EVENT_CUSTOM::EC_LoadingFinishedData*);
	void onLoadingFinished();

	static const std::string LOGO_NAME;
	static const std::string CIRCLE_NAME;

	cocos2d::ui::Layout *layout = nullptr; // Layout chua cac obj
	cocos2d::ui::Text *bottomLabel = nullptr; // Layout chua cac obj

};

#endif // __LOADING_SCENE_H__
