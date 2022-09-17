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

//class BSPrizeHolder {
//public:
//	friend class BSPrizeSelector;
//
//	using OnHolderTouched = std::function<void(BSPrizeHolder *)>;
//
//	static std::shared_ptr<BSPrizeHolder> create(cocos2d::Node *rootnode);
//	BSPrizeHolder(cocos2d::Node *rootnode);
//	virtual ~BSPrizeHolder();
//	bool init();
//
//	cocos2d::Node* getRoot() const { return root; }
//
//	unsigned int idx = 0;
//
//	cocos2d::Node *root = nullptr;
//	cocos2d::ui::Layout *container = nullptr;
//	cocos2d::Node *cardMarker = nullptr;
//	cocos2d::ui::ImageView *selectBorder = nullptr;
//	cocos2d::ui::Layout *touchPanel = nullptr;
//
//	BSCard *bsCard = nullptr;
//	bool selected = false;
//	bool selectable = false;
//	OnHolderTouched onTouched;
//
//	void addCard(BSCard *card);
//	BSCard* removeCard();
//
//	void setSelectable(bool e);
//	void setSelected(bool s);
//	void setOnTouched(const OnHolderTouched f) { onTouched = f; }
//	BSCard* getCard() const { return bsCard; }
//	bool hasCard() const { return bsCard != nullptr; }
//	bool isSelected() const { return selectable && selected; }
//	bool isSelectable() const { return selectable; }
//};

class UnselectedCardHolder : public cocos2d::ui::Layout {
public:
	using CardId = std::string;
	static UnselectedCardHolder* create();
	UnselectedCardHolder();
	virtual ~UnselectedCardHolder();

	cocos2d::Node* getRoot() const { return root; }
	void renderWith(const CardId &cid, unsigned int stackedNum);

protected:
	virtual bool init() override;

	cocos2d::Node *root = nullptr;
	cocos2d::ui::Layout *container = nullptr;
	cocos2d::ui::ImageView *numPanel = nullptr;
	cocos2d::ui::Text *numLabel = nullptr;

	CardId cid;
	unsigned int stackedNum = 0;
};

class BSCardSelector: public IEventsHandler
{
public:
	using CardId = std::string;
	using SelectType = GConfig::SelectType;
	using SelectMap = std::map<SelectType, unsigned int>;

	/// All Events ///
	static const std::string ON_DONE_SELECT; // Event Data is Null

	using CardId = std::string;
	static std::shared_ptr<BSCardSelector> create(BattleScene *scn);
	BSCardSelector(BattleScene *scn);
	virtual ~BSCardSelector();


	void showToSelect(const std::vector<CardId> &cidVec, const SelectMap &selMap);

	void clearUI();

	cocos2d::Node* getRoot() const { return root; }
protected:

	virtual bool init();

	BattleScene *btlScn = nullptr;
	cocos2d::Node *root = nullptr;

	// Unselected Panel
	cocos2d::ui::Layout *unselectedPanel = nullptr;
	cocos2d::ui::Layout *unselectedContainer = nullptr;
	cocos2d::ui::Layout *unselectedTouchPanel = nullptr;
	cocos2d::ui::Button *unselectedPrevBtn = nullptr;
	cocos2d::ui::Button *unselectedNextBtn = nullptr;

};

BATTLE_SCENE_NS_END