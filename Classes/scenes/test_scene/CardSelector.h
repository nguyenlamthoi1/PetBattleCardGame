#pragma once

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>


class CardSelector
{
public:
	static std::shared_ptr<CardSelector> create(cocos2d::Node *rootnode);
	CardSelector(cocos2d::Node *rootnode);
	virtual ~CardSelector();
protected:

	virtual bool init();

	cocos2d::Node *root = nullptr;

	// Xu ly Unselected Panel
	cocos2d::ui::Layout *unselectedPanel = nullptr;
	cocos2d::ui::Layout *unselectedContainer = nullptr;
	cocos2d::ui::Layout *unselectedTouchPanel = nullptr;
	cocos2d::ui::Button *unselectedBtnNext = nullptr;
	cocos2d::ui::Button *unselectedBtnPrev = nullptr;

	bool onUnselectedTouchBegan(cocos2d::Touch*, cocos2d::Event*);
	void onUnselectedTouchEnded(cocos2d::Touch*, cocos2d::Event*);
	void onUnselectedTouchMoved(cocos2d::Touch*, cocos2d::Event*);
	void onUnselectedTouchCancelled(cocos2d::Touch*, cocos2d::Event*);

	cocos2d::ui::Layout *selectedPanel = nullptr;
	
};