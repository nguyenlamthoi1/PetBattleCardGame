#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIText.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>


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

	void doPlayGame();
	void doContinue();
	void doQuitGame();
	void doLangSet();

	void showOptionPage(unsigned char pageId);
	void showOptions(const std::vector<Option> &options);

	const unsigned char PAGE_0 = 0;
	const unsigned char PAGE_1 = 1;
	const unsigned char PAGE_2 = 2; // Page for language
	const unsigned char PAGE_3 = 3; // Page for setting
	const unsigned char PAGE_4 = 4;
	std::unordered_map<unsigned char, std::vector<Option>> pages;
	std::vector<cocos2d::ui::Text*> texts;
	cocos2d::ui::Layout *ui = nullptr;
	void doClear();
};

#endif // __TITLE_SCENE_H__
