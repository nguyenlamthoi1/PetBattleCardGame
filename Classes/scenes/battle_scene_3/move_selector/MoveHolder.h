#pragma once

#include "base_classes/EventsHandler/IEventsHandler.h"

#include "../BSDefine.h"

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"
#include "ui/UIImageView.h"
#include "ui/UIListView.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>



BATTLE_SCENE_NS_BEG

class MoveHolder : public cocos2d::ui::Layout
{
public:
	using CardId = std::string;

	MoveHolder* create(const );
	MoveHolder();
	virtual ~MoveHolder();

	cocos2d::Node* getRoot() const { return root; }

	void cleanUI();

protected:

	virtual bool init();

	cocos2d::Node *root = nullptr;
	
};

BATTLE_SCENE_NS_END