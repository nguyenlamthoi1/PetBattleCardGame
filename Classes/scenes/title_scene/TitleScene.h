#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>


class TitleScene : public cocos2d::Scene
{
public:
	using OnClickedFunc = std::function<void()>;
	using Option = std::pair<std::string, OnClickedFunc>;
	using size_type = std::vector<Option>::size_type;
    static TitleScene* create();

protected:
	TitleScene();
	virtual ~TitleScene();

    virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;

	void showFirstOptions();
	void showOptions(std::vector<Option> options);
	void showPrevOptions(); // * Chi nen duoc goi tu OnClickedFunc cua string("Back")

	void doPlayGame();
	void doContinue();
	void doQuitGame();
	void doLangSet();

	std::vector<Option> curOpVec;
	std::vector<Option> preOpVec;
	std::vector<cocos2d::ui::Text*> texts;
	cocos2d::ui::Layout *ui = nullptr;
	void doClear();
};

#endif // __TITLE_SCENE_H__
