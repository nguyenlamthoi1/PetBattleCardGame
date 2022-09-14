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


class TestScene : public cocos2d::Scene
{
public:
	static TestScene* create();
	TestScene();
	virtual ~TestScene();

	virtual bool init() override;

	virtual void onEnter() override;
	virtual void onExit() override;

	cocos2d::Node *root = nullptr;
	
};