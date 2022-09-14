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

BATTLE_SCENE_NS_BEG

class BattleScene;
class BSCard;

class BSPrizePile
{
public:
	using CardId = std::string;
	static std::shared_ptr<BSPrizePile> create(BattleScene *scn, cocos2d::ui::Layout *layout);
	BSPrizePile(BattleScene *scn, cocos2d::ui::Layout *layout);
	virtual ~BSPrizePile();

	void drawFromDeck(const std::vector<CardId> &idvec);
protected:

	virtual bool init();

	BattleScene *btlScn = nullptr;

	cocos2d::ui::Layout *root = nullptr;
	cocos2d::Node *cardMarker = nullptr;
	cocos2d::ui::Text *numLabel = nullptr;

	std::vector<BSCard*> cardVec;
	unsigned int capacity = 0;
};

BATTLE_SCENE_NS_END