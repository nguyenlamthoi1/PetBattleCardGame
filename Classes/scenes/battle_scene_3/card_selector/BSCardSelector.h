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

class BSSelectedCardHolder;
class BSUnselectedCardHolder;
class BattleScene;
class BSCard;


class BSCardSelector : public IEventsHandler
{
public:
	///--All Events--//
	static const std::string SELECT_DONE;
	
	static const std::string UPDATE_SCHEDULER;


	friend class HelloWorld;
	using CardId = std::string;
	using SelectType = GConfig::SelectType;
	using SelectMap = std::map<SelectType, unsigned int>;

	/// All Events ///
	static const std::string ON_DONE_SELECT; // Event Data is Null

	using CardId = std::string;
	static std::shared_ptr<BSCardSelector> create(BattleScene *root);
	BSCardSelector(BattleScene *root);
	virtual ~BSCardSelector();

	void showToSelect(const std::vector<CardId> &cidVec, const SelectMap &selMap, bool selectFull);
	void hide();
	void clearUI();
	void update(float t);


	cocos2d::Node* getRoot() const { return root; }
protected:

	virtual bool init();
	bool initUnselListView();
	bool initSelListView();

	BattleScene *btlScn = nullptr;

	cocos2d::Node *root = nullptr;
	std::vector<BSCard*> cardVec;

	bool selectFull = false;

	// Unselected Panel
	cocos2d::ui::Layout *unselectedPanel = nullptr;
	cocos2d::ui::ListView* unselectedListView = nullptr;
	cocos2d::ui::Button *unselectedPrevBtn = nullptr;
	cocos2d::ui::Button *unselectedNextBtn = nullptr;
	std::vector<BSUnselectedCardHolder*> unSelHolderVec;
	void onSelItemTouched(cocos2d::Ref* sender, cocos2d::ui::ListView::EventType e);

	// Selected Panel
	cocos2d::ui::Layout *selectedPanel = nullptr;
	cocos2d::ui::ListView* selectedListView = nullptr;
	cocos2d::ui::Button *selectedPrevBtn = nullptr;
	cocos2d::ui::Button *selectedNextBtn = nullptr;
	std::vector<BSSelectedCardHolder*> selHolderVec;
	void onUnSelItemTouched(cocos2d::Ref* sender, cocos2d::ui::ListView::EventType e);


	// Message
	cocos2d::ui::Layout *msgBoard = nullptr;

	cocos2d::ui::Button *doneBtn = nullptr;
	void onDoneBtnClicked(cocos2d::Ref* sender);

	void setMsg(const std::string &msg);
	std::string createSelectMsg(const SelectMap &selMap);

	bool canDoneSelect() const;
};

BATTLE_SCENE_NS_END