#pragma once

#include "base_classes/EventsHandler/IEventsHandler.h"

#include "../BSDefine.h"

#include "cocos2d.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"
#include "ui/UIText.h"
#include "ui/UIImageView.h"

#include <initializer_list>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_map>

BATTLE_SCENE_NS_BEG

class BattleScene;
class BSCard;

class BSPrizeHolder {
public:
	static std::shared_ptr<BSPrizeHolder> create(cocos2d::Node *rootnode);
	BSPrizeHolder(cocos2d::Node *rootnode);
	virtual ~BSPrizeHolder();
	bool init();

	cocos2d::Node* getRoot() const { return root; }

	cocos2d::Node *root = nullptr;
	cocos2d::ui::Layout *container = nullptr;
	cocos2d::Node *cardMarker = nullptr;
	cocos2d::ui::ImageView *selectBorder = nullptr;
};

class BSPrizeSelector : public IEventsHandler
{
public:
	/// All Events ///
	static const std::string EV_DRAW_ACTION_DONE; // Event Data is Null

	using CardId = std::string;
	static std::shared_ptr<BSPrizeSelector> create(BattleScene *scn);
	BSPrizeSelector(BattleScene *scn);
	virtual ~BSPrizeSelector();

	cocos2d::Node* getRoot() const { return root; }
protected:

	virtual bool init();

	BattleScene *btlScn = nullptr;
	cocos2d::Node *root = nullptr;
	cocos2d::ui::Layout *container = nullptr;

	std::vector<std::shared_ptr<BSPrizeHolder>> holderVec;
};

BATTLE_SCENE_NS_END