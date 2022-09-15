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
	friend class BSPrizeSelector;

	using OnHolderTouched = std::function<void(BSPrizeHolder *)>;

	static std::shared_ptr<BSPrizeHolder> create(cocos2d::Node *rootnode);
	BSPrizeHolder(cocos2d::Node *rootnode);
	virtual ~BSPrizeHolder();
	bool init();

	cocos2d::Node* getRoot() const { return root; }

	unsigned int idx = 0;

	cocos2d::Node *root = nullptr;
	cocos2d::ui::Layout *container = nullptr;
	cocos2d::Node *cardMarker = nullptr;
	cocos2d::ui::ImageView *selectBorder = nullptr;
	cocos2d::ui::Layout *touchPanel = nullptr;

	BSCard *bsCard = nullptr;
	bool selected = false;
	bool selectable = false;
	OnHolderTouched onTouched;

	void addCard(BSCard *card);
	BSCard* removeCard();

	void setSelectable(bool e);
	void setSelected(bool s);
	void setOnTouched(const OnHolderTouched f) { onTouched = f; }
	BSCard* getCard() const { return bsCard; }
	bool hasCard() const { return bsCard != nullptr; }
	bool isSelected() const { return selectable && selected; }
	bool isSelectable() const { return selectable; }
};

class BSPrizeSelector : public IEventsHandler
{
public:
	/// All Events ///
	static const std::string ON_DONE_SELECT; // Event Data is Null

	using CardId = std::string;
	static std::shared_ptr<BSPrizeSelector> create(BattleScene *scn);
	BSPrizeSelector(BattleScene *scn);
	virtual ~BSPrizeSelector();

	cocos2d::Node* getRoot() const { return root; }

	void showPrizeCards(const PlayerIdType &pid);
	void showPrizeCardsToSelect(const PlayerIdType &pid, unsigned int num);
	void hidePrizeCard();

protected:

	virtual bool init();

	BattleScene *btlScn = nullptr;
	cocos2d::Node *root = nullptr;
	cocos2d::ui::Layout *container = nullptr;
	cocos2d::ui::Button *doneBtn = nullptr;
	cocos2d::ui::Button *showMatchBtn = nullptr;

	PlayerIdType curPid;
	std::vector<std::shared_ptr<BSPrizeHolder>> holderVec;
	
	std::vector<unsigned int> idxVec; // Luu index cua cac holder duoc chon
	unsigned int selectNum = 0;
	unsigned int selectedNum = 0;

	void onHolderTouched(BSPrizeHolder *holder);
	void onDoneBtnTouched(cocos2d::Ref*);
	void onShowMatchBtnTouched(cocos2d::Ref*);
};

BATTLE_SCENE_NS_END