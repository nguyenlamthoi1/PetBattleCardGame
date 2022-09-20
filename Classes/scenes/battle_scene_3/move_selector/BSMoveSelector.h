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

class BattleScene;
class BSCard;
class CardHolder;
class MoveHolder;

class BSMoveSelector : public IEventsHandler
{
public:
	using CardId = std::string;
	static std::shared_ptr<BSMoveSelector> create(BattleScene *scn);
	BSMoveSelector(BattleScene *scn);
	virtual ~BSMoveSelector();

	cocos2d::Node* getRoot() const { return root; }

	void showInfoHolder(CardHolder *holder, bool allowUseMove = false, bool allowRetreat = false);
	void showInfoHolder2(CardHolder *holder);

	void hide();
	void cleanUI();

protected:

	virtual bool init();

	BattleScene *btlScn = nullptr;
	cocos2d::Node *root = nullptr;
	cocos2d::ui::Layout *container = nullptr;
	cocos2d::ui::Layout *blackPanel = nullptr;
	cocos2d::ui::Layout *topPanel = nullptr;
	cocos2d::ui::Text *petNameLb = nullptr;
	cocos2d::ui::Text *curHpLb = nullptr;
	cocos2d::ui::Text *maxHpLb = nullptr;

	cocos2d::ui::Layout *eSpritePanel = nullptr;
	void updateEnergySpritePanel();
	
	cocos2d::ui::ListView *listView = nullptr;

	cocos2d::ui::Layout *bottomPanel = nullptr;

	cocos2d::ui::Layout *weakPanel = nullptr;
	cocos2d::Node *weakEnergyMarker = nullptr;


	cocos2d::ui::Layout *resistPanel = nullptr;
	cocos2d::Node *resistEnergyMarker = nullptr;

	cocos2d::ui::Layout *retreatPanel = nullptr;
	cocos2d::ui::Layout *retreatEnergyPanel = nullptr;
	cocos2d::ui::Button *retreatBtn = nullptr;
	void updateRetreatEnergySpritePanel();

	cocos2d::Node *cardMarker1 = nullptr;
	cocos2d::Node *cardMarker2 = nullptr;
	std::vector<BSCard*> cardVec;
	unsigned int shownCurIdx = 0;
	cocos2d::ui::Button *nextBtn = nullptr;
	cocos2d::ui::Button *prevBtn = nullptr;

	void onClickNextBtn(cocos2d::Ref *sender);
	void onClickPrevBtn(cocos2d::Ref *sender);
	void updateCardPos();

	void onClickUseMove(CardHolder * cHolder, MoveHolder *mHolder, unsigned int moveIdx);

	PlayerIdType curPid;
};

BATTLE_SCENE_NS_END